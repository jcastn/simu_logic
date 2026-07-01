//functions-run-loop.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"
#include "prototypes.h"




// Command help
static void command_help(char* words[MAX_COMMAND_WORDS], Model *model)
{
	(void)model;

	if (words[1] == NULL)
	{
		printf(MESS_TIP"Tip : You can type 'help' after the name of a command to learn how to use it !\n\nYou can use :\n• 'help commands' to see the list of commands.\n• 'help components' to see the list of available components and how they work (not yet implemented).\n• 'help links' to see how the links work (not yet implemented).\n");
		return;
	}
	if (strcmp(words[1], "commands") == 0)
	{
		printf(MESS_TIP"List of commands :\n• 'help'   : User guide of the app\n• 'list'   : List of all the currently loaded circuits.\n• 'select' : Select an active circuit.\n• 'show'   : Show the components of a circuit (or of all circuits)\n• 'quit'   : Close the application properly.\n");
		return;
	}

	printf(MESS_ERROR"Unknown help topic '%s'. Type 'help' to see all available topics.\n", words[1]);
}

// Command list
static void command_list(char* words[MAX_COMMAND_WORDS], Model *model)
{
	if (words[1] == NULL)
	{
		printf(MESS_SYNTAXE"Please type :\n• 'list help' to learn how to use this command.\n");
		return;
	}

	// If the parameter is "help"
	if (strcmp(words[1], "help") == 0)
	{
		printf(MESS_TIP"'list' command  : It displays a list of all the loaded circuits (with their label, their components count and their links count)\n You can type :\n• 'list circuits'\n");
		return;
	}
	
	// If the parameter is "circuits"
	if (strcmp(words[1], "circuits") == 0)
	{
		int active_circuit_id = -1 ;

		if (model->circuits_count == 0)
		{
			printf(MESS_INFO"No circuits are currently loaded\n");
			return;
		}

		if (model->active_circuit != NULL)
		{
			active_circuit_id = model->active_circuit->id;
		}
		int i;

		i = 0;

		printf("\nLoaded circuits :\n");
		while(i < model->circuits_count)
		{
			if (active_circuit_id == model->circuits[i]->id)
			{
				printf("• Circuit %d (ACTIVE) : \"%s\" (%d Components and %d Links)\n", model->circuits[i]->id, model->circuits[i]->label, model->circuits[i]->component_count, model->circuits[i]->link_count);
			}
			else
			{
				printf("• Circuit %d : \"%s\" (%d Components and %d Links)\n", model->circuits[i]->id, model->circuits[i]->label, model->circuits[i]->component_count, model->circuits[i]->link_count);
			}
			i++;
		}
		return;
		
	}

	printf(MESS_ERROR"Unknown 'list' command parameter : '%s'. Type 'list help' to see available parameters with 'list'.\n", words[1]);
	return;
}

// Command select 
static void command_select(char* words[MAX_COMMAND_WORDS], Model *model)
{
	// If there's no parameter after select 
	if (words[1] == NULL)
	{
		printf(MESS_SYNTAXE"Please type :\n• 'select help' to learn how to use this command.\n");
		return;
	}

	// If the parameter is "help"
	if (strcmp(words[1], "help") == 0)
	{
		printf(MESS_TIP"'select' command  : It changes the active circuit. \nYou can type :\n• 'select circuit circuit_name' to define the active circuit\n• 'select none' to unselect the active circuit.\n");
		return;
	}


	// If the parameter is "circuit"
	if (strcmp(words[1], "circuit") == 0)
	{
		if (words[2] == NULL)
		{
			printf(MESS_SYNTAXE"Please type a valid circuit name 'select circuit circuit_name' ! If you don't know the name of a circuit, you can use 'show all' to show all the circuits or you can use 'list circuits' to list the loaded circuits.\n");
			return;
		}
	
		// Searching the circuit corresponding to the circuit label
		model->active_circuit = get_circuit_by_label(words[2], model);

		if (model->active_circuit != NULL)
		{
			printf(MESS_INFO"The active circuit is now : '%s'\n", model->active_circuit->label);
			return;
		}
		
		//printf(MESS_ERROR"INVALID CIRCUIT NAME ! '%s' is not a valid circuit name. If you don't know the name of a circuit, you can use 'show all' to show all the circuits or you can use 'list circuits' to list the loaded circuits.\n", words[2]);
		return;
	}

	// If the parameter is "none"
	if (strcmp(words[1], "none") == 0)
	{
		if (model->active_circuit != NULL)
		{
			model->active_circuit = NULL;
			printf(MESS_INFO"There's no longer an active circuit.\n");
			return;
		}
		
		printf(MESS_INFO"There's already no active circuits.\n");
		return;
		
	}
	

	printf(MESS_ERROR"Unknown 'select' command parameter : '%s'. Type 'select help' to see available parameters with 'select'.\n", words[1]);
	return;
}


