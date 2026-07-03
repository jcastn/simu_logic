//functions-run-loop.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"
#include "prototypes.h"


// Command help
static void command_help(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	(void)model;

	if (word_count == 1)
	{
		printf(	MESS_TIP"You can type 'help' after the name of a command to learn how to use it !\n"
				"\nYou can use :"
				"\n• 'help commands' to see the list of commands."
				"\n• 'help components' to see the list of available components and how they work (not yet implemented)."
				"\n• 'help links' to see how the links work (not yet implemented).\n");
		return;
	}
	if (strcmp(words[1], "commands") == 0)
	{
		printf(	MESS_TIP"List of commands :"
				"\n• 'circuit' : Interact with a circuit (more details in 'circuit help')."
				"\n• 'help'    : User guide of the app"
				"\n• 'quit'    : Close the application properly.\n");
		return;
	}

	printf(MESS_ERROR"Unknown help topic '%s'. Type 'help' to see all available topics.\n", words[1]);
}


// Command quit 
static void	command_quit(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	(void)words;
	(void)word_count;

	printf("\nBye !\n");
	printf(MESS_INFO"The run_loop has been stopped by the user.\n");

	model->run_loop = false;
	return;
}

// Command circuit 
static void	command_circuit(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	(void)word_count;
	
	// If there's no options after circuit 
	if (word_count < 2)
	{
		printf(MESS_SYNTAX"Please type 'circuit help' to learn how to use this command.\n");
		return;
	}

	// If the option is "help"
	if (strcmp(words[1], "help") == 0)
	{
		printf(MESS_INFO"'circuit' command  : It allows to manage a circuit.\n"
			"\nYou can type :"
			"\n• 'import' / 'export' option :"
			"\n  ▻ 'circuit import all \"path/from/simu_logic/folder\"'     : import all the circuits from a file."
			"\n  ▻ 'circuit export all \"path/from/simu_logic/folder\"'     : export all the loaded circuits to a file.\n"
			"\n• 'select' / 'unselect' option :"
			"\n  ▻ 'circuit select \"circuit_name\"'.                        : define the active circuit."
			"\n  ▻ 'circuit unselect'                                      : unselect the active circuit.\n"
			"\n• 'create' / 'delete' option :"
			"\n  ▻ 'circuit create \"circuit name\"'                       : create an empty circuit.\n"
			"\n  ▻ 'circuit delete \"circuit name\"'                       : delete a circuit.\n"
			"\n• 'rename' option :"
			"\n  ▻ 'circuit rename \"old_circuit_name\" \"new_circuit_name\"'  : rename a circuit."
			"\n  ▻ 'circuit rename active \"new_circuit_name\"'              : rename the active circuit.\n"
			"\n• 'list' option :"
			"\n  ▻ 'circuit list' :                                        : display a list of all the loaded circuits.\n"
			"\n• 'show' option :"
			"\n  ▻ 'circuit show all'                                      : show all the components of all loaded circuits."
			"\n  ▻ 'circuit show active'                                   : show all the components of the active circuit ('select' option)."
			"\n  ▻ 'circuit show \"circuit name\"                            : show all the components of the given circuit.\n"
			"\n• 'simulate' option :"
			"\n  ▻ 'circuit simulate all'                                  : simulate all the components of all loaded circuits."
			"\n  ▻ 'circuit simulate active'                               : simulate all the components of the active circuit ('select' option)."
			"\n  ▻ 'circuit simulate \"circuit name\"                        : simulate all the components of the given circuit.");
	
		printf(MESS_TIP"About paths :\nYou can write file path from \"simu-logic\" folder by writing it between quotes or write \"IDK\" to open it with a popup from the File Explorer.\nDon't hesitate to use 'help path' to get more details about how to use file paths.\n");
		return;
	}

	// 'circuit import all "file/path"'
	if ((strcmp(words[1], "import") == 0))
	{
		if (word_count < 4 || strcmp(words[2], "all") != 0)
		{
			printf(	MESS_SYNTAX"Expected: 'circuit import all \"path\"' or 'circuit import all IDK'\n"
					MESS_TIP"Don't hesitate to refer to 'circuit help' command !");
			return;
		}

		if (strcmp(words[3], "IDK") == 0)
		{
			file_process(NULL, IMPORT, model);
		}
		else 
		{
			file_process(words[3], IMPORT, model);
		}

		return;
	}

	// 'circuit export all "file/path'
	if (strcmp(words[1], "export") == 0)
	{
		if (word_count < 4 || strcmp(words[2], "all") != 0)
		{
			printf(	MESS_SYNTAX"Expected: 'circuit export all \"path\"' or 'circuit export all IDK'\n"
					MESS_TIP"Don't hesitate to refer to 'circuit help' !");
			return;
		}

		if (strcmp(words[3], "IDK") == 0)
		{
			file_process(NULL, EXPORT, model);
		}
		else 
		{
			file_process(words[3], EXPORT, model);
		}
		
		return;
	}

	// 'circuit rename active "new_circuit_name"'
	// or 'circuit rename "old_circuit_name" "new_circuit_name"
	if (strcmp(words[1], "rename") == 0)
	{
		if (word_count < 4)
		{
			printf(	MESS_SYNTAX"Please type valid options for the command 'circuit rename'. Syntax: 'circuit rename active \"new_name\"' or 'circuit rename \"old circuit name\" \"new circuit name\"'\n"
					MESS_TIP"Don't hesitate to refer to 'circuit help' !");
			return;
		}

		if (strcmp(words[2], "active") == 0)
		{
			if (model->active_circuit == NULL){
				printf(MESS_ERROR"There's no active circuit. Add one with 'circuit select \"circuit name\"'.\n");
				return;
			}
			rename_circuit(model, model->active_circuit, words[3]);
		}
		else 
		{
			Circuit* circ = get_circuit_by_label(words[2], model);
			if (circ == NULL)
			{
				printf(MESS_ERROR"The circuit name '%s' doesn't exist. Check names with 'circuit list'.\n", words[2]);
				return;
			}
			rename_circuit(model, circ, words[3]);
		}
		return;
	}

	// 1st option "select"
	if (strcmp(words[1], "select") == 0)
	{
		// check if 2nd option don't exist
		if (word_count < 3)
		{
			printf(	MESS_SYNTAX"Please type a valid circuit name 'circuit select \"circuit_name\"' !\n"
					MESS_TIP"If you don't know the name of a circuit, you can use :"
					"\n• 'circuit show all' to see all the loaded circuits names"
					"\n• 'circuit list' to list the loaded circuits.\n");
			return;
		}
	
		// 2nd option "circuit name" Searching the circuit corresponding to the circuit label
		model->active_circuit = get_circuit_by_label(words[2], model);

		if (model->active_circuit != NULL)
		{
			printf(MESS_INFO"The active circuit is now : '%s'\n", model->active_circuit->label);
			return;
		}
		return;
	}

	// 1st option "unselect"
	if (strcmp(words[1], "unselect") == 0)
	{
		if (model->active_circuit != NULL)
		{
			printf(MESS_INFO"The circuit '%s' is no longer activated.\n", model->active_circuit->label);
			model->active_circuit = NULL;
			return;
		}
		
		printf(MESS_INFO"There's no already active circuits.\n");
		return;
	}

	// 1st option "list"
	if (strcmp(words[1], "list") == 0)
	{
		list_loaded_circuits(model);
		return;
	}


	// 1st option "show"
	if (strcmp(words[1], "show") == 0)
	{
		if (word_count < 3 )
		{
			printf(	MESS_SYNTAX"Please type a valid option after 'circuit show' : 'all', 'active' or '\"circuit name\"'.\n"
					MESS_TIP "Refer to 'circuits show help' for more details."
					MESS_TIP "If you don't know the name of a circuit, you can use 'circuits list' to list the loaded circuits.\n");
			return;

		}
		// 'circuit show all'
		if (strcmp(words[2], "all") == 0)
		{
			show_components_from_model(model);
			return;
		}

		// 'circuit show active'
		if (strcmp(words[2], "active") == 0)
		{
			if (model->active_circuit == NULL)
			{
				printf(MESS_INFO"There's no active circuits, please use 'select' command to set an active circuit.\n");
				return;
			}
			show_components_from_circuit(model->active_circuit);
			return;
		}

		// 'circuit show help'
		if (strcmp(words[2], "help") == 0)
		{
			printf( MESS_INFO"'circuit show' command  : It allows to show the components of a circuit.\n"
					"\nYou can type :"
					"\n▻ 'circuit show all'           : show all the loaded circuits.\n"
					"\n▻ 'circuit show active'        : show the active circuit.\n"
					"\n▻ 'circuit show \"circuit name\" : show a given circuit.\n");
			return;
		}

		// 'circuit show "circuit name"'
		Circuit* circ = get_circuit_by_label(words[2], model);
		if (circ != NULL)
		{
			show_components_from_circuit(circ);
			return;
		}
		printf(MESS_SYNTAX"The circuit name doesn't exist. Please check the name with 'circuit list'.\n");
		return;
	}

	// 1st option "simulate" or "simu"
	if ((strcmp(words[1], "simulate") == 0) || (strcmp(words[1], "simu") == 0))
	{
		if (word_count < 3 )
		{
			printf(	MESS_SYNTAX"Please type a valid option after 'circuit simulate' : 'all', 'active' or '\"circuit name\"'.\n"
					MESS_TIP "Refer to 'circuits simulate help' for more details."
					MESS_TIP "If you don't know the name of a circuit, you can use 'circuits list' to list the loaded circuits.\n");
			return;

		}
		// 'circuit simulate all'
		if (strcmp(words[2], "all") == 0)
		{
			simulate_model(model);
			return;
		}

		// 'circuit simulate active'
		if (strcmp(words[2], "active") == 0)
		{
			if (model->active_circuit == NULL)
			{
				printf(MESS_INFO"There's no active circuits, please use 'select' command to set an active circuit.\n");
				return;
			}
			simulate_circuit(model->active_circuit);
			return;
		}

		// 'circuit simulate help'
		if (strcmp(words[2], "help") == 0)
		{
			printf( MESS_INFO"'circuit show' command  : It allows to show the components of a circuit.\n"
					"\nYou can type :"
					"\n▻ 'circuit simulate all'           : simulate all the loaded circuits.\n"
					"\n▻ 'circuit simulate active'        : simulate the active circuit.\n"
					"\n▻ 'circuit simulate \"circuit name\" : simulate a given circuit.\n");
			return;
		}

		// 'circuit simulate "circuit name"'
		Circuit* circ = get_circuit_by_label(words[2], model);
		if (circ != NULL)
		{
			simulate_circuit(circ);
			return;
		}
		printf(MESS_SYNTAX"The circuit name doesn't exist. Please check the name with 'circuit list'.\n");
		return;
	}


	
	// 1st option "create"
	if (strcmp(words[1], "create") == 0)
	{
		// check if 2nd option don't exist
		if (word_count < 2)
		{
			printf(	MESS_SYNTAX"Please type a valid circuit name 'circuit create \"circuit_name\"' !\n");
			return;
		}
	
		// 2nd option "circuit create"
		if ((words[2] == NULL) || (strcmp(words[2], "default") == 0))
		{
			create_circuit(model, "NULL");
		}
		else
		{
			create_circuit(model, words[2]);
		}

		if (model->active_circuit != NULL)
		{
			printf(MESS_INFO"The active circuit is now : '%s'\n", model->active_circuit->label);
			return;
		}
		return;
	}
	
	// 1st option "delete"
	if (strcmp(words[1], "delete") == 0)
	{
		// check if 2nd option don't exist
		if (word_count < 3)
		{
			printf(MESS_SYNTAX"Please type a valid circuit name 'circuit create \"circuit_name\"' !\n");
			return;
		}

		if (strcmp(words[2], "active") == 0)
		{
			delete_circuit(model->active_circuit);
			model->active_circuit = NULL;
			return;
		}
	
		delete_circuit(get_circuit_by_label(words[2], model));
		return;
	}




	printf(MESS_ERROR"Unknown 'circuit' command option : '%s'. Type 'circuit help' to see available options with 'circuit'.\n", words[1]);
	return;
}


void exec_command(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{

	// Mapping of the commands with the functions
	static const CommandMap commands[] = {
		{"circ",		command_circuit},
		{"circuit",		command_circuit},
		{"circuits",	command_circuit},
		{"help",		command_help},
		{"quit",		command_quit},
		{"exit",		command_quit},
		{"close",		command_quit},
		{"leave",		command_quit},
	};

	int commands_count = sizeof(commands) / sizeof(commands[0]);

	// Search between user inputs (words) and registered commands
	for (int i = 0; i < commands_count; i++)
	{
		if (strcmp(words[0], commands[i].name) == 0)
		{
			commands[i].function(words, model, word_count);
			return; 
		}
	}

	// If the command don't exit
	printf(MESS_INFO"The command you wrote doesn't exit. Please use 'help' command to know the available commands.\n");
	return;
}
