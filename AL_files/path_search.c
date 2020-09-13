#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "parser.h"
#include "path_search.h"

void search_for_command(char *command, tokenlist *tokens)
{
	char *mainPATH = getenv("PATH");

	char *copyPATH = malloc(strlen(mainPATH));
	strcpy(copyPATH, mainPATH);

	char **parsedPATH = NULL;
	char *tok = strtok(copyPATH, ":");
	int size = 0;

	//allocates space in array for each individual path
	while (tok)
	{
		parsedPATH = realloc(parsedPATH, (sizeof(char *) * ++size));

		if (parsedPATH == NULL)
			exit(-1);

		parsedPATH[size - 1] = tok; //sets char pointer at size-1 to the token
		tok = strtok(NULL, ":");
	}

	//null-termination of array
	parsedPATH = realloc(parsedPATH, sizeof(char *) * size + 1);
	parsedPATH[size] = 0;

	//append command to a temp variable that is overwritten in each iteration to check then next path

	int i;
	for (i = 0; i < size; i++)
	{
		if (parsedPATH[i] != NULL)
		{
			int size_0 = strlen(parsedPATH[i]) + strlen(command) + 1;
			char *temp_command = (char *)malloc(size_0);
			strcpy(temp_command, parsedPATH[i]);
			strcat(temp_command, "/");
			strcat(temp_command, command);
			//printf("temp_command at %d: %s\n", i, temp_command);
			if (does_command_exist(temp_command))
			{
				printf("Running Command\n"); //insert command execution here
				execute_command(temp_command, tokens, 1);
				printf("After running command\n");

				break;
			}
		}
	}

	if (size == i)
		printf("command not found\n");

	free(parsedPATH);
}

void execute_command(char *cmdpath, tokenlist *tokens, int checkCallLocation)
{
	int isOutput = 0;
	int isInput = 0;
	int isBoth = 0;
	int fd_in = 0;
	int fd_out = -1;
	int store_out_index = -1;
	int store_in_index = -1;
	int lowest_index = 0;
	char *infile = NULL;
	char *outfile = NULL;
	int pipettes[2];
	int hasPipe = 0;
	tokenlist *temp = new_tokenlist();

	if (checkCallLocation == 1)
		free(tokens->items[0]); //free memory at this space so we can replace

	tokens->items[0] = cmdpath;

	for (int i = 0; i < tokens->size - 1; i++)
	{
		printf("token %d: (%s)\n", i, tokens->items[i]);

		if (strcmp(tokens->items[i], ">") == 0) //output redir
		{
			printf("found output redir '>'\n");
			isOutput = 1;
			store_out_index = i;			//location of > if it exists
			outfile = tokens->items[i + 1]; //outfile will be token after >

			fd_out = open(outfile, O_WRONLY | O_CREAT, 0644);
			if ((fd_out) < 0)
			{
				perror("Output file could not be opened");
				return;
			}
		}
		if (strcmp(tokens->items[i], "<") == 0)
		{
			printf("found input redir '<'\n"); //input redir
			isInput = 1;
			store_in_index = i;
			infile = tokens->items[i + 1];

			fd_in = open(infile, O_RDONLY, 0644);
			if ((fd_in) < 0)
			{
				perror("Input file could not be opened");
				return;
			}
		}

		if (strcmp(tokens->items[i], "|") == 0)
		{ //piping
			printf("piping detected\n");
			hasPipe = 1;
			pipe(pipettes);
			dup(pipettes[0]);
			dup(pipettes[1]);
		}
	}

	printf("'>' is located at index %d\n", store_out_index);
	printf("'<' is located at index %d\n", store_in_index);

	if (store_in_index == -1 && store_out_index > 0)
	{
		printf("there is only output redirection\n\n");
		for (int k = 0; k < store_out_index; k++)
			add_token(temp, tokens->items[k]);
	}
	else if (store_in_index > 0 && store_out_index == -1)
	{
		printf("there is only input redirection\n\n");
		for (int k = 0; k < store_in_index; k++)
			add_token(temp, tokens->items[k]);
	}
	else if (store_in_index > 0 && store_out_index > 0) //if both input and output redir are present
	{
		if (store_in_index > store_out_index) //if the output redir comes first
			lowest_index = store_out_index;
		else if (store_in_index < store_out_index) //if the input redir comes first
			lowest_index = store_in_index;

		for (int k = 0; k < lowest_index; k++)
			add_token(temp, tokens->items[k]);
	}

	for (int k = 0; k < temp->size; k++)
		printf("%d: %s\n", k, temp->items[k]);

	//printf("isOutput: %d\nisInput: %d\nisBoth: %d\n", isOutput, isInput, isBoth);

	pid_t pid1 = fork();

	if (pid1 == 0)
	{
		if (hasPipe == 1)
		{
			close(pipettes[0]);
			//redirect stdout to be input of pipe
			close(STDOUT_FILENO);
			dup(pipettes[1]);
			close(pipettes[1]);
			execv(temp->items[0], temp->items);
		}
		printf("\n\n");
		if (isInput == 0 && isOutput == 0)
		{
			execv(tokens->items[0], tokens->items);
		}

		if (isInput == 1)
		{
			printf("changing input file descriptors\n");
			close(STDIN_FILENO);
			dup(fd_in);
			close(fd_in);
		}
		if (isOutput == 1)
		{
			printf("changing output file descriptors\n");
			close(STDOUT_FILENO);
			dup(fd_out);
			close(fd_out);
		}

		execv(temp->items[0], temp->items);

		exit(1);
	}
	//check for piping first{}
	if (hasPipe == 1)
	{
		pid_t pid2 = fork();
		if (pid2 == 0)
		{
			//redirect stdin to be output of pipe
			close(pipettes[1]);
			close(STDIN_FILENO);
			dup(pipettes[0]);
			close(pipettes[0]);
		}
		else
		{
			free_tokens(temp);

			if (isOutput == 1)
				close(fd_out);
			if (isInput == 1)
				close(fd_in);

			waitpid(pid1, NULL, 0);
			waitpid(pid2, NULL, 0);
			printf("\n\nChild exited\n");
			return; //this waits for both processes in the event of piping
		}
	}
	else
	{
		free_tokens(temp);

		if (isOutput == 1)
			close(fd_out);
		if (isInput == 1)
			close(fd_in);

		waitpid(pid1, NULL, 0);
		printf("\n\nChild exited\n");
	}
}


// checks for exists of a command
int does_command_exist(char *path)
{
	FILE *check = fopen(path, "r");
	if (check == NULL)
		return 0;
	fclose(check);
	return 1;
}
