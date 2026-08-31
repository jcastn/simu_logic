// src/cli/user-entry.c
#include "prototypes-cli.h"
#include "prototypes-core.h"
#include <stdio.h>

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
	char* args[MAX_COMMAND_ARGS + 1] = {NULL};
	int arg_count;
	int i;

	i = 0;
	arg_count = 0;
	while (command_user[i] != '\0' && arg_count < MAX_COMMAND_ARGS)
	{
		// Skip spaces between args
		while (command_user[i] && find_char_in_str(command_user[i], " \t\n\r"))
			i++;

		if (command_user[i] == '\0')
			break;

		// args in quotes (Strings)
		if (command_user[i] && command_user[i] == '"') 
		{
			i++;
			args[arg_count] = &command_user[i];

			while (command_user[i] && !find_char_in_str(command_user[i], "\"\n"))
				i++;

			if (command_user[i] && find_char_in_str(command_user[i], "\"\n"))
			{
				command_user[i] = '\0';
				i++;
			}
		}
		// args not in quotes
		else 
		{
			args[arg_count] = &command_user[i];

			while (command_user[i] && !find_char_in_str(command_user[i], " \t\n\r"))
				i++;

			if (command_user[i] != '\0')
			{
				command_user[i] = '\0';
				i++;
			}
		}
		//printf("arg %d : %s\n", arg_count, args[arg_count]);
		arg_count++;
	}
	args[arg_count] = NULL;

	if (!args[0])
	{
		return;
	}

	exec_command(args, model, arg_count);
}
