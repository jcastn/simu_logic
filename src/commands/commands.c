//commands.c
#include "../../include/prototypes.h"

static void		command_help	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void		command_hello	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void		command_reset	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
static void		command_quit	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);


// The commands functions that have their own file are prototyped in the include/prototypes.h file


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
	{"wesh",		command_hello,				1,	true},
	{"yo",			command_hello,				1,	true},
	{"link",		command_link,				2, 	false},
	{"list",		command_list,				2, 	false},
	{"ls",			command_list,				2, 	true},
	{"quit",		command_quit,				1,	false},
	{"exit",		command_quit,				1,	true},
	{"close",		command_quit,				1,	true},
	{"leave",		command_quit,				1,	true},
	{"reset",		command_reset,				1,	false}
};

void		exec_full_help(char* command_name, const CommandMap command_map[], int command_count, Model* model)
{
	int counter;
	
	counter = 0;
	while (counter < command_count)
	{
		if (command_map[counter].is_alias == false)
		{
			exec_command((char*[]){command_name, command_map[counter].command, "help", NULL, NULL}, model, 3);
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
				"\n• "OPTION_COM(help aliases)" to the list of commands and options aliases."
				"\n• "OPTION_COM(help commands)" to see the list of commands."
				"\n• "OPTION_COM(help components)" to see the list of available components and how they work."
				"\n• "OPTION_COM(help links)" to see how the links work (not yet implemented).\n");
		return;
	}

	if (arg_count == 2)
	{
		if (strcmp(args[1], "aliases") == 0)
		{
			printf( MESS_TIP"The aliases allows you to write commands with shorter arguments ! Once you know them, you will be able write commands at the speed of light !\n"
					"\nList of commands aliases :"
					"\n• "OPTION_COM(circuit)"   : "OPTION_COM(circ)
					"\n• "OPTION_COM(component)" : "OPTION_COM(comp)
					"\n• "OPTION_COM(hello)"     : "OPTION_COM(wesh) "," OPTION_COM(yo)
					"\n• "OPTION_COM(list)"      : "OPTION_COM(ls)
					"\n• "OPTION_COM(quit)"      : "OPTION_COM(exit) "," OPTION_COM(close) "," OPTION_COM(leave)"\n"
					"\nList of options aliases :"
					"\n• "OPTION_COM(create)"    : "OPTION_COM(cre)
					"\n• "OPTION_COM(delete)"    : "OPTION_COM(del)
					"\n• "OPTION_COM(duplicate)" : "OPTION_COM(dup)
					"\n• "OPTION_COM(clear)"     : "OPTION_COM(cl)
					"\n• "OPTION_COM(rearrange)" : "OPTION_COM(rea)
					"\n• "OPTION_COM(rename)"    : "OPTION_COM(ren)
					"\n• "OPTION_COM(import)"    : "OPTION_COM(im)
					"\n• "OPTION_COM(export)"    : "OPTION_COM(ex)
					"\n• "OPTION_COM(toggle)"    : "OPTION_COM(tog)
					"\n• "OPTION_COM(select)"    : "OPTION_COM(sel)
					"\n• "OPTION_COM(unselect)"  : "OPTION_COM(unsel)
					"\n• "OPTION_COM(show)"      : "OPTION_COM(sh)
					"\n• "OPTION_COM(simulate)"  : "OPTION_COM(simu)
					"\n• "OPTION_COM(move)"      : "OPTION_COM(mv)"\n");
			return;
		}

		if (strcmp(args[1], "commands") == 0)
		{
			printf(	"\nList of commands :"
					"\n• "OPTION_COM(circuit)"   : Interact with a circuit."
					"\n• "OPTION_COM(component)" : Interact with the components of a circuit."
					"\n• "OPTION_COM(hello)"     : Displays an"OPTION_STR(Hello World !)" message."
					"\n• "OPTION_COM(help)"      : User guide of the app."
					"\n• "OPTION_COM(link)"      : Interact with the links of a circuit."
					"\n• "OPTION_COM(list)"      : Displays a list of all the loaded circuits or all the components/links of a circuit."
					"\n• "OPTION_COM(quit)"      : Close the application properly.\n"
					MESS_TIP"You can write"OPTION_COM(help)" after the name of a command to know how to use it !\n");
			return;
		}

		if (strcmp(args[1], "link") == 0)
		{
			printf(	MESS_ERROR"\nSORRY ! This help page is not yet added ! :(\n");
			return;
		}

		if ((strcmp(args[1], "components") == 0) || (strcmp(args[1], "comp") == 0))
		{
			printf(	"\nList of available components :\n"
					"\n" TERMINAL_WHITE "Sources & Diodes:" TERMINAL_DEFAULT "\n"
					"  ▻ "TERMINAL_CYAN"SOURCE"TERMINAL_DEFAULT"      : Outputs a binary signal ("TERMINAL_GRAY"TRUE"TERMINAL_DEFAULT" or "TERMINAL_GRAY"FALSE"TERMINAL_DEFAULT").\n"
					"  ▻ "TERMINAL_BLUE"DIODE"TERMINAL_DEFAULT"       : Single-input indicator that shows the received binary state ("TERMINAL_GRAY"ON = TRUE"TERMINAL_DEFAULT" / "TERMINAL_GRAY"OFF = FALSE"TERMINAL_DEFAULT").\n"
					"  ▻ "TERMINAL_BLUE"DIODE_RGB"TERMINAL_DEFAULT"   : Three-input indicator that shows a color based on the binary combination.\n"
					
					"\n" TERMINAL_WHITE "Single-Input Gates: (1 input)" TERMINAL_DEFAULT "\n"
					"  ▻ "TERMINAL_PINK"GATE_NOT"TERMINAL_DEFAULT"    : Inverts the input binary signal ("TERMINAL_GRAY"TRUE → FALSE"TERMINAL_DEFAULT" / "TERMINAL_GRAY"FALSE → TRUE"TERMINAL_DEFAULT").\n"
					"  ▻ "TERMINAL_PINK"BUFFER"TERMINAL_DEFAULT"      : Replicates the input binary signal ("TERMINAL_GRAY"TRUE → TRUE"TERMINAL_DEFAULT" / "TERMINAL_GRAY"FALSE → FALSE"TERMINAL_DEFAULT").\n"
					
					"\n" TERMINAL_WHITE "Multi-Input Gates: (2 to "COMPONENTS_IN_PORTS_STR" inputs)" TERMINAL_DEFAULT "\n"
					"  ▻ "TERMINAL_MAGENTA"GATE_AND"TERMINAL_DEFAULT"    : Outputs "TERMINAL_GRAY"TRUE"TERMINAL_DEFAULT" only if ALL inputs are "TERMINAL_GRAY"TRUE"TERMINAL_DEFAULT".\n"
					"  ▻ "TERMINAL_MAGENTA"GATE_OR"TERMINAL_DEFAULT"     : Outputs "TERMINAL_GRAY"TRUE"TERMINAL_DEFAULT" if AT LEAST ONE input is "TERMINAL_GRAY"TRUE"TERMINAL_DEFAULT".\n"
					"  ▻ "TERMINAL_MAGENTA"GATE_XOR"TERMINAL_DEFAULT"    : Outputs "TERMINAL_GRAY"TRUE"TERMINAL_DEFAULT" if the number of "TERMINAL_GRAY"TRUE"TERMINAL_DEFAULT" inputs is "TERMINAL_GRAY"ODD"TERMINAL_DEFAULT".\n"
					
					"\n" TERMINAL_WHITE "Inverted Multi-Input Gates: (2 to "COMPONENTS_IN_PORTS_STR" inputs)" TERMINAL_DEFAULT "\n"
					"  ▻ "TERMINAL_PURPLE"GATE_NAND"TERMINAL_DEFAULT"   : Inverted GATE_AND. Outputs "TERMINAL_GRAY"TRUE"TERMINAL_DEFAULT" if AT LEAST ONE input is "TERMINAL_GRAY"FALSE"TERMINAL_DEFAULT".\n"
					"  ▻ "TERMINAL_PURPLE"GATE_NOR"TERMINAL_DEFAULT"    : Inverted GATE_OR. Outputs "TERMINAL_GRAY"TRUE"TERMINAL_DEFAULT" only if ALL inputs are "TERMINAL_GRAY"FALSE"TERMINAL_DEFAULT".\n"
					"  ▻ "TERMINAL_PURPLE"GATE_NXOR"TERMINAL_DEFAULT"   : Inverted GATE_XOR. Outputs "TERMINAL_GRAY"TRUE"TERMINAL_DEFAULT" if the number of "TERMINAL_GRAY"TRUE"TERMINAL_DEFAULT" inputs is "TERMINAL_GRAY"EVEN"TERMINAL_DEFAULT".\n"
					
					"\n" TERMINAL_WHITE "Implication Gates (2 inputs):" TERMINAL_DEFAULT "\n"
					"  ▻ "TERMINAL_VIOLET"GATE_IMPLY"TERMINAL_DEFAULT"  : Logical Implication gate.\n"
					"  ▻ "TERMINAL_VIOLET"GATE_NIMPLY"TERMINAL_DEFAULT" : Logical Non-Implication gate.\n"
					
					MESS_TIP"To connect these components together in a circuit, use the"OPTION_COM(link)" command. Learn how they work with"OPTION_COM(help link)" command.\n" TERMINAL_DEFAULT);
			return;
		}
	}
	printf(MESS_ERROR"Unknown help topic '%s'. Type"OPTION_COM(help)" to see all available topics.\n", args[1]);
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

// Command 'reset' 
static void	command_reset(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)args;
	(void)arg_count;

	delete_model(model, false);
	return;
}

// Command quit 
static void	command_quit(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
{
	(void)args;
	(void)arg_count;

	printf(MESS_INFO"The app was stopped by the user.\n");

	model->run_loop = false;
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
	printf(MESS_SYNTAX"The command you wrote doesn't exist. Please use 'help' command to know the available commands.\n");
	return;
}