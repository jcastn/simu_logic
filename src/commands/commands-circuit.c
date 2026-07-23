//commands-circuit.c
#include "../../include/prototypes.h"
#include <stdbool.h>

static void			command_circuit_create		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_delete		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_rename		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_duplicate	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_clear		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_import		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_export		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_rearrange	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_simulate	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_select		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_unselect	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_help		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);

static const CommandMap circuit_options[] = {
	{"create",		command_circuit_create,		3,	false},
	{"cre",			command_circuit_create,		3,	true},
	{"delete",		command_circuit_delete,		3,	false},
	{"del",			command_circuit_delete,		3,	true},
	{"clear",		command_circuit_clear,		3,	false},
	{"cl",			command_circuit_clear,		3,	true},
	{"rename",		command_circuit_rename,		4,	false},
	{"ren",			command_circuit_rename,		4,	true},
	{"duplicate",	command_circuit_duplicate,	3,	false},
	{"dup",			command_circuit_duplicate,	3,	true},
	{"import",		command_circuit_import,		4,	false},
	{"im",			command_circuit_import,		4,	true},
	{"export",		command_circuit_export,		4,	false},
	{"ex",			command_circuit_export,		4,	true},
	{"rearrange",	command_circuit_rearrange,	3,	false},
	{"rea",		command_circuit_rearrange,	3,	true},
	{"select",		command_circuit_select,		3,	false},
	{"sel",		command_circuit_select,		3,	true},
	{"unselect",	command_circuit_unselect,	2,	false},
	{"unsel",		command_circuit_unselect,	2,	true},
	{"simulate",	command_circuit_simulate,	3,	false},
	{"simu",		command_circuit_simulate,	3,	true},
	{"help",		command_circuit_help,		2,	true}
};

// "circuit create"
static void	command_circuit_create(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)arg_count;

	// 'circuit create help'
	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(create)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(create) OPTION_CIRCUIT(circuit name) COM_CLOSE"                         : Create an empty circuit.\n");
		return;
	}

	// 2nd option "circuit create"
	if (strcmp(args[2], "default") == 0)
	{
		create_circuit(model, "default");
	}
	else
	{
		create_circuit(model, args[2]);
	}
	
	return;
}

// "circuit delete"
static void	command_circuit_delete(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)arg_count;

	if (strcmp(args[2], "all") == 0)
	{
		delete_model(model, false);
		
		if (model->active_circuit != NULL)
		{
			model->active_circuit = NULL;
		}
		return;
	}

	// 'circuit delete help'
	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(delete)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(delete) KEYWORD_ALL COM_CLOSE"                                    : Delete all the circuits of the model."
				"\n  ▻ "COM_OPEN"circuit "OPTION(delete) KEYWORD_ACTIVE COM_CLOSE"                                 : Delete the active circuit."
				"\n  ▻ "COM_OPEN"circuit "OPTION(delete) OPTION_CIRCUIT(circuit name) COM_CLOSE"                         : Delete a circuit.\n");
		return;
	}
	// 'circuit delete "circuit name"' 
	Circuit* circ = get_circuit_by_label(model, args[2]);
	if (circ == NULL)
	{
		return;
	}
	if (delete_circuit(model, circ, true))
	{
		printf("\n"MESS_CIRC"Circuit deleted\n");
	}
	return;
}

// 'circuit clear'
static void			command_circuit_clear(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)arg_count;
	int counter;

	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(clear)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(clear) KEYWORD_ALL COM_CLOSE"                                     : Clear all circuits."
				"\n  ▻ "COM_OPEN"circuit "OPTION(clear) KEYWORD_ACTIVE COM_CLOSE"                                  : Clear the active circuit."
				"\n  ▻ "COM_OPEN"circuit "OPTION(clear) OPTION_CIRCUIT(circuit name) COM_CLOSE"                          : Clear a precise circuit.\n");
		return;
	}

	// 'circuit clear all'
	if (strcmp(args[2], "all") == 0)
	{
		counter = 0;
		while (counter < model->circuits_count)
		{
			delete_circuit(model, model->circuits[counter], false);
			counter++;
		}
		printf("\n"MESS_CIRC"All loaded circuits are cleared.\n");
		return;
	}

	// 'circuit clear "circuit name"' 
	Circuit* circ = get_circuit_by_label(model, args[2]);
	if (circ == NULL)
	{
		return;
	}
	if (delete_circuit(model, circ, false))
	{
		printf("\n"MESS_CIRC"Circuit '%s' cleared !\n", circ->label);
	}
	return;

}

