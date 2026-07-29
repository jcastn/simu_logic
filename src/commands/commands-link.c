//commands-link.c
#include "../../include/prototypes.h"
#include "../../include/prototypes-commands.h"

// 'link create' 
void			command_link_create(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	if ((arg_count == 3) && (strcmp(args[2], "help") == 0))
	{
		printf( "\n• "OPTION(create)" :"
				"\n  ▻ "COM_OPEN"link "OPTION(create) OPTION_STR(src comp) OPTION_INT(src port) OPTION_STR(dest comp) OPTION_INT(dest port) COM_CLOSE"        : Create a link from a source component to a destination component and specify\n"
				"                                                              the port number of the destination (from 1 to 8).\n");
		return;
	}

	// SRC component and ports 
	Component* src = get_component_by_label(args[2], model->active_circuit);
	if (src == NULL)
	{
		return;
	}

	int src_input_port = string_to_int(args[3]);
	int src_port_number = PORT_INPUT(src_input_port);

	if (src_port_number < 0 || src_port_number >= src->nb_out_ports)
	{
		printf(MESS_ERROR "Invalid source port index '%d' for component '%s'.\n", PORT_DISPLAY(src_port_number), src->label);
		return;
	}

	// DEST component and ports 
	Component* dest = get_component_by_label(args[4], model->active_circuit);
	if (dest == NULL)
	{
		return;
	}

	int dest_input_port = string_to_int(args[5]);
	int dest_port_number = PORT_INPUT(dest_input_port);

	if (dest_port_number < 0 || dest_port_number >= dest->nb_in_ports)
	{
		printf(MESS_ERROR "Invalid destination port index '%d' for component '%s'.\n", PORT_DISPLAY(dest_port_number), dest->label);
		return;
	}

	if (dest->in_links[dest_port_number] != NULL)
	{
		printf(MESS_ERROR"A link is already connected on the inbound port '%d' of the component '%s'. Link not created !\n", PORT_DISPLAY(dest_port_number), dest->label);
		return;
	}

	// Link creation
	Link* link = create_link(model->active_circuit, src, src_port_number, dest, dest_port_number);
	if (!link)
	{
		printf(MESS_ERROR"Link not created.\n");
		return;
	}

	printf(MESS_LINK"Link created : '%s%s (port %d)%s'  → '%s%s (port %d)%s' \n", COMPONENT_MAP[src->type].color, src->label, PORT_DISPLAY(src_port_number), TERMINAL_DEFAULT, COMPONENT_MAP[dest->type].color, dest->label, PORT_DISPLAY(dest_port_number), TERMINAL_DEFAULT);
	return;
}

// 'link delete' 
void			command_link_delete(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	if ((arg_count == 3) && (strcmp(args[2], "help") == 0))
	{
		printf( "\n• "OPTION(delete)" :"
				"\n  ▻ "COM_OPEN"link "OPTION(delete) OPTION_STR(src comp) OPTION_INT(src port) OPTION_STR(dest comp) OPTION_INT(dest port) COM_CLOSE"                    : Delete all the links that are between two components."
				"\n  ▻ "COM_OPEN"link "OPTION(delete) KEYWORD_ALL OPTION_STR(comp name) COM_CLOSE"                           : Delete all the links of a component.\n");
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
		printf(MESS_INFO"All inbound and outbound links of '%s%s%s' are deleted.\n", COMPONENT_MAP[comp->type].color, comp->label, TERMINAL_DEFAULT);
		return;
	}
	
	if (arg_count != 6)
	{
		printf(MESS_ERROR"Unknown arguments for "OPTION_COM(link delete)" command. Please type "OPTION_COM(link help)" to see available options with "OPTION_COM(link)" command.\n");
		return;
	}

	Component* src = get_component_by_label(args[2], model->active_circuit);
	if (src == NULL)
	{
		return;
	}

	int input_src_port = string_to_int(args[3]);
	int src_port_number = PORT_INPUT(input_src_port);
	
	Component* dest = get_component_by_label(args[4], model->active_circuit);
	if (dest == NULL)
	{
		return;
	}

	int input_dest_port = string_to_int(args[5]);
	int dest_port_number = PORT_INPUT(input_dest_port);

	if ((dest_port_number < 0) || (dest_port_number >= dest->nb_in_ports)){
		printf(MESS_ERROR"Link not deleted because the port number '%d' is invalid !\n", input_dest_port);
		return;
	}

	Link* link = get_link(model->active_circuit, src, src_port_number, dest, dest_port_number);
	if (link != NULL)
	{
		delete_link(model->active_circuit, link);
		printf(MESS_LINK"Link deleted : '%s%s (port : %d)%s'  -> '%s%s (port : %d)%s'\n", COMPONENT_MAP[src->type].color, src->label, input_src_port, TERMINAL_DEFAULT, COMPONENT_MAP[dest->type].color, dest->label, input_dest_port, TERMINAL_DEFAULT);
	}
	else
	{
		printf(MESS_INFO"No link found ! (Maybe the port number is invalid ?) \n");
	}
	return;
}

// 'link' 
void			command_link(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	int counter;
	int options_count;
	bool is_not_help;
	
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
	const SubCommandMap* link_options = get_sub_command_map("link", &options_count);


	while (counter < options_count)
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