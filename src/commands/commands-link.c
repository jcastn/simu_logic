//commands-link.c
#include "../../include/prototypes.h"

static void			command_link_create			(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_link_delete			(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_link_help			(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);

static const CommandMap link_options[] = {
	{"create",		command_link_create,		4,	false},
	{"cre",			command_link_create,		4,	true},
	{"delete",		command_link_delete,		3,	false},
	{"del",			command_link_delete,		3,	true},
	{"help",		command_link_help,			2,	true}
};

// 'link create' 
static void			command_link_create(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	if ((arg_count == 3) && (strcmp(args[2], "help") == 0))
	{
		printf( "\n• "OPTION(create)" :"
				"\n  ▻ "COM_OPEN"link "OPTION(create) OPTION_STR(comp src) OPTION_STR(comp dest) OPTION_INT(port number) COM_CLOSE"        : create a link from a source component to a destination component and specify\n"
				"                                                              the port number of the destination (from 0 to 10).\n");
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

	if ((port_number < 0) || (port_number >= dest->nb_in_links)){
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
				"\n  ▻ "COM_OPEN"link "OPTION(delete) OPTION_STR(comp src) OPTION_STR(comp dest) COM_CLOSE"                    : delete a link between two components."
				"\n  ▻ "COM_OPEN"link "OPTION(delete) KEYWORD_ALL OPTION_STR(comp name) COM_CLOSE"                           : delete all the links of a component. (NOT YET IMPLEMENTED)\n");
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

	if ((port_number < 0) || (port_number >= dest->nb_in_links)){
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
void			command_link(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
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

	printf(MESS_ERROR"Unknown"OPTION_COM(link)" command option : '%s'. Type"OPTION_COM(link help)" to see available options with"OPTION_COM(link)" command.\n", args[1]);
	return;
}