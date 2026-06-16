#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions-prototypes.h"
#include "structures.h"

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
			printf("• Diode (ID: %d) at coordinates (%d, %d) is: %s\n", comp->id, comp->x, comp->y, comp->out_status ? "ON (true)" : "OFF (false)");
		}
		i++;
	}
}

void	print_circuit_components(Circuit* circ)
{
	printf("Components status (Circuit ID:%d) :\n", circ->id);
	for (int i = 0; i<circ->component_count; i++)
	{
		Component* comp = circ->components[i];
		printf("• Component %s (ID: %d) | Level : %d | Out Status : %s\n", ComponentNames[comp->type], comp->id, comp->level, comp->out_status ? "ON (true)" : "OFF (false)");		
	}
}



/*

void		save_circuit(Circuit *circ)
{
	//Export the given circuit to a .txt file 
}

void		import_circuit(Circuit *circ)
{
	//Import a circuit from a .txt file 
}
*/