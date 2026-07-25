//functions-run-loop.c
#include <unistd.h>
#include "../../third_party/linenoise/linenoise.h"
#include "../../include/prototypes.h"

static const char*	main_commands[] = {
	"circuit",
	"component",
	"link",
	"list",
	"help",
	"hello",
	"quit",
	"reset",
	NULL
};

// Commands auto-completion (with tab) for linenoise 
static void			linenoise_completion(const char *buf, linenoiseCompletions *lc)
{
	int i = 0;

	while (main_commands[i] != NULL)
	{
		// If what the user wrote is the beggining of a command, it completes it
		if (strncmp(buf, main_commands[i], strlen(buf)) == 0)
		{
			linenoiseAddCompletion(lc, main_commands[i]);
		}
		i++;
	}
}

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

void			run_loop(Model *model)
{
	char*	user_entry;
	char	prompt[256];

	linenoiseSetCompletionCallback(linenoise_completion);
	linenoiseHistorySetMaxLen(100);

	model->run_loop = true;

	while(model->run_loop)
	{
		printf("\n");

		if ((model->active_circuit != NULL) && (strlen(model->active_circuit->label) > 0))
		{
			snprintf(prompt, sizeof(prompt), TERMINAL_CYAN "[" APP_NAME " " APP_VERSION "] "TERMINAL_GREEN "\"%s\"" TERMINAL_CYAN" > "TERMINAL_DEFAULT, model->active_circuit->label);
		}
		else
		{
			snprintf(prompt, sizeof(prompt), TERMINAL_CYAN "[" APP_NAME " " APP_VERSION "] > "TERMINAL_DEFAULT);
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
