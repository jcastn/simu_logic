//commands-help.c
#include "../../include/prototypes.h"
#include "../../include/prototypes-commands.h"

void	display_help	(char* command_name, char* command_description)
{
	int counter = 0;
	int options_count = 0;
	const SubCommandMap* list_options = get_sub_command_map(command_name, &options_count);

	if (options_count == 0)
	{
		printf(MESS_INFO "You can use the "TERMINAL_YELLOW "'%s'"TERMINAL_DEFAULT " command to %s.\n", command_name, command_description);
	}
	else
	{
		printf(MESS_INFO "You can use the "TERMINAL_YELLOW "'%s'"TERMINAL_DEFAULT " command to %s.\nYou have plenty of options :\n", command_name, command_description);
		
		while (counter < options_count)
		{
			if (list_options[counter].is_alias == false)
			{
				exec_command((char*[]){command_name, list_options[counter].command, "help", NULL, NULL}, NULL, 3);
			}
			counter++;
		}
	}
	return;
}


// Command help
void	command_help	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count)
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
					"\n• "OPTION_COM(quit)"      : Close the application properly."
					"\n• "OPTION_COM(reset)"     : Reset the app to its loading state.\n"

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
					"\n" TERMINAL_WHITE "Sources, Diodes & Numbers display:" TERMINAL_DEFAULT "\n"
					"  ▻ "TERMINAL_CYAN"SOURCE"TERMINAL_DEFAULT"      : Outputs a binary signal ("TERMINAL_GRAY"TRUE"TERMINAL_DEFAULT" or "TERMINAL_GRAY"FALSE"TERMINAL_DEFAULT").\n"
					"  ▻ "TERMINAL_BLUE"DIODE"TERMINAL_DEFAULT"       : Single-input indicator that shows the received binary state ("TERMINAL_GRAY"ON = TRUE"TERMINAL_DEFAULT" / "TERMINAL_GRAY"OFF = FALSE"TERMINAL_DEFAULT").\n"
					"  ▻ "TERMINAL_BLUE"DIODE_RGB"TERMINAL_DEFAULT"   : Three-input indicator that shows a color based on the binary combination.\n"
					"  ▻ "TERMINAL_YELLOW"DISPLAY_DEC"TERMINAL_DEFAULT" : Display a decimal number depending on the inbound binary signals.\n"
					"  ▻ "TERMINAL_YELLOW"DISPLAY_HEX"TERMINAL_DEFAULT" : Display a hex number depending on the inbound binary signals.\n"
					
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