// Command show 
static void	command_show(char* words[MAX_COMMAND_WORDS], Model *model)
{
	// If there's no parameters after show 
	if (words[1] == NULL)
	{
		printf(MESS_SYNTAXE"Please type :\n• 'show help' to learn how to use this command.\n");
		return;
	}

	// If the parameter is "help"
	if (strcmp(words[1], "help") == 0)
	{
		printf(MESS_INFO"'show' command  : It displays all the components of a circuit (or of all the circuits) as a table, containing all components propreties.\n");
		printf("\nYou can type :\n• 'show all' to show all circuits \n• 'show circuit circuit_name' to show a precise circuit.\n• 'show active' to show the active circuit.\n");
		return;
	}

	// If the parameter is "all"
	if (strcmp(words[1], "all") == 0)
	{
		show_components_from_model(model);
		return;
	}

	// If the parameter is "circuit"
	if (strcmp(words[1], "circuit") == 0)
	{
		// If there's no parameter after "show circuit"
		if (words[2] == NULL)
		{
			printf(MESS_ERROR"WRONG SYNTAX ! Please type a circuit name 'show circuit circuit_name' ! If you don't know the name of a circuit, you can use 'show all' to show all circuits or you can use 'list circuits' to list the loaded circuits.\n");
			return;
		}
	
		// Searching the circuit corresponding to the circuit label
		Circuit* circ = get_circuit_by_label(words[2], model);			
		if (circ != NULL)
		{
			show_components_from_circuit(circ);
		}
		return;
	}

	// If the parameter is 'active'
	if (strcmp(words[1], "active") == 0)
	{
		if (model->active_circuit != NULL)
		{
			show_components_from_circuit(model->active_circuit);
			return;
		}
		
		printf(MESS_INFO"There's no active circuits, please use 'select' command to set an active circuit.\n");
		return;
	}


	printf(MESS_ERROR"Unknown 'show' command parameter : '%s'. Type 'show help' to see available parameters with 'show'.\n", words[1]);
	return;
}

// Command quit 
static void	command_quit(char* words[MAX_COMMAND_WORDS], Model *model)
{
	(void)words;

	printf("\nBye !\n");
	printf(MESS_INFO"The run_loop has been stopped by the user.\n");

	model->run_loop = false;
	return;
}


static void exec_command(char* words[MAX_COMMAND_WORDS], Model *model)
{

	// Mapping of the commands with the functions
	static const CommandMap commands[] = {
		{"help", command_help},
		{"list", command_list},
		{"quit", command_quit},
		{"select", command_select},
		{"show", command_show}
	};

	int commands_count = sizeof(commands) / sizeof(commands[0]);

	// Search between user inputs (words) and registered commands
	for (int i = 0; i < commands_count; i++)
	{
		if (strcmp(words[0], commands[i].name) == 0)
		{
			commands[i].function(words, model);
			return; 
		}
	}

	// If the command don't exit
	printf(MESS_INFO"The command you wrote doesn't exit. Please use 'help' command to know the available commands.\n");
	return;
}


static void	scan_user_entry(char* command_user, Model *model)
{
	int i;
	const char * separators = " \n\r\t";
	char* words[MAX_COMMAND_WORDS] = {NULL};
	
	char* strToken = strtok ( command_user, separators );

	i = 0;
	while ( strToken != NULL && i < MAX_COMMAND_WORDS) {
		//printf ("> %s\n", strToken );
		words[i] = strToken;
		strToken = strtok ( NULL, separators );
		i++;
	}

	if (words[0] == NULL) {
		return; 
	}

	exec_command(words, model);
}


void	run_loop(Model *model)
{
	char	user_entry[100];
	
	model->run_loop = true;

	while(model->run_loop)
	{
		printf("\n["APP_NAME" "APP_VERSION"] > ");

		if (fgets(user_entry, sizeof(user_entry), stdin) != NULL) {
			scan_user_entry(user_entry, model);
		}
	}

	return;
}
