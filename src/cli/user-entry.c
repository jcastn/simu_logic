// src/cli/user-entry.c
#include "../../include/prototypes-cli.h"

#ifndef DEBUG_MODE
#include "../../third_party/linenoise/linenoise.h"

void		auto_complete_entry(const char *user_entry, linenoiseCompletions *lc)
{
	int					commands_count = 0;
	const CommandMap*	commands = get_command_map(&commands_count);
	int counter = 0;

	const char*	space = strchr(user_entry, ' ');

	// If there's no space in the entry, it means that we're on the first word 
	if (!space)
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

void 			scan_user_entry(Model* model, char* command_user)
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

	if (!args[0])
	{
		return;
	}

	exec_command(args, model, arg_count);
}