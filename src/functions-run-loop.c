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
	char* words[MAX_COMMAND_WORDS] = {NULL};
	int word_count = 0;
	char* letter = command_user;

	while (*letter != '\0' && word_count < MAX_COMMAND_WORDS)
	{
		// Skip spaces between words
		while (*letter == ' ' || *letter == '\t' || *letter == '\n' || *letter == '\r') {
			letter++;
		}

		if (*letter == '\0') {
			break;
		}

		// Words in quotes (Strings)
		if (*letter == '"') 
		{
			letter++;
			words[word_count] = letter;

			while (*letter != '"' && *letter != '\0') {
				letter++;
			}

			if (*letter == '"') {
				*letter = '\0';
				letter++;
			}
		}
		// Words not in quotes
		else 
		{
			words[word_count] = letter;

			while (*letter != ' ' && *letter != '\t' && *letter != '\n' && *letter != '\r' && *letter != '\0') {
				letter++;
			}

			if (*letter != '\0') {
				*letter = '\0';
				letter++;
			}
		}
		word_count++;
	}

	if (words[0] == NULL) {
		return;
	}

	/*
	int i = 0;
	printf("Extracted words :\n");
	while(i < word_count)
	{
		printf("> Word %d : '%s' \n", i+1, words[i]);
		i++;
	}
	*/
	

	exec_command(words, model, word_count);
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

