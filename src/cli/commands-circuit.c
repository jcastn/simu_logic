// src/cli/commands-circuit.c
#include "../../include/prototypes-core.h"
#include "../../include/prototypes-cli.h"

// "circuit create"
void	command_circuit_create(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)arg_count;

	// 'circuit create help'
	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(create)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(create) OPTION_CIRCUIT(circuit name) COM_CLOSE"                         : Create an empty circuit.\n");
		return;
	}

	Circuit* circ = create_circuit(model, args[2]);
	if (!circ)
	{
		printf(MESS_ERROR"Circuit not created.");
		return;
	}
	
	printf(MESS_CIRC"Circuit created : "TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT, circ->label);
	return;
}

// "circuit delete"
void	command_circuit_delete(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
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
	if (!circ)
	{
		printf(MESS_CIRC"Circuit not find.");
		return;
	}

	if (delete_circuit(model, circ, true))
	{
		printf(MESS_CIRC"Circuit '%s' deleted", args[2]);
	}
	return;
}

// 'circuit clear'
void			command_circuit_clear(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
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
		printf(MESS_CIRC"All loaded circuits are cleared.");
		return;
	}

	// 'circuit clear "circuit name"' 
	Circuit* circ = get_circuit_by_label(model, args[2]);
	if (!circ)
	{
		printf(MESS_ERROR"Circuit '%s' not found", args[2]);
		return;
	}
	if (delete_circuit(model, circ, false))
	{
		printf(MESS_CIRC"Circuit "TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT" cleared !", circ->label);
	}
	return;

}

// 'circuit rename "old_circuit_name" "new_circuit_name"
void	command_circuit_rename(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
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
	if (!circ)
	{
		printf(MESS_ERROR"Circuit '%s' not found", args[2]);
		return;
	}
	
	if (rename_circuit(model, circ, args[3]))
	{
		printf(MESS_CIRC"Circuit renamed : "TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT, circ->label);
	}
	else
	{
		if (strcmp(circ->label, args[3]) == 0)
		{
			printf(MESS_ERROR"The circuit is already named "TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT". Rename operation is aborted.", args[3]);
		}
		else if (check_circuit_label(model, args[3]) == false)
		{
			printf(MESS_ERROR"A circuit is already named "TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT". Rename operation of circuit "TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT" (id:%d) is aborted.", args[3], circ->label, circ->id);
		}
		else
		{
			printf(MESS_ERROR "Model, circuit or new label not found.");
		}
	}
	return;
}

// 'circuit duplicate "circuit name" "new_circuit_name"'
void			command_circuit_duplicate(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
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
	if (!src_circ)
	{
		printf(MESS_ERROR"Circuit '%s' not found", args[2]);
		return;
	}

	if (arg_count == 4)
	{
		Circuit* dest_circ = duplicate_circuit(model, src_circ, args[3]);
		printf(MESS_INFO"Circuit "TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT" has been duplicated as "TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT, src_circ->label, dest_circ->label);
	}
	else
	{
		Circuit* dest_circ = duplicate_circuit(model, src_circ, "default");
		printf(MESS_INFO"Circuit "TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT" has been duplicated as "TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT, src_circ->label, dest_circ->label);
	}
}

// 'circuit import all "file/path"'
void	command_circuit_import(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
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
		printf(	MESS_SYNTAX"Expected: "COM_OPEN"circuit import all "KEYWORD_PATH COM_CLOSE" or "OPTION_COM(circuit import all IDK)
				MESS_TIP"Don't hesitate to refer to "OPTION_COM(circuit help)" command !");
		return;
	}

	if (strcmp(args[3], "IDK") == 0)
	{
		file_process(NULL, IMPORT, model, -1, NULL);
	}
	else 
	{
		file_process(args[3], IMPORT, model, -1, NULL);
	}

	return;
}

// 'circuit export all "file/path'
void	command_circuit_export(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
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
			printf(	MESS_SYNTAX"Expected: 'circuit export all "KEYWORD_PATH"' or 'circuit export all IDK'"
					MESS_TIP"Don't hesitate to refer to 'circuit help' !");
			return;
		}
		else
		{
			Circuit *circ = get_circuit_by_label(model, args[2]);
			if (!circ)
			{
				printf(MESS_ERROR"Circuit '%s' not found", args[2]);
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
		file_process(NULL, EXPORT, model, circ_number, NULL);
	}
	else 
	{
		file_process(args[3], EXPORT, model, circ_number, NULL);
	}
	
	return;
}

// 'circuit rearrange "circuit name"'
void			command_circuit_rearrange(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
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
			rearrange_circuit(model->circuits[counter], true);
			counter++;
		}
		printf(MESS_INFO"All loaded circuits are rearranged !");
	}
	else
	{
		Circuit* circ = get_circuit_by_label(model, args[2]);
		if (circ != NULL)
		{
			rearrange_circuit(circ, true);
			printf(MESS_INFO"The circuit "TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT" is rearranged !", circ->label);
		}
		return;
	}

}

// "circuit simulate"
void	command_circuit_simulate(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
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
	if (!circ)
	{
		printf(MESS_ERROR"Circuit '%s' not found", args[2]);
	}
	else
	{
		simulate_circuit(circ);
		printf(MESS_INFO"Circuit "TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT" simulated !", args[2]);
	}
	return;
}

// "circuit select"
void	command_circuit_select(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
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
	if (!model->active_circuit)
	{
		printf(MESS_ERROR"Circuit '%s' not found", args[2]);
	}
	else
	{
		printf(MESS_INFO"The active circuit is now : "TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT, model->active_circuit->label);
	}
	return;
}

// "circuit unselect"
void	command_circuit_unselect(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
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
		printf(MESS_INFO"The circuit "TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT" is no longer activated.", model->active_circuit->label);
		model->active_circuit = NULL;
		return;
	}
	
	printf(MESS_INFO"There's no already active circuits.");
	return;
}

// Command circuit 
void	command_circuit(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	int		counter = 0;
	int		options_count = 0;
	
	
	// If there's the "active" keyword in the command and there's an active cirucit
	// it will edit the args[2] value to the circuit name of the active circuit
	replace_active_keyword(model, args, arg_count);

	const SubCommandMap* circuit_options = get_sub_command_map("circuit", &options_count);
	

	while (counter < options_count)
	{
		if (strcmp(args[1], circuit_options[counter].command) == 0)
		{
			//If there is not enough args and it's not an help command : we display an error
			if ((arg_count < circuit_options[counter].needed_args) && !((arg_count >= 3) && (strcmp("help", args[2]) == 0)))
			{
				printf(MESS_SYNTAX"The command you wrote is invalid, please check the available formats for this command with : "COM_OPEN "circuit %s help" COM_CLOSE, circuit_options[counter].command);
				return;
			}
			circuit_options[counter].function(args, model, arg_count);
			return;
		}
		counter++;
	}

	printf(MESS_ERROR"Unknown "OPTION_COM(help)" command option : '%s'. Type "OPTION_COM(circuit help)" to see available options with "OPTION_COM(circuit)" command.", args[1]);
	return;
}