// 'circuit rename "old_circuit_name" "new_circuit_name"
static void	command_circuit_rename(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	// 'circuit rename help'
	if ((arg_count <= 3) && (strcmp(args[2], "help") == 0))
	{
		printf( "\n• "OPTION(rename)" :"
		"\n  ▻ "COM_OPEN"circuit "OPTION(rename) KEYWORD_ACTIVE OPTION_CIRCUIT(new circuit name) COM_CLOSE"              : Rename the active circuit."
		"\n  ▻ "COM_OPEN"circuit "OPTION(rename) OPTION_CIRCUIT(old circuit name) OPTION_CIRCUIT(new circuit name) COM_CLOSE"  : Rename a circuit.\n");
		return;
	}

	Circuit* circ = get_circuit_by_label(model, args[2]);
	if (circ == NULL)
	{
		return;
	}
	rename_circuit(model, circ, args[3]);
	return;
}

// 'circuit duplicate "circuit name" "new_circuit_name"'
static void			command_circuit_duplicate(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)arg_count;
	//Circuit* dest_circ = NULL;

	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(duplicate)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(duplicate) KEYWORD_ACTIVE OPTION_CIRCUIT(new circuit name) COM_CLOSE"           : Duplicate the active circuit."
				"\n  ▻ "COM_OPEN"circuit "OPTION(duplicate) OPTION_CIRCUIT(circuit name) OPTION_CIRCUIT(new circuit name) COM_CLOSE"   : Duplicate a circuit.\n");
		return;
	}

	Circuit* src_circ = get_circuit_by_label(model, args[2]);
	if (src_circ == NULL)
	{
		return;
	}

	if (arg_count == 4)
	{
		Circuit* dest_circ = duplicate_circuit(model, src_circ, args[3]);
		printf(MESS_INFO"Circuit '%s' has been duplicated as '%s'\n", src_circ->label, dest_circ->label);
	}
	else
	{
		Circuit* dest_circ = duplicate_circuit(model, src_circ, "default");
		printf(MESS_INFO"Circuit '%s' has been duplicated as '%s'\n", src_circ->label, dest_circ->label);
	}
}

// 'circuit import all "file/path"'
static void	command_circuit_import(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	// 'circuit import help'
	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(import)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(import) KEYWORD_ALL KEYWORD_PATH COM_CLOSE"                     : Import all the circuits from a file.\n");
		return;
	}

	if (arg_count < 4 || strcmp(args[2], "all") != 0)
	{
		printf(	MESS_SYNTAX"Expected: 'circuit import all "KEYWORD_PATH" or 'circuit import all IDK'\n"
				MESS_TIP"Don't hesitate to refer to 'circuit help' command !");
		return;
	}

	if (strcmp(args[3], "IDK") == 0)
	{
		file_process(NULL, IMPORT, model, -1);
	}
	else 
	{
		file_process(args[3], IMPORT, model, -1);
	}

	return;
}

// 'circuit export all "file/path'
static void	command_circuit_export(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	int circ_number;
	// 'circuit export help'
	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(export)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(export) KEYWORD_ALL KEYWORD_PATH COM_CLOSE"                     : Export all the loaded circuits to a file."
				"\n  ▻ "COM_OPEN"circuit "OPTION(export) KEYWORD_ACTIVE KEYWORD_PATH COM_CLOSE"                  : Export the active circuit to a file."
				"\n  ▻ "COM_OPEN"circuit "OPTION(export) OPTION_CIRCUIT(circuit name) KEYWORD_PATH COM_CLOSE"          : Export a circuit to a file.\n");

		return;
	}

	if ((strcmp(args[2], "all") != 0))
	{
		if (arg_count < 4)
		{
			printf(	MESS_SYNTAX"Expected: 'circuit export all "KEYWORD_PATH"' or 'circuit export all IDK'\n"
					MESS_TIP"Don't hesitate to refer to 'circuit help' !");
			return;
		}
		else
		{
			Circuit *circ = get_circuit_by_label(model, args[2]);
			if (circ == NULL)
			{
				return;
			}
			circ_number = get_circuit_number_in_model(model, circ);
		}

	}
	else {
		circ_number = -1;
	}

	// if filepath is IDK 
	if (strcmp(args[3], "IDK") == 0)
	{
		file_process(NULL, EXPORT, model, circ_number);
	}
	else 
	{
		file_process(args[3], EXPORT, model, circ_number);
	}
	
	return;
}

