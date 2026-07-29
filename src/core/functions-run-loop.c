//functions-run-loop.c
#include "../../third_party/linenoise/linenoise.h"
#include "../../include/prototypes.h"

#ifdef DEBUG_MODE
#include <unistd.h>
#endif

#ifndef DEBUG_MODE
static void		auto_complete_entry(const char *user_entry, linenoiseCompletions *lc)
{
	int					commands_count = 0;
	const CommandMap*	commands = get_command_map(&commands_count);
	int counter = 0;

	const char*	space = strchr(user_entry, ' ');

	// If there's no space in the entry, it means that we're on the first word 
	if (space == NULL)
	{
		// If we're on the first word, the auto-completion will work on it
		while(counter < commands_count)
		{
			if ((strncmp(user_entry, commands[counter].command, strlen(user_entry)) == 0) && (!commands[counter].is_alias))
			{
				linenoiseAddCompletion(lc, commands[counter].command);
			}
			counter++;
		}
		return;
	}

	// If more than once space, we stop trying to auto-complete
	if (strchr(space + 1, ' ') != NULL)
	{
		return;
	}

	char	first_arg[64];
	char	completion[128];

	snprintf(first_arg, sizeof(first_arg), "%.*s", (int)(space - user_entry), user_entry);

	commands_count = 0;
	const SubCommandMap* sub_commands = get_sub_command_map(first_arg, &commands_count);

	// Loop on all the available sub-commands
	counter = 0;
	while (sub_commands != NULL && counter < commands_count)
	{
		if ((strncmp(space + 1, sub_commands[counter].command, strlen(space + 1)) == 0) && (!sub_commands[counter].is_alias))
		{
			snprintf(completion, sizeof(completion), "%s %s", first_arg, sub_commands[counter].command);
			linenoiseAddCompletion(lc, completion);
		}
		counter++; 
	}

	// If all the options have been showed, "help" is showed
	if (strncmp(space + 1, "help", strlen(space + 1)) == 0)
	{
		snprintf(completion, sizeof(completion), "%s help", first_arg);
		linenoiseAddCompletion(lc, completion);
	}
}
#endif

static void 		scan_user_entry(char* command_user, Model *model)
{
	char* args[MAX_COMMAND_ARGS] = {NULL};
	int arg_count = 0;
	char* letter = command_user;

	while (*letter != '\0' && arg_count < MAX_COMMAND_ARGS)
	{
		// Skip spaces between args
		while (*letter == ' ' || *letter == '\t' || *letter == '\n' || *letter == '\r')
		{
			letter++;
		}

		if (*letter == '\0')
		{
			break;
		}

		// args in quotes (Strings)
		if (*letter == '"') 
		{
			letter++;
			args[arg_count] = letter;

			while (*letter != '"' && *letter != '\0' && *letter != '\n') 
			{
				letter++;
			}

			if (*letter == '"' || *letter == '\n') 
			{
				*letter = '\0';
				letter++;
			}
		}
		// args not in quotes
		else 
		{
			args[arg_count] = letter;

			while (*letter != ' ' && *letter != '\t' && *letter != '\n' && *letter != '\r' && *letter != '\0')
			{
				letter++;
			}

			if (*letter != '\0')
			{
				*letter = '\0';
				letter++;
			}
		}
		arg_count++;
	}

	if (args[0] == NULL)
	{
		return;
	}

	exec_command(args, model, arg_count);
}


#ifndef DEBUG_MODE
void			run_loop(Model *model)
{
	char	prompt[256];

	char*	user_entry;
	linenoiseSetCompletionCallback(auto_complete_entry);
	linenoiseHistorySetMaxLen(100);

	model->run_loop = true;

	while(model->run_loop)
	{
		printf("\n");

		if ((model->active_circuit != NULL) && (strlen(model->active_circuit->label) > 0))
		{
			snprintf(prompt, sizeof(prompt), TERMINAL_CYAN "[" APP_NAME " " APP_VERSION"] "TERMINAL_GREEN "\"%s\"" TERMINAL_CYAN" > "TERMINAL_DEFAULT, model->active_circuit->label);
		}
		else
		{
			snprintf(prompt, sizeof(prompt), TERMINAL_CYAN "[" APP_NAME " " APP_VERSION"] > "TERMINAL_DEFAULT);
		}

		user_entry = linenoise(prompt);
		if (user_entry == NULL) 
		{
			break;
		}

		if (user_entry[0] != '\0')
		{
			linenoiseHistoryAdd(user_entry);
			scan_user_entry(user_entry, model);
		}
		free(user_entry);
	}

	return;
}
#endif

#ifdef DEBUG_MODE
void			run_loop(Model *model)
{
	char	prompt[256];


	model->run_loop = true;

	while(model->run_loop)
	{
		printf("\n");
		usleep(10000);

		if ((model->active_circuit != NULL) && (strlen(model->active_circuit->label) > 0))		
		{
			printf("\n" TERMINAL_CYAN "[" APP_NAME " " APP_VERSION"-"APP_DEBUG"] "TERMINAL_GREEN "\"%s\"" TERMINAL_CYAN" > "TERMINAL_DEFAULT, model->active_circuit->label);
		}
		else {
			printf("\n" TERMINAL_CYAN "[" APP_NAME " " APP_VERSION"-"APP_DEBUG"] > "TERMINAL_DEFAULT);
		}

		if (fgets(prompt, sizeof(prompt), stdin) != NULL)
		{
			scan_user_entry(prompt, model);
		}
	}

	return;
}
#endif