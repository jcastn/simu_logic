//functions-run-loop.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "structures.h"
#include "prototypes.h"

static void scan_user_entry(char* command_user, Model *model)
{
	char* args[MAX_COMMAND_ARGS] = {NULL};
	int arg_count = 0;
	char* letter = command_user;

	while (*letter != '\0' && arg_count < MAX_COMMAND_ARGS)
	{
		// Skip spaces between args
		while (*letter == ' ' || *letter == '\t' || *letter == '\n' || *letter == '\r') {
			letter++;
		}

		if (*letter == '\0') {
			break;
		}

		// args in quotes (Strings)
		if (*letter == '"') 
		{
			letter++;
			args[arg_count] = letter;

			while (*letter != '"' && *letter != '\0') {
				letter++;
			}

			if (*letter == '"') {
				*letter = '\0';
				letter++;
			}
		}
		// args not in quotes
		else 
		{
			args[arg_count] = letter;

			while (*letter != ' ' && *letter != '\t' && *letter != '\n' && *letter != '\r' && *letter != '\0') {
				letter++;
			}

			if (*letter != '\0') {
				*letter = '\0';
				letter++;
			}
		}
		arg_count++;
	}

	if (args[0] == NULL) {
		return;
	}

	/*
	int i = 0;
	printf("Extracted args :\n");
	while(i < arg_count)
	{
		printf("> arg %d : '%s' \n", i+1, args[i]);
		i++;
	}
	*/
	

	exec_command(args, model, arg_count);
}

void	run_loop(Model *model)
{
	char	user_entry[100];
	
	model->run_loop = true;

	while(model->run_loop)
	{
		usleep(10000);
		printf(APP_PROMPT);

		if (fgets(user_entry, sizeof(user_entry), stdin) != NULL) {
			scan_user_entry(user_entry, model);
		}
	}

	return;
}

