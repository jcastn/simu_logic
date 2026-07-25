//commands.c
#include "../../include/prototypes.h"
#include "../../include/prototypes-commands.h"

// Command hello 
void	command_hello(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)args;
	(void)arg_count;
	(void)model;

	printf(TERMINAL_MAGENTA"\nHello world !\n\n♪(๑ᴖ◡ᴖ๑)♪\n"TERMINAL_DEFAULT);
	return;
}

// Command 'reset' 
void	command_reset(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)args;
	(void)arg_count;

	delete_model(model, false);
	return;
}

// Command quit 
void	command_quit(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)args;
	(void)arg_count;

	printf(MESS_INFO"The app was stopped by the user.\n");

	model->run_loop = false;
	return;
}


void exec_command(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)model;
	int counter = 0;
	int commands_count = 0;

	const CommandMap* commands = get_command_map(&commands_count);
	// Search between user inputs (args) and registered commands
	while(counter < commands_count)
	{
		if (strcmp(args[0], commands[counter].command) == 0)
		{
			// if arg[1] == "help", it directly display the help page of the command
			if ((arg_count >= 2) && (strcmp(args[1], "help") == 0))
			{
				display_help(args[0], commands[counter].description);
				return;
			}

			commands[counter].function(args, model, arg_count);
			return;
		}
		counter++;
	}

	// If the command don't exit
	printf(MESS_SYNTAX"The command you wrote doesn't exist. Please use 'help' command to know the available commands.\n");
	return;
}