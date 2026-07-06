//functions-run-loop.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "prototypes.h"

#define COM_OPEN				TERMINAL_YELLOW		"'"
#define COM_CLOSE				TERMINAL_YELLOW		"'"					TERMINAL_DEFAULT
#define STR_OPEN				TERMINAL_ORANGE		" \""
#define STR_CLOSE				"\""				TERMINAL_DEFAULT
#define STR_PATH				TERMINAL_ORANGE		"\"path/to/file\""	TERMINAL_DEFAULT
#define KEYWORD_ALL				TERMINAL_BLUE		" all"				TERMINAL_YELLOW
#define KEYWORD_ACTIVE			TERMINAL_GREEN		" active"			TERMINAL_YELLOW
#define OPTION(option)			TERMINAL_YELLOW		#option 			TERMINAL_DEFAULT
#define OPTION_COM(option)		TERMINAL_YELLOW		"'"					#option 			"'"		TERMINAL_DEFAULT

// Mapping of the commands names with the linked functions and the required number of words needed when running a command.
CommandMap commands[] = {
	{"circ",		command_circuit,	2},
	{"circuit",		command_circuit,	2},
	{"circuits",	command_circuit,	2},
	{"help",		command_help,		2},
	{"hello",		command_hello,		1},
	{"quit",		command_quit,		1},
	{"exit",		command_quit,		1},
	{"close",		command_quit,		1},
	{"leave",		command_quit,		1},
};

CommandMap circuit_options[] = {
	{"create",		command_circuit_create,		3},
	{"delete",		command_circuit_delete,		3},
	{"rename",		command_circuit_rename,		4},
	{"import",		command_circuit_import,		4},
	{"export",		command_circuit_export,		4},
	{"simulate",	command_circuit_simulate,	3},
	{"show",		command_circuit_show,		3},
	{"select",		command_circuit_select,		3},
	{"unselect",	command_circuit_unselect,	2},
	{"list",		command_circuit_list,		2},
	{"help",		command_circuit_help,		2}
};

// Command help
void command_help(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	(void)model;

	if (word_count == 1)
	{
		printf(	MESS_TIP"You can type "OPTION_COM(help)" after the name of a command to learn how to use it !\n"
				"\nYou can use :"
				"\n• "OPTION_COM(help commands)" to see the list of commands."
				"\n• "OPTION_COM(help components)" to see the list of available components and how they work (not yet implemented)."
				"\n• "OPTION_COM(help links)" to see how the links work (not yet implemented).\n");
		return;
	}
	if (strcmp(words[1], "commands") == 0)
	{
		printf(	MESS_TIP"List of commands :"
				"\n• "OPTION_COM(circuit)"   : Interact with a circuit."
				"\n• "OPTION_COM(component)" : Interact with a component of a circuit. (not yet implemeted)"
				"\n• "OPTION_COM(hello)"     : Displays an \"Hello World !\" message."
				"\n• "OPTION_COM(help)"      : User guide of the app."
				"\n• "OPTION_COM(link)"      : Interact with a link of a circuit. (not yet implemeted)"
				"\n• "OPTION_COM(quit)"      : Close the application properly.\n"
				MESS_TIP"You can write "OPTION_COM(help)" after the name of a command to know how to use it !\n");
		return;
	}

	printf(MESS_ERROR"Unknown help topic '%s'. Type "OPTION_COM(help)" to see all available topics.\n", words[1]);
}

// Command hello 
void	command_hello(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	(void)words;
	(void)word_count;
	(void)model;

	printf(TERMINAL_MAGENTA"\nHello world !\n\n♪(๑ᴖ◡ᴖ๑)♪\n");
	return;
}


// Command quit 
void	command_quit(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	(void)words;
	(void)word_count;

	printf("\nBye !\n");
	printf(MESS_INFO"The run_loop has been stopped by the user.\n");

	model->run_loop = false;
	return;
}

