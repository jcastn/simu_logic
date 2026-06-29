//functions-rearrange-circuit.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "prototypes.h"
#include "structures.h"

Component*	update_coordinates(Component* comp, int x, int y)
{
	comp->coordinates->x = x;
	comp->coordinates->y = y;
	return comp;
}

void	rearange_circuit(Circuit* circ)
{
	if (!circ)
		return;

	topological_sort(circ);
}

static void	propagate_level(Component* comp, int* max_level)
{
	int i;
	int next_level;
	
	i = 0;
	// Loop on the out_links of the component
	while (i < comp->nb_out)
	{
		Link* link = comp->out_links[i];
		if (link && link->dest)
		{
			Component* dest = link->dest;
			next_level = comp->coordinates->level + 1;

			if (next_level > dest->coordinates->level)
			{
				dest->coordinates->level = next_level;
				update_coordinates(dest, dest->coordinates->level*100, dest->coordinates->alignment*100);

				// Verification and update of the max_level of the circuit
				if (dest->coordinates->level > *max_level)
                {
                    *max_level = dest->coordinates->level;
                }

				propagate_level(dest, max_level);
			}
		}
		i++;
	}
}


void	topological_sort(Circuit* circ)
{
	int i;

	circ->max_level = 0;

	if (!circ || circ->component_count <= 1)
		return;

	// Init of every root nodes (level == 0)
	i = 0;
	while (i < circ->component_count)
	{
		Component* comp = circ->components[i];
		if (comp->nb_in == 0 || comp->in_links == NULL)
		{
			comp->coordinates->level = 0; 
		}
		i+=1;
	}

	// Propagation from all the root nodes (level == 0)
	i = 0;
	while (i < circ->component_count)
	{
		if (circ->components[i]->coordinates->level == 0)
		{
			propagate_level(circ->components[i], &(circ->max_level));
		}
		i+=1;
	}
}

