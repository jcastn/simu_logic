//commands-list.c
#include "../../include/prototypes.h"

static void			command_list_circuit		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_list_components		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_list_links			(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_list_help			(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);


static const CommandMap list_options[] = {
	{"circuits",	command_list_circuit,		2,	false},
	{"circuit",		command_list_circuit,		2,	true},
	{"circ",		command_list_circuit,		2,	true},
	{"components",	command_list_components,	3,	false},
	{"component",	command_list_components,	3,	true},
	{"comp",		command_list_components,	3,	true},
	{"links",		command_list_links,			3,	false},
	{"link",		command_list_links,			3,	true},
	{"help",		command_list_help,			2,	true}
};

// list circuits
static void			command_list_circuit		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	if ((arg_count == 3) && (strcmp(args[2], "help") == 0))
	{
		printf( "\n• "OPTION(circuits)" :"
				"\n  ▻ "COM_OPEN "list circuits" COM_CLOSE"                                         : Display a list of all loaded circuits.\n");
		return;
	}

	list_loaded_circuits(model);
	return;
}


// list components (of a circuit)
static void			command_list_components		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)arg_count;

	// 'list components help'
	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(components)" :"
				"\n  ▻ "COM_OPEN "list components" KEYWORD_ALL COM_CLOSE"                                   : Show all the components of all the loaded circuits."
				"\n  ▻ "COM_OPEN "list components" KEYWORD_ACTIVE COM_CLOSE"                                : Show all the components of the active circuit (use 'circuit select' command)."
				"\n  ▻ "COM_OPEN "list components" OPTION_CIRCUIT(circuit name) COM_CLOSE"                        : Show all the components of the given circuit.\n");
		return;
	}

	// 'list components all'
	if (strcmp(args[2], "all") == 0)
	{
		show_components_from_model(model);
		return;
	}

	// 'list components "circuit name"'
	Circuit* circ = get_circuit_by_label(model, args[2]);
	if (circ != NULL)
	{
		show_components_from_circuit(circ);
	}

}


// list links (of a circuit)
static void			command_list_links			(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)arg_count;
	int counter;

	// 'list links help'
	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(links)" :"
				"\n  ▻ "COM_OPEN "list links" KEYWORD_ALL COM_CLOSE"                                        : Show all the links of all the loaded circuits."
				"\n  ▻ "COM_OPEN "list links" KEYWORD_ACTIVE COM_CLOSE"                                     : Show all the links of the active circuit (use 'circuit select' command)."
				"\n  ▻ "COM_OPEN "list links" OPTION_CIRCUIT(circuit name) COM_CLOSE"                             : Show all the links of the given circuit.\n");
		return;
	}

	// 'list links all'
	if (strcmp(args[2], "all") == 0)
	{
		counter = 0;
		while (counter < model->circuits_count)
		{
			show_links_from_circuit(model->circuits[counter]);
			counter++;
		}
		return;
	}

	// 'list links "circuit name"'
	Circuit* circ = get_circuit_by_label(model, args[2]);
	if (circ != NULL)
	{
		show_links_from_circuit(circ);
	}
}


// 'list help' 
static void			command_list_help(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)args;
	(void)arg_count;

	int list_options_count = sizeof(list_options) / sizeof(list_options[0]);

	printf(MESS_INFO OPTION_COM(list)" command : Use it to display information about loaded circuits.\n\nYou have plenty of options :\n");
	
	exec_full_help("list", list_options, list_options_count, model);

	return;
}

// 'list' 
void			command_list(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	int counter;
	bool is_not_help;
	int list_commands_count; 
	
	// If there's no options after list 
	if (arg_count < 2)
	{
		printf(MESS_SYNTAX"Please type "OPTION_COM(list help)" to learn how to use this command.\n");
		return;
	}
	
	is_not_help = !((strcmp("help", args[1]) == 0) || ((arg_count >= 3) && (strcmp("help", args[2]) == 0)));
	replace_active_keyword(model, args, arg_count);

	counter = 0;
	list_commands_count = sizeof(list_options) / sizeof(list_options[0]);

	while (counter < list_commands_count)
	{
		if (strcmp(args[1], list_options[counter].command) == 0)
		{

			//If it's not an help command and there is not enough args : display an error
			if ((is_not_help) && (arg_count < list_options[counter].needed_args))
			{
				printf(MESS_SYNTAX"The command you wrote is invalid, please check the available formats for this command with : "COM_OPEN "list %s help" COM_CLOSE "\n", list_options[counter].command);
				return;
			}
			list_options[counter].function(args, model, arg_count);
			return;
		}
		counter++;
	}

	printf(MESS_ERROR"Unknown "OPTION_COM(help)" command option : '%s'. Type "OPTION_COM(list help)" to see available options with "OPTION_COM(list)" command.\n", args[1]);
	return;
}