// "circuit create"
void	command_circuit_create(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	(void)word_count;

	// 'circuit create help'
	if (strcmp(words[2], "help") == 0)
	{
		printf( "\n• "OPTION(create)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(create) STR_OPEN"circuit name"STR_CLOSE COM_CLOSE"                         : create an empty circuit.\n");
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

// "circuit delete"
void	command_circuit_delete(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	(void)word_count;

	if (strcmp(words[2], "all") == 0)
	{
		while (model->circuits_count > 0)
		{
			delete_circuit(model, model->circuits[0]);
		}
		if (model->active_circuit != NULL)
		{
			model->active_circuit = NULL;
		}
		return;
	}

	// 'circuit delete help'
	if (strcmp(words[2], "help") == 0)
	{
		printf( "\n• "OPTION(delete)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(delete) STR_OPEN"circuit name"STR_CLOSE COM_CLOSE"                         : delete a circuit."
				"\n  ▻ "COM_OPEN"circuit "OPTION(delete) KEYWORD_ACTIVE COM_CLOSE"                                 : delete the active circuit."
				"\n  ▻ "COM_OPEN"circuit "OPTION(delete) KEYWORD_ALL COM_CLOSE"                                    : delete all the circuits of the model.\n");
		return;
	}

	delete_circuit(model, get_circuit_by_label(words[2], model));
	return;
}

// or 'circuit rename "old_circuit_name" "new_circuit_name"
void	command_circuit_rename(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	// 'circuit rename help'
	if ((word_count <= 3) && (strcmp(words[2], "help") == 0))
	{
		printf( "\n• "OPTION(rename)" :"
		"\n  ▻ "COM_OPEN"circuit "OPTION(rename) STR_OPEN"old_circuit_name"STR_CLOSE STR_OPEN"new_circuit_name"STR_CLOSE COM_CLOSE"  : rename a circuit."
		"\n  ▻ "COM_OPEN"circuit "OPTION(rename) KEYWORD_ACTIVE STR_OPEN"new_circuit_name"STR_CLOSE COM_CLOSE"              : rename the active circuit.\n");
		return;
	}

	Circuit* circ = get_circuit_by_label(words[2], model);
	if (circ == NULL)
	{
		printf(MESS_ERROR"The circuit name '%s' doesn't exist. Check names with 'circuit list'.\n", words[2]);
		return;
	}
	rename_circuit(model, circ, words[3]);
	return;
}

// 'circuit import all "file/path"'
void	command_circuit_import(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	// 'circuit rename help'
	if (strcmp(words[2], "help") == 0)
	{
		printf( "\n• "OPTION(import)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(import) KEYWORD_ALL" "STR_PATH COM_CLOSE"                     : import all the circuits from a file.\n");
		return;
	}

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
void	command_circuit_export(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	// 'circuit export help'
	if (strcmp(words[2], "help") == 0)
	{
		printf( "\n• "OPTION(export)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(export) KEYWORD_ALL" "STR_PATH COM_CLOSE"                     : export all the loaded circuits to a file.\n");
		return;
	}

	if ((word_count < 4) && (strcmp(words[2], "all") != 0))
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

// "circuit simulate" or "simu"
void	command_circuit_simulate(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	(void)word_count;

	// 'circuit simulate all'
	if (strcmp(words[2], "all") == 0)
	{
		simulate_model(model);
		return;
	}

	// 'circuit simulate help'
	if (strcmp(words[2], "help") == 0)
	{
		printf( "\n• "OPTION(simulate)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(simulate) KEYWORD_ALL COM_CLOSE"                                  : simulate all the components of all loaded circuits."
				"\n  ▻ "COM_OPEN"circuit "OPTION(simulate) KEYWORD_ACTIVE COM_CLOSE"                               : simulate all the components of the active circuit ('select' option)."
				"\n  ▻ "COM_OPEN"circuit "OPTION(simulate) STR_OPEN"circuit name"STR_CLOSE COM_CLOSE"                       : simulate all the components of the given circuit.\n");
		return;
	}

	// 'circuit simulate "circuit name"'
	Circuit* circ = get_circuit_by_label(words[2], model);
	if (circ != NULL)
	{
		simulate_circuit(circ);
		return;
	}
	printf(MESS_SYNTAX"The circuit name doesn't exist. Please check the name with "OPTION_COM(circuit list)".\n");
	return;
}

// "circuit show"
void	command_circuit_show(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	(void)word_count;

	// 'circuit show all'
	if (strcmp(words[2], "all") == 0)
	{
		show_components_from_model(model);
		return;
	}

	// 'circuit show help'
	if (strcmp(words[2], "help") == 0)
	{
		printf( "\n• "OPTION(show)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(show) KEYWORD_ALL COM_CLOSE"                                      : show all the components of all loaded circuits."
				"\n  ▻ "COM_OPEN"circuit "OPTION(show) KEYWORD_ACTIVE COM_CLOSE"                                   : show all the components of the active circuit ('select' option)."
				"\n  ▻ "COM_OPEN"circuit "OPTION(show) STR_OPEN"circuit name"STR_CLOSE COM_CLOSE"                           : show all the components of the given circuit.\n");
		return;
	}

	// 'circuit show "circuit name"'
	Circuit* circ = get_circuit_by_label(words[2], model);
	if (circ != NULL)
	{
		show_components_from_circuit(circ);
		return;
	}
	printf(MESS_SYNTAX"The circuit name doesn't exist. Please check the name with "OPTION_COM(circuit list)".\n");
	return;
}

// "circuit select"
void	command_circuit_select(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	(void)word_count;

	// 'circuit select help'
	if (strcmp(words[2], "help") == 0)
	{
		printf( "\n• "OPTION(select)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(select) STR_OPEN"circuit_name"STR_CLOSE COM_CLOSE"                         : define the active circuit.\n");
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

// "circuit unselect"
void	command_circuit_unselect(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	(void)word_count;
	// 'circuit unselect help'
	if ((word_count == 3) && (strcmp(words[2], "help") == 0))
	{
		printf(	"\n• "OPTION(unselect)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(unselect) COM_CLOSE"                                      : unselect the active circuit.\n");
		return;
	}

	if (model->active_circuit != NULL)
	{
		printf(MESS_INFO"The circuit '%s' is no longer activated.\n", model->active_circuit->label);
		model->active_circuit = NULL;
		return;
	}
	
	printf(MESS_INFO"There's no already active circuits.\n");
	return;
}

// "circuit list"
void	command_circuit_list(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	if ((word_count == 3) && (strcmp(words[2], "help") == 0))
	{
		printf( "\n• "OPTION(list)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(list) COM_CLOSE"                                          : display a list of all the loaded circuits.\n");
		return;
	}

	list_loaded_circuits(model);
	return;
}

// 'circuit help'
void	command_circuit_help(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	(void)words;
	(void)word_count;

	printf(MESS_INFO""OPTION_COM(circuit)" command : Use it to manage loaded circuits.\n\nYou have plenty of options :\n");
	
	int i;
	
	i = 0;
	while (i < 11-1)
	{
		exec_command((char*[]){"circuit", circuit_options[i].command, "help"}, model, 3);
		i++;
	}
	
	printf(MESS_TIP"After you've set up an active circuit (with "COM_OPEN"circuit select"COM_CLOSE" command), you can modify the content of it with "COM_OPEN"component"COM_CLOSE" and "COM_OPEN"link"COM_CLOSE" commands.\n");
	return;
}

// Command circuit 
void	command_circuit(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	(void)word_count;
	int i;
	
	// If there's no options after circuit 
	if (word_count < 2)
	{
		printf(MESS_SYNTAX"Please type "OPTION_COM(circuit help)" to learn how to use this command.\n");
		return;
	}
	
	if (word_count >= 3)
	{
		if(strcmp(words[2], "active") == 0)
		{
			if (model->active_circuit != NULL)
			{
				strcpy(words[2], model->active_circuit->label);
			}
			else
			{
				printf(MESS_INFO"There's no active circuits, please use "OPTION_COM(circuit select)" command to set an active circuit.\n");
				return;
			}
		}
	}

	i = 0;
	while (i < 11)
	{
		if (strcmp(words[1], circuit_options[i].command) == 0)
		{
			//If there is not enough word and it's not an help command : we display an error
			if ((word_count < circuit_options[i].needed_words) && !((word_count >= 3) && (strcmp("help", words[2]) == 0)))
			{
				printf(MESS_SYNTAX"The command you wrote is invalid, please check the available formats for this command with : "COM_OPEN "circuit %s" COM_CLOSE "\n", circuit_options[i].command);
				return;
			}
			circuit_options[i].function(words, model, word_count);
			return;
		}
		i++;
	}

	printf(MESS_ERROR"Unknown "OPTION_COM(help)" command option : '%s'. Type "OPTION_COM(circuit help)" to see available options with "OPTION_COM(circuit)" command.\n", words[1]);
	return;
}


void exec_command(char* words[MAX_COMMAND_WORDS], Model *model, int word_count)
{
	int commands_count = sizeof(commands) / sizeof(commands[0]);

	// Search between user inputs (words) and registered commands
	for (int i = 0; i < commands_count; i++)
	{
		if (strcmp(words[0], commands[i].command) == 0)
		{
			commands[i].function(words, model, word_count);
			return; 
		}
	}

	// If the command don't exit
	printf(MESS_INFO"The command you wrote doesn't exit. Please use 'help' command to know the available commands.\n");
	return;
}