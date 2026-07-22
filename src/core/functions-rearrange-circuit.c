//functions-rearrange-circuit.c
#include "../../include/prototypes.h"

Component*	update_coordinates(Component* comp, int x, int y)
{
	comp->coordinates->x = x;
	comp->coordinates->y = y;
	return comp;
}

static void	reset_circuit_levels(Circuit *circ)
{
	int counter; 

	circ->max_level = 0;
	counter = 0;

	while(counter < circ->component_count)
	{
		circ->components[counter]->coordinates->level = 0;
		counter++;
	}
	
}

void	rearrange_circuit(Circuit* circ)
{
	if (!circ)
	{
		return;
	}

	reset_circuit_levels(circ);
	topological_sort(circ);
}

static void	propagate_level(Component* comp, int* max_level)
{
	if (!comp || max_level < 0)
	{
		return;
	}
	
	int counter;
	int next_level;
	
	counter = 0;
	// Loop on the out_links of the component
	while (counter < comp->nb_out_links)
	{
		Link* link = comp->out_links[counter];
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
		counter++;
	}
}



void	topological_sort(Circuit* circ)
{
	int counter;

	if (!circ || circ->component_count <= 1)
	{
		return;
	}

	circ->max_level = 0;

	// Init of every root nodes (level == 0)
	counter = 0;
	while (counter < circ->component_count)
	{
		Component* comp = circ->components[counter];
		if (comp->nb_in_links == 0 || comp->in_links == NULL)
		{
			comp->coordinates->level = 0; 
		}
		counter+=1;
	}

	// Propagation from all the root nodes (level == 0)
	counter = 0;
	while (counter < circ->component_count)
	{
		if (circ->components[counter]->coordinates->level == 0)
		{
			propagate_level(circ->components[counter], &(circ->max_level));
		}
		counter+=1;
	}
}