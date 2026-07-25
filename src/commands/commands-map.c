//command-map.c
#include "../../include/prototypes-commands.h"

// Mapping of each commands names with : 
// - the command function,
// - the required number of args to run the command,
// - the boolean to see if the command is the command is an alias or not
// - the sub-function CommandMap, if the command have sub-fonctions (if not it's NULL).
// - the sub-function size, if the command have sub-fonctions (if not it's 0).
 
//  commnand name,      command function,      	needed args,   is_alias

// Circuit options 

static const SubCommandMap circuit_options[] = {
	{"create",		command_circuit_create,			3,		false},
	{"cre",			command_circuit_create,			3,		true},
	{"delete",		command_circuit_delete,			3,		false},
	{"del",			command_circuit_delete,			3,		true},
	{"clear",		command_circuit_clear,			3,		false},
	{"cl",			command_circuit_clear,			3,		true},
	{"rename",		command_circuit_rename,			4,		false},
	{"ren",			command_circuit_rename,			4,		true},
	{"duplicate",	command_circuit_duplicate,		3,		false},
	{"dup",			command_circuit_duplicate,		3,		true},
	{"import",		command_circuit_import,			4,		false},
	{"im",			command_circuit_import,			4,		true},
	{"export",		command_circuit_export,			4,		false},
	{"ex",			command_circuit_export,			4,		true},
	{"rearrange",	command_circuit_rearrange,		3,		false},
	{"rea",		command_circuit_rearrange,		3,		true},
	{"select",		command_circuit_select,			3,		false},
	{"sel",		command_circuit_select,			3,		true},
	{"unselect",	command_circuit_unselect,		2,		false},
	{"unsel",		command_circuit_unselect,		2,		true},
	{"simulate",	command_circuit_simulate,		3,		false},
	{"simu",		command_circuit_simulate,		3,		true},
};


// Component options

static const SubCommandMap component_options[] = {
	{"create",		command_component_create,		5,		false},
	{"cre",			command_component_create,		5,		true},
	{"delete",		command_component_delete,		3,		false},
	{"del",			command_component_delete,		3,		true},
	{"rename",		command_component_rename,		4,		false},
	{"ren",			command_component_rename,		4,		true},
	{"move",		command_component_move,			5,		false},
	{"mv",			command_component_move,			5,		true},
	{"show",		command_component_show,			3,		false},
	{"sh",			command_component_show,			3,		true},
	{"toggle",		command_component_toggle,		3,		false},
	{"tog",		command_component_toggle,		3,		true},
	{"set",		command_component_set,			4,		false},
};


// Link options 

static const SubCommandMap link_options[] = {
	{"create",		command_link_create,			4,		false},
	{"cre",			command_link_create,			4,		true},
	{"delete",		command_link_delete,			3,		false},
	{"del",			command_link_delete,			3,		true},
};


// List options 

static const SubCommandMap list_options[] = {
	{"circuits",	command_list_circuit,			2,		false},
	{"circuit",		command_list_circuit,			2,		true},
	{"circ",		command_list_circuit,			2,		true},
	{"components",	command_list_components,		3,		false},
	{"component",	command_list_components,		3,		true},
	{"comp",		command_list_components,		3,		true},
	{"links",		command_list_links,				3,		false},
	{"link",		command_list_links,				3,		true},
};


// Main commands 
//  commnand name,      description,                            command function,     needed args,    is_alias,      sub_func,               sub_func_size

static const CommandMap commands[] = {
	{"circuit",		"manage loaded circuits",						command_circuit,		2,		false,		circuit_options,   		sizeof(circuit_options) / sizeof(SubCommandMap)},
	{"circ",		"manage loaded circuits",						command_circuit,		2,		true,		circuit_options,   		sizeof(circuit_options) / sizeof(SubCommandMap)},
	{"circuits",	"manage loaded circuits",						command_circuit,		2,		true,		circuit_options,   		sizeof(circuit_options) / sizeof(SubCommandMap)},
	{"component",	"manage the components of a circuit",			command_component,		2,		false,		component_options, 		sizeof(component_options) / sizeof(SubCommandMap)},
	{"comp",		"manage the components of a circuit",			command_component,		2,		true,		component_options, 		sizeof(component_options) / sizeof(SubCommandMap)},
	{"help",		"display the guide",							command_help,			2,		false,		NULL,					0},
	{"hello",		"display an hello world",						command_hello,			1,		false,		NULL,					0},
	{"wesh",		"display an hello world",						command_hello,			1,		true,		NULL,					0},
	{"yo",			"display an hello world",						command_hello,			1,		true,		NULL,					0},
	{"link",		"manage the links of a circuit",				command_link,			2,		false,		link_options,			sizeof(link_options) / sizeof(SubCommandMap)},
	{"list",		"display information about loaded circuits",	command_list,			2,		false,		list_options,			sizeof(list_options) / sizeof(SubCommandMap)},
	{"ls",			"display information about loaded circuits",	command_list,			2,		true,		list_options,			sizeof(list_options) / sizeof(SubCommandMap)},
	{"quit",		"leave the app",								command_quit,			1,		false,		NULL,					0},
	{"exit",		"leave the app",								command_quit,			1,		true,		NULL,					0},
	{"close",		"leave the app",								command_quit,			1,		true,		NULL,					0},
	{"leave",		"leave the app",								command_quit,			1,		true,		NULL,					0},
	{"reset",		"reset the app to its loading state",			command_reset,			1,		false,		NULL,					0}
};

const CommandMap* get_command_map(int* count)
{
	*count = sizeof(commands) / sizeof(commands[0]);
	return commands;
}

const SubCommandMap* get_sub_command_map(const char* command_name, int* count)
{
	int main_count = 0;
	const CommandMap* commands = get_command_map(&main_count);

	for (int i = 0; i < main_count; i++)
	{
		if (strcmp(commands[i].command, command_name) == 0)
		{
			*count = commands[i].sub_commands_count;
			return commands[i].sub_commands;
		}
	}

	*count = 0;
	return NULL;
}