//functions-commands.c
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
#define STR_PATH				TERMINAL_ORANGE		" \"path/to/file\""	TERMINAL_DEFAULT
#define KEYWORD_ALL				TERMINAL_BLUE		" all"				TERMINAL_YELLOW
#define KEYWORD_ACTIVE			TERMINAL_GREEN		" active"			TERMINAL_YELLOW
#define OPTION(option)			TERMINAL_YELLOW		#option 			TERMINAL_DEFAULT
#define OPTION_COM(option)		TERMINAL_YELLOW		" '"					#option 			"'"					TERMINAL_DEFAULT
#define OPTION_INT(option)		TERMINAL_MAGENTA	" " 				#option 			TERMINAL_DEFAULT
#define OPTION_STR(option)		TERMINAL_ORANGE		" \""				#option				"\""				TERMINAL_DEFAULT

// Mapping of the commands with the functions

static void			command_circuit(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_component(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_help(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_hello(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_link(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_quit(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);

// Mapping of the commands names with the linked functions and the required number of args needed when running a command.
//  commnand name,      command function, needed args,  is_alias
static const CommandMap commands[] = {
	{"circ",		command_circuit,			2,	true},
	{"circuit",		command_circuit,			2,	false},
	{"circuits",	command_circuit,			2,	true},
	{"component",	command_component,			2, 	false},
	{"comp",		command_component,			2, 	true},
	{"help",		command_help,				2,	false},
	{"hello",		command_hello,				1,	false},
	{"link",		command_link,				2, 	false},
	{"quit",		command_quit,				1,	false},
	{"exit",		command_quit,				1,	true},
	{"close",		command_quit,				1,	true},
	{"leave",		command_quit,				1,	true},
};

static void			command_circuit_create(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_delete(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_rename(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_import(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_export(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_simulate(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_show(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_select(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_unselect(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_list(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_circuit_help(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);

static const CommandMap circuit_options[] = {
	{"create",		command_circuit_create,		3,	false},
	{"delete",		command_circuit_delete,		3,	false},
	{"del",			command_circuit_delete,		3,	true},
	{"rename",		command_circuit_rename,		4,	false},
	{"ren",			command_circuit_rename,		4,	true},
	{"import",		command_circuit_import,		4,	false},
	{"im",			command_circuit_import,		4,	true},
	{"export",		command_circuit_export,		4,	false},
	{"ex",			command_circuit_export,		4,	true},
	{"select",		command_circuit_select,		3,	false},
	{"sel",		command_circuit_select,		3,	true},
	{"unselect",	command_circuit_unselect,	2,	false},
	{"unsel",		command_circuit_unselect,	2,	true},
	{"show",		command_circuit_show,		3,	false},
	{"sh",			command_circuit_show,		3,	true},
	{"simulate",	command_circuit_simulate,	3,	false},
	{"simu",		command_circuit_simulate,	3,	true},
	{"list",		command_circuit_list,		2,	false},
	{"help",		command_circuit_help,		2,	true}
};

static void			command_link_create(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_link_delete(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_link_show(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_link_help(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);

static const CommandMap link_options[] = {
	{"create",		command_link_create,		4,	false},
	{"delete",		command_link_delete,		3,	false},
	{"del",			command_link_delete,		3,	true},
	{"show",		command_link_show,			3,	false},
	{"help",		command_link_help,			2,	true}
};


static void			command_component_create(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_component_delete(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_component_rename(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_component_move(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_component_help(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);

static const CommandMap component_options[] = {
	{"create",		command_component_create,		5,	false},
	{"delete",		command_component_delete,		3,	false},
	{"del",			command_component_delete,		3,	true},
	{"rename",		command_component_rename,		4,	false},
	{"ren",			command_component_rename,		4,	true},
	{"move",		command_component_move,			5,	false},
	{"mv",			command_component_move,			5,	true},
	{"help",		command_component_help,			3,	true}
};



static void		exec_full_help(char* command_name, const CommandMap command_map[], int command_count, Model* model)
{
	int counter;
	
	counter = 0;
	while (counter < command_count)
	{
		if (command_map[counter].is_alias == false)
		{
			exec_command((char*[]){command_name, command_map[counter].command, "help"}, model, 3);
		}
		counter++;
	}
	return;
}



// Command help
static void command_help(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)model;

	if (arg_count == 1)
	{
		printf(	MESS_TIP"You can type "OPTION_COM(help)" after the name of a command to learn how to use it !\n"
				"\nYou can use :"
				"\n• "OPTION_COM(help commands)" to see the list of commands."
				"\n• "OPTION_COM(help components)" to see the list of available components and how they work (not yet implemented)."
				"\n• "OPTION_COM(help links)" to see how the links work (not yet implemented).\n");
		return;
	}
	if (strcmp(args[1], "commands") == 0)
	{
		printf(	MESS_TIP"List of commands :"
				"\n• "OPTION_COM(circuit)"   : Interact with a circuit."
				"\n• "OPTION_COM(component)" : Interact with a component of a circuit. (not yet implemeted)"
				"\n• "OPTION_COM(hello)"     : Displays an \"Hello World !\" message."
				"\n• "OPTION_COM(help)"      : User guide of the app."
				"\n• "OPTION_COM(link)"      : Interact with a link of a circuit."
				"\n• "OPTION_COM(quit)"      : Close the application properly.\n"
				MESS_TIP"You can write "OPTION_COM(help)" after the name of a command to know how to use it !\n");
		return;
	}

	printf(MESS_ERROR"Unknown help topic '%s'. Type "OPTION_COM(help)" to see all available topics.\n", args[1]);
}

// Command hello 
static void	command_hello(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)args;
	(void)arg_count;
	(void)model;

	printf(TERMINAL_MAGENTA"\nHello world !\n\n♪(๑ᴖ◡ᴖ๑)♪\n"TERMINAL_DEFAULT);
	return;
}


// Command quit 
static void	command_quit(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)args;
	(void)arg_count;

	printf(MESS_INFO"The run_loop was stopped by the user.\n");

	model->run_loop = false;
	return;
}

// "circuit create"
static void	command_circuit_create(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)arg_count;

	// 'circuit create help'
	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(create)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(create) OPTION_STR(circuit name) COM_CLOSE"                         : create an empty circuit.\n");
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

	if (model->active_circuit != NULL)
	{
		printf(MESS_INFO"The active circuit is now : \"%s\"\n", model->active_circuit->label);
		return;
	}
	return;
}

// "circuit delete"
static void	command_circuit_delete(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)arg_count;

	if (strcmp(args[2], "all") == 0)
	{
		while (model->circuits_count > 0)
		{
			delete_circuit(model, model->circuits[0]);
		}
		printf("\n"MESS_CIRC"All loaded circuits are deleted.\n");
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
				"\n  ▻ "COM_OPEN"circuit "OPTION(delete) OPTION_STR(circuit name) COM_CLOSE"                         : delete a circuit."
				"\n  ▻ "COM_OPEN"circuit "OPTION(delete) KEYWORD_ACTIVE COM_CLOSE"                                 : delete the active circuit."
				"\n  ▻ "COM_OPEN"circuit "OPTION(delete) KEYWORD_ALL COM_CLOSE"                                    : delete all the circuits of the model.\n");
		return;
	}
	// 'circuit delete "circuit name"' 
	Circuit* circ = get_circuit_by_label(args[2], model);
	if (circ == NULL)
	{
		return;
	}
	if (delete_circuit(model, circ))
	{
		printf("\n"MESS_CIRC"Circuit deleted\n");
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
		"\n  ▻ "COM_OPEN"circuit "OPTION(rename) OPTION_STR(old circuit name) OPTION_STR(new circuit name) COM_CLOSE"  : rename a circuit."
		"\n  ▻ "COM_OPEN"circuit "OPTION(rename) KEYWORD_ACTIVE OPTION_STR(new circuit name) COM_CLOSE"              : rename the active circuit.\n");
		return;
	}

	Circuit* circ = get_circuit_by_label(args[2], model);
	if (circ == NULL)
	{
		return;
	}
	rename_circuit(model, circ, args[3]);
	return;
}

// 'circuit import all "file/path"'
static void	command_circuit_import(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	// 'circuit import help'
	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(import)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(import) KEYWORD_ALL STR_PATH COM_CLOSE"                     : import all the circuits from a file.\n");
		return;
	}

	if (arg_count < 4 || strcmp(args[2], "all") != 0)
	{
		printf(	MESS_SYNTAX"Expected: 'circuit import all \"path\"' or 'circuit import all IDK'\n"
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
				"\n  ▻ "COM_OPEN"circuit "OPTION(export) KEYWORD_ALL STR_PATH COM_CLOSE"                     : export all the loaded circuits to a file."
				"\n  ▻ "COM_OPEN"circuit "OPTION(export) KEYWORD_ACTIVE STR_PATH COM_CLOSE"                  : export the active circuit to a file."
				"\n  ▻ "COM_OPEN"circuit "OPTION(export) OPTION_STR(circuit name) STR_PATH COM_CLOSE"          : export a circuit to a file.\n");

		return;
	}

	// Bad options 
	if ((strcmp(args[2], "all") != 0))
	{
		if (arg_count < 4)
		{
			printf(	MESS_SYNTAX"Expected: 'circuit export all \"path\"' or 'circuit export all IDK'\n"
					MESS_TIP"Don't hesitate to refer to 'circuit help' !");
			return;
		}
		else
		{
			Circuit *circ = get_circuit_by_label(args[2], model);
			if (circ == NULL)
			{
				return;
			}
			circ_number = get_circuit_number_in_model(circ, model);
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
				"\n  ▻ "COM_OPEN"circuit "OPTION(simulate) KEYWORD_ALL COM_CLOSE"                                  : simulate all the components of all loaded circuits."
				"\n  ▻ "COM_OPEN"circuit "OPTION(simulate) KEYWORD_ACTIVE COM_CLOSE"                               : simulate all the components of the active circuit ('select' option)."
				"\n  ▻ "COM_OPEN"circuit "OPTION(simulate) OPTION_STR(circuit name) COM_CLOSE"                       : simulate all the components of the given circuit.\n");
		return;
	}

	// 'circuit simulate "circuit name"'
	Circuit* circ = get_circuit_by_label(args[2], model);
	if (circ != NULL)
	{
		simulate_circuit(circ);
		printf(MESS_INFO"Circuit \"%s\" simulated !\n", args[2]);
	}
	return;
}

// "circuit show"
static void	command_circuit_show(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)arg_count;

	// 'circuit show all'
	if (strcmp(args[2], "all") == 0)
	{
		show_components_from_model(model);
		return;
	}

	// 'circuit show help'
	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(show)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(show) KEYWORD_ALL COM_CLOSE"                                      : show all the components of all loaded circuits."
				"\n  ▻ "COM_OPEN"circuit "OPTION(show) KEYWORD_ACTIVE COM_CLOSE"                                   : show all the components of the active circuit ('select' option)."
				"\n  ▻ "COM_OPEN"circuit "OPTION(show) OPTION_STR(circuit name) COM_CLOSE"                           : show all the components of the given circuit.\n");
		return;
	}

	// 'circuit show "circuit name"'
	Circuit* circ = get_circuit_by_label(args[2], model);
	if (circ != NULL)
	{
		show_components_from_circuit(circ);
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
				"\n  ▻ "COM_OPEN"circuit "OPTION(select) OPTION_STR(circuit name) COM_CLOSE"                         : define the active circuit.\n");
		return;
	}

	// 2nd option "circuit name" Searching the circuit corresponding to the circuit label
	model->active_circuit = get_circuit_by_label(args[2], model);
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
static void	command_circuit_list(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	if ((arg_count == 3) && (strcmp(args[2], "help") == 0))
	{
		printf( "\n• "OPTION(list)" :"
				"\n  ▻ "COM_OPEN"circuit "OPTION(list) COM_CLOSE"                                          : display a list of all the loaded circuits.\n");
		return;
	}

	list_loaded_circuits(model);
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

	printf(MESS_TIP"After you've set up an " KEYWORD_ACTIVE " circuit (with "COM_OPEN"circuit select"COM_CLOSE" command), you can modify the content of it with "COM_OPEN"component"COM_CLOSE" and "COM_OPEN"link"COM_CLOSE" commands.\n");
	return;
}

// Command circuit 
static void	command_circuit(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	int counter;
	int circuit_options_count;
	
	// If there's the "active" keyword in the command and there's an active cirucit
	// it will edit the args[2] value to the circuit name of the active circuit
	if (arg_count >= 3)
	{
		if(strcmp(args[2], "active") == 0)
		{
			if (model->active_circuit != NULL)
			{
				strcpy(args[2], model->active_circuit->label);
			}
			else
			{
				printf(MESS_INFO"There's no active circuits, please use "OPTION_COM(circuit select)" command to set an "KEYWORD_ACTIVE" circuit.\n");
				return;
			}
		}
	}

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



// 'link create' 
static void			command_link_create(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	if ((arg_count == 3) && (strcmp(args[2], "help") == 0))
	{
		printf( "\n• "OPTION(create)" :"
				"\n  ▻ "COM_OPEN"link "OPTION(create) OPTION_STR(component source) OPTION_STR(component dest) OPTION_INT(port_number) COM_CLOSE" : create a link from a source component to a destination component and specify\n"
				"                                                          the port number of the destination (from 0 to 10).\n");
		return;
	}

	Component* src = get_component_by_label(args[2], model->active_circuit);
	if (src == NULL)
	{
		return;
	}

	Component* dest = get_component_by_label(args[3], model->active_circuit);
	if (dest == NULL)
	{
		return;
	}

	int port_number = string_to_int(args[4]);

	if ((port_number < 0) || (port_number >= dest->nb_in)){
		printf(MESS_ERROR"Link not created because the port number '%d' is invalid !\n", port_number);
		return;
	}

	create_link(src, dest, port_number, model->active_circuit);
	return;
}

// 'link delete' 
static void			command_link_delete(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	if ((arg_count == 3) && (strcmp(args[2], "help") == 0))
	{
		printf( "\n• "OPTION(delete)" :"
				"\n  ▻ "COM_OPEN"link "OPTION(delete) OPTION_STR(component source) OPTION_STR(component dest) COM_CLOSE"   : delete a link between two components."
				"\n  ▻ "COM_OPEN"link "OPTION(delete) KEYWORD_ALL OPTION_STR(component name) COM_CLOSE"              : delete all the links of a component. (NOT YET IMPLEMENTED)\n");
		return;
	}
	
	if ((arg_count == 4) && (strcmp(args[2], "all") == 0))
	{
		Component* comp = get_component_by_label(args[3], model->active_circuit);
		if (comp == NULL)
		{
			return;
		}

		delete_all_component_links(model->active_circuit, comp, false);
		printf(MESS_INFO"All inbound and outbound links of '%s' are deleted.\n", comp->label);
		return;
	}
	
	if (arg_count != 5)
	{
		printf(MESS_ERROR"Unknown arguments for "OPTION_COM(link delete)" command. Please type "OPTION_COM(link help)" to see available options with "OPTION_COM(link)" command.\n");
		return;
	}

	Component* src = get_component_by_label(args[2], model->active_circuit);
	if (src == NULL)
	{
		return;
	}
	
	Component* dest = get_component_by_label(args[3], model->active_circuit);
	if (dest == NULL)
	{
		return;
	}

	int port_number = string_to_int(args[4]);

	if ((port_number < 0) || (port_number >= dest->nb_in)){
		printf(MESS_ERROR"Link not deleted because the port number '%d' is invalid !\n", port_number);
		return;
	}

	Link* link = get_link(model->active_circuit, src, dest, port_number);
	if (link != NULL)
	{
		delete_link(model->active_circuit, link);
	}

	printf(MESS_LINK"Link deleted : '%s' -> '%s' (port %d)\n", src->label, dest->label, port_number);
	return;
}

// 'link show' 
static void			command_link_show(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	if ((arg_count == 3) && (strcmp(args[2], "help") == 0))
	{
		printf( "\n• "OPTION(show)" :"
				"\n  ▻ "COM_OPEN"link "OPTION(show) OPTION_STR(component name) COM_CLOSE"                        : show all the inbound and outbound links of a component.\n");
		return;
	}

	Component* comp = get_component_by_label(args[2], model->active_circuit);
	if (comp == NULL)
	{
		return;
	}

	show_component_links(comp);
	return;
}

// 'link help' 
static void			command_link_help(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)args;
	(void)arg_count;
	int link_options_count = sizeof(link_options) / sizeof(link_options[0]);

	printf(MESS_INFO OPTION_COM(link)" command : Use it to manage the links of a circuit.\n\nYou have plenty of options :\n");
	
	exec_full_help("link", link_options, link_options_count, model);

	return;
}

// 'link' 
static void			command_link(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	int counter;
	bool is_not_help;
	int link_commands_count; 
	
	// If there's no options after link 
	if (arg_count < 2)
	{
		printf(MESS_SYNTAX"Please type "OPTION_COM(link help)" to learn how to use this command.\n");
		return;
	}
	
	is_not_help = !((strcmp("help", args[1]) == 0) || ((arg_count >= 3) && (strcmp("help", args[2]) == 0)));
	
	if ((is_not_help) && (model->active_circuit == NULL))
	{
		printf(	MESS_INFO"There's no active circuits.\n"
				MESS_TIP"Before trying to interact with links, please use"OPTION_COM(circuit select)" command to set an active circuit.\n");
		return;
	}

	counter = 0;
	link_commands_count = sizeof(link_options) / sizeof(link_options[0]);

	while (counter < link_commands_count)
	{
		if (strcmp(args[1], link_options[counter].command) == 0)
		{

			//If it's not an help command and there is not enough args : display an error
			if ((is_not_help) && (arg_count < link_options[counter].needed_args))
			{
				printf(MESS_SYNTAX"The command you wrote is invalid, please check the available formats for this command with : "COM_OPEN "link %s help" COM_CLOSE "\n", link_options[counter].command);
				return;
			}
			link_options[counter].function(args, model, arg_count);
			return;
		}
		counter++;
	}

	printf(MESS_ERROR"Unknown "OPTION_COM(help)" command option : '%s'. Type "OPTION_COM(link help)" to see available options with "OPTION_COM(link)" command.\n", args[1]);
	return;
}



// 'component create' 
static void			command_component_create(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	TypeComponent	type;
	bool			type_found;

	if ((arg_count == 3) && (strcmp(args[2], "help") == 0))
	{
		printf( "\n• "OPTION(create)" :"
				"\n  ▻ "COM_OPEN"component "OPTION(create) OPTION_STR(Component_Type) OPTION_STR(component name) OPTION_INT(InboundPorts) COM_CLOSE"                        : Create a component (need to specify the type, the name and the number of inbound ports).\n");
		return;
	}

	type_found = false;
	type = string_to_typecomponent(args[2], &type_found);
	if (type_found == false)
	{
		printf(	MESS_TIP"Type" OPTION_COM(help component) " to get a list of all components types.\n");
		return;
	}

	int in_nbr = string_to_int(args[4]);

	create_component(type, args[3], in_nbr, model->active_circuit);

	return;
}

// 'component delete'
static void			command_component_delete(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	if ((arg_count == 3) && (strcmp(args[2], "help") == 0))
	{
		printf( "\n• "OPTION(delete)" :"
				"\n  ▻ "COM_OPEN"component "OPTION(delete) OPTION_STR(component name) COM_CLOSE"                        : Delete a component (need to specify the name of the component).\n");
		return;
	}

	Component* comp = get_component_by_label(args[2], model->active_circuit);
	if (comp == NULL)
	{
		return;
	}

	if (delete_component(model->active_circuit, comp))
	{
		printf("\n"MESS_CIRC"Component '%s' deleted\n", args[2]);

	}

	return;
}

// 'component rename'
static void			command_component_rename(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	if ((arg_count == 3) && (strcmp(args[2], "help") == 0))
	{
		printf( "\n• "OPTION(rename)" :"
				"\n  ▻ "COM_OPEN"component "OPTION(rename) OPTION_STR(old component name) OPTION_STR(new component name) COM_CLOSE"                        : Rename a component (need to specify the old name and the new name).\n");
		return;
	}

	Component* comp = get_component_by_label(args[2], model->active_circuit);
	if (comp == NULL)
	{
		return;
	}

	rename_component(model->active_circuit, comp, args[3]);

	return;
}

// 'component move'
static void			command_component_move(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	if ((arg_count == 3) && (strcmp(args[2], "help") == 0))
	{
		printf( "\n• "OPTION(move)" :"
				"\n  ▻ "COM_OPEN"component "OPTION(move) OPTION_STR(component name) OPTION_INT(x) OPTION_INT(y) COM_CLOSE"                        : Move a component (need to specify the name of the component and the new " OPTION_INT(x) " and "OPTION_INT(y) " coordinates\n");
		return;
	}

	Component* comp = get_component_by_label(args[2], model->active_circuit);
	if (comp == NULL)
	{
		return;
	}

	int x = string_to_int(args[3]);
	int y = string_to_int(args[4]);


	if ((x < 0) || (x > 10000) || (y < 0) || (y > 10000) ){
		printf(MESS_SYNTAX"x and y values need to be between 0 and 10000 !\n");
		return;
	}

	comp->coordinates->x = x;
	comp->coordinates->y = y;

	printf(MESS_COMP"Component '%s' moved to x:%d y:%d", comp->label, comp->coordinates->x, comp->coordinates->y);
	return;
}

// 'component help'
static void			command_component_help(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)args;
	(void)arg_count;
	int component_commands_count = sizeof(component_options) / sizeof(component_options[0]);

	printf(MESS_INFO OPTION_COM(component)" command : Use it to manage the components of a circuit.\n\nYou have plenty of options :\n");
	
	exec_full_help("component", component_options, component_commands_count, model);

}

// 'component'
static void			command_component(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	int counter;
	bool is_not_help;
	int component_commands_count = sizeof(component_options) / sizeof(component_options[0]);

	
	// If there's no options after component 
	if (arg_count < 2)
	{
		printf(MESS_SYNTAX"Please type "OPTION_COM(component help)" to learn how to use this command.\n");
		return;
	}
	
	is_not_help = !((strcmp("help", args[1]) == 0) || ((arg_count >= 3) && (strcmp("help", args[2]) == 0)));
	
	if ((is_not_help) && (model->active_circuit == NULL))
	{
		printf(	MESS_INFO"There's no active circuits.\n"
				MESS_TIP"Before trying to interact with components, please use"OPTION_COM(circuit select)" command to set an active circuit.\n");
		return;
	}

	counter = 0;
	while (counter < component_commands_count)
	{
		if (strcmp(args[1], component_options[counter].command) == 0)
		{

			//If it's not an help command and there is not enough args : display an error
			if ((is_not_help) && (arg_count < component_options[counter].needed_args))
			{
				printf(MESS_SYNTAX"The command you wrote is invalid, please check the available formats for this command with : "COM_OPEN "component %s help" COM_CLOSE "\n", component_options[counter].command);
				return;
			}
			component_options[counter].function(args, model, arg_count);
			return;
		}
		counter++;
	}

	printf(MESS_ERROR"Unknown "OPTION_COM(help)" command option : '%s'. Type "OPTION_COM(component help)" to see available options with "OPTION_COM(component)" command.\n", args[1]);
	return;
}


void exec_command(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	int counter;
	int commands_count = sizeof(commands) / sizeof(commands[0]);

	// Search between user inputs (args) and registered commands
	counter = 0;
	while(counter < commands_count)
	{
		if (strcmp(args[0], commands[counter].command) == 0)
		{
			commands[counter].function(args, model, arg_count);
			return; 
		}
		counter++;
	}

	// If the command don't exit
	printf(MESS_SYNTAX"The command you wrote dosen't exit. Please use 'help' command to know the available commands.\n");
	return;
}