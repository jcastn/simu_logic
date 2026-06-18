#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions-prototypes.h"
#include "structures.h"

#define TERMINAL_RED		"\033[0;31m"
#define TERMINAL_GREEN		"\033[0;32m"
#define	TERMINAL_BLUE		"\x1b[34m"
#define TERMINAL_MAGENTA	"\x1b[35m"
#define TERMINAL_CYAN		"\x1b[36m"
#define TERMINAL_DEFAULT   	"\033[0m"

void	print_circuit_diodes(Circuit* circ)
{
	if (!circ)
	{
		return;
	}

	printf("Diodes status (Circuit ID:%d) :\n", circ->id);
	int i = 0;
	while (i < circ->component_count)
	{
		Component* comp = circ->components[i];
		if (comp->type == DIODE)
		{
			printf("• Diode (ID: %d) at coordinates (%d, %d) is: %s\n", comp->id, comp->coordinates->x, comp->coordinates->y, comp->out_status ? "ON (true)" : "OFF (false)");
		}
		i++;
	}
}

void	print_circuit_components(Circuit* circ)
{
	printf("The circuit with ID %d contains : %d Levels, %d Components, %d Links \n\n", circ->id, circ->max_level, circ->component_count, circ->link_count);
	printf("•------------•-------•-----•-------•-------•--------•--------•------------------•\n");
	printf("| Components | State | ID  | Level | Align | x      | y      | Links            |\n");
	printf("•------------•-------•-----•-------•-------•--------•--------•------------------•\n");


	for (int i = 0; i<circ->component_count; i++)
	{
		const char* current_color;
		Component* comp = circ->components[i];

		if (comp->type <= 1){
			current_color = TERMINAL_CYAN;
		}
		else 
		{
			current_color = TERMINAL_MAGENTA;
		}

		printf("| %s%-10s " TERMINAL_DEFAULT "| %s%-5s" TERMINAL_DEFAULT " | %-3d | %-5d | %-5d | %-6d | %-6d | In:%-4d Out:%-4d |\n", 
			current_color, 
			ComponentNames[comp->type],
			comp->out_status ? TERMINAL_GREEN : TERMINAL_RED, 
			comp->out_status ? "ON" : "OFF",
			comp->id, 
			comp->coordinates->level, 
			comp->coordinates->alignment, 
			comp->coordinates->x, 
			comp->coordinates->y,
			comp->nb_in,
			comp->nb_out);

	}
	printf("•------------•-------•-----•-------•-------•--------•--------•------------------•\n");
}

/*
void		save_circuit(Circuit *circ)
{
	//Export a circuit to a .txt or .json file 
}

void		import_circuit(Circuit *circ)
{
	//Import a circuit from a .txt or .json file 
}
*/