// 'circuit rearrange "circuit name"'
static void			command_circuit_rearrange(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)arg_count;

	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(rearrange)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(rearrange) KEYWORD_ALL COM_CLOSE"                                 : Rearrange all circuits."
				"\n  ▻ "COM_OPEN"circuit "OPTION(rearrange) KEYWORD_ACTIVE COM_CLOSE"                              : Rearrange the active circuit."
				"\n  ▻ "COM_OPEN"circuit "OPTION(rearrange) OPTION_CIRCUIT(circuit name) COM_CLOSE"                      : Rearrange a circuit.\n");
		return;
	}

	if ((strcmp(args[2], "all") == 0))
	{
		int counter;

		counter = 0;
		while (counter < model->circuits_count)
		{
			rearrange_circuit(model->circuits[counter]);
			counter++;
		}
		printf(MESS_INFO"All loaded circuits are rearranged !\n");
	}
	else
	{
		Circuit* circ = get_circuit_by_label(model, args[2]);
		if (circ != NULL)
		{
			rearrange_circuit(circ);
			printf(MESS_INFO"The circuit '%s' is rearranged !\n", circ->label);
		}
		return;
	}

}

// "circuit simulate"
static void	command_circuit_simulate(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)arg_count;

	// 'circuit simulate all'
	if (strcmp(args[2], "all") == 0)
	{
		simulate_model(model);
		printf(MESS_INFO"All circuits simulated !\n");
		return;
	}

	// 'circuit simulate help'
	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(simulate)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(simulate) KEYWORD_ALL COM_CLOSE"                                  : Simulate all the components of all loaded circuits."
				"\n  ▻ "COM_OPEN"circuit "OPTION(simulate) KEYWORD_ACTIVE COM_CLOSE"                               : Simulate all the components of the active circuit ('select' option)."
				"\n  ▻ "COM_OPEN"circuit "OPTION(simulate) OPTION_CIRCUIT(circuit name) COM_CLOSE"                       : Simulate all the components of the given circuit.\n");
		return;
	}

	// 'circuit simulate "circuit name"'
	Circuit* circ = get_circuit_by_label(model, args[2]);
	if (circ != NULL)
	{
		simulate_circuit(circ);
		printf(MESS_INFO"Circuit \"%s\" simulated !\n", args[2]);
	}
	return;
}

// "circuit select"
static void	command_circuit_select(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)arg_count;

	// 'circuit select help'
	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(select)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(select) OPTION_CIRCUIT(circuit name) COM_CLOSE"                         : Define the active circuit.\n");
		return;
	}

	// 2nd option "circuit name" Searching the circuit corresponding to the circuit label
	model->active_circuit = get_circuit_by_label(model, args[2]);
	if (model->active_circuit != NULL)
	{
		printf(MESS_INFO"The active circuit is now : \"%s\"\n", model->active_circuit->label);
		return;
	}
	return;
}

// "circuit unselect"
static void	command_circuit_unselect(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	// 'circuit unselect help'
	if ((arg_count == 3) && (strcmp(args[2], "help") == 0))
	{
		printf(	"\n• "OPTION(unselect)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(unselect) COM_CLOSE"                                      : Unselect the active circuit.\n");
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


// 'circuit help'
static void	command_circuit_help(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)args;
	(void)arg_count;
	int circuit_options_count = sizeof(circuit_options) / sizeof(circuit_options[0]);


	printf(MESS_INFO""OPTION_COM(circuit)" command : Use it to manage loaded circuits.\n\nYou have plenty of options :\n");
	
	exec_full_help("circuit", circuit_options, circuit_options_count, model);

	printf(MESS_TIP"After you've set an "TERMINAL_GREEN"active"TERMINAL_DEFAULT" circuit (with "COM_OPEN"circuit select"COM_CLOSE" command), you will be able to edit the content of it with "COM_OPEN"component"COM_CLOSE" and "COM_OPEN"link"COM_CLOSE" commands.\n");
	return;
}

// Command circuit 
void	command_circuit(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	int		counter;
	int		circuit_options_count;
	
	// If there's the "active" keyword in the command and there's an active cirucit
	// it will edit the args[2] value to the circuit name of the active circuit
	replace_active_keyword(model, args, arg_count);

	counter = 0;
	circuit_options_count = sizeof(circuit_options) / sizeof(circuit_options[0]);

	while (counter < circuit_options_count)
	{
		if (strcmp(args[1], circuit_options[counter].command) == 0)
		{
			//If there is not enough args and it's not an help command : we display an error
			if ((arg_count < circuit_options[counter].needed_args) && !((arg_count >= 3) && (strcmp("help", args[2]) == 0)))
			{
				printf(MESS_SYNTAX"The command you wrote is invalid, please check the available formats for this command with : "COM_OPEN "circuit %s help" COM_CLOSE "\n", circuit_options[counter].command);
				return;
			}
			circuit_options[counter].function(args, model, arg_count);
			return;
		}
		counter++;
	}

	printf(MESS_ERROR"Unknown "OPTION_COM(help)" command option : '%s'. Type "OPTION_COM(circuit help)" to see available options with "OPTION_COM(circuit)" command.\n", args[1]);
	return;
}

