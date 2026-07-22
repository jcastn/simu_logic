//commands-component.c
#include "../../include/prototypes.h"

static void			command_component_create	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_component_delete	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_component_rename	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_component_move		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void			command_component_toggle	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);	
static void			command_component_set		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);	//Not yet implemented
static void			command_component_show		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);	// Will be reworked in the next update
static void			command_component_help		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);

static const CommandMap component_options[] = {
	{"create",		command_component_create,		5,	false},
	{"cre",			command_component_create,		5,	true},
	{"delete",		command_component_delete,		3,	false},
	{"del",			command_component_delete,		3,	true},
	{"rename",		command_component_rename,		4,	false},
	{"ren",			command_component_rename,		4,	true},
	{"move",		command_component_move,			5,	false},
	{"mv",			command_component_move,			5,	true},
	{"show",		command_component_show,			5,	false},
	{"sh",			command_component_show,			5,	true},
	{"toggle",		command_component_toggle,		5,	false},
	{"tog",		command_component_toggle,		5,	true},
	{"set",		command_component_set,			5,	false},
	{"help",		command_component_help,			3,	true}
};


// 'component create' 
static void			command_component_create(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	TypeComponent	type;
	bool			type_found;

	if ((arg_count == 3) && (strcmp(args[2], "help") == 0))
	{
		printf( "\n• "OPTION(create)" :"
				"\n  ▻ "COM_OPEN"component "OPTION(create) OPTION_STR(COMP_TYPE) OPTION_STR(comp name) OPTION_INT(InboundPorts) COM_CLOSE" : Create a component (need to specify the type, the name and the number of inbound ports).\n");
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

	create_component(model->active_circuit, type, args[3], in_nbr);

	return;
}

// 'component delete'
static void			command_component_delete(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	if ((arg_count == 3) && (strcmp(args[2], "help") == 0))
	{
		printf( "\n• "OPTION(delete)" :"
				"\n  ▻ "COM_OPEN"component "OPTION(delete) OPTION_STR(comp name) COM_CLOSE"                          : Delete a component (need to specify the name of the component).\n");
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
				"\n  ▻ "COM_OPEN"component "OPTION(rename) OPTION_STR(old comp name) OPTION_STR(new comp name) COM_CLOSE"      : Rename a component (need to specify the old name and the new name).\n");
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
				"\n  ▻ "COM_OPEN"component "OPTION(move) OPTION_STR(comp name) OPTION_INT(x) OPTION_INT(y) COM_CLOSE"                        : Move a component (need to specify the name of the component and the new " OPTION_INT(x) " and "OPTION_INT(y) " coordinates\n");
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

// 'component show "comp name"'
static void			command_component_show(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)model;
	(void)arg_count;

	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(show)" :"
				"\n  ▻ "COM_OPEN"component "OPTION(show) OPTION_STR(comp name) COM_CLOSE"                            : show the details of a component (NOT YET IMPLEMENTED).\n");
		return;
	}
}

// 'component set "comp name" "state"'
// NOT YET IMPLEMENTED
static void			command_component_set(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)model;
	(void)arg_count;

	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(set)" :"
				"\n  ▻ "COM_OPEN"component "OPTION(set) OPTION_STR(comp name) COM_CLOSE"                            : set the state of a source component (NOT YET IMPLEMENTED).\n");
		return;
	}
}

// 'component toggle "comp name"'
static void			command_component_toggle(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)arg_count;
	const char* status_text;


	if (strcmp(args[2], "help") == 0)
	{
		printf( "\n• "OPTION(toggle)" :"
				"\n  ▻ "COM_OPEN"component "OPTION(toggle) OPTION_STR(comp name) COM_CLOSE"                          : toggle the state of a source component (NOT YET IMPLEMENTED).\n");
		return;
	}

	Component* comp = get_component_by_label(args[2], model->active_circuit);

	invert_source_state(comp);

	if (comp->type == SOURCE)
	{
		comp->out_status.out = !comp->out_status.out;

		if (comp->out_status.out == true)
		{
			status_text = TERMINAL_GREEN"ON";
		}
		else
		{
			status_text = TERMINAL_RED"OFF";
		}
		printf(MESS_COMP"Status of the component '%s' inverted to '%s'"TERMINAL_DEFAULT"\n", comp->label, status_text);
	}
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
void			command_component(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
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