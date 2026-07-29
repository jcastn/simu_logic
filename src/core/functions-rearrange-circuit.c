//functions-rearrange-circuit.c
#include "../../include/prototypes.h"

Component*	update_coordinates(Component* comp, int x, int y)
{
	comp->coordinates.x = x;
	comp->coordinates.y = y;
	return comp;
}

static void	reset_circuit_levels(Circuit *circ)
{
	int counter; 

	circ->max_level = 0;
	counter = 0;

	while(counter < circ->component_count)
	{
		circ->components[counter]->coordinates.level = 0;
		counter++;
	}
	
}

void	rearrange_circuit(Circuit* circ, bool flag_rearrange_components)
{
	if (!circ)
	{
		return;
	}

	reset_circuit_levels(circ);
	topological_sort(circ, flag_rearrange_components);

	return;
}

static void	propagate_level(Component* comp, int* max_level, bool flag_rearrange_components)
{
	if (!comp || *max_level < 0)
	{
		return;
	}
	
	int counter;
	int counter_bis;
	int next_level;
	
	counter = 0;
	while (counter < comp->nb_out_ports)
	{
		counter_bis = 0;
		// Loop on the out_links of the component
		while (counter_bis < comp->out_ports[counter]->nb_out_links)
		{
			Link* link = comp->out_ports[counter]->out_links[counter_bis];
			if (link && link->dest)
			{
				Component* dest = link->dest;
				next_level = comp->coordinates.level + 1;

				if (next_level > dest->coordinates.level)
				{
					dest->coordinates.level = next_level;
					if (flag_rearrange_components)
					{
						update_coordinates(dest, dest->coordinates.level*100, dest->coordinates.alignment*100);
					}

					// Verification and update of the max_level of the circuit
					if (dest->coordinates.level > *max_level)
					{
						*max_level = dest->coordinates.level;
					}

					propagate_level(dest, max_level, flag_rearrange_components);
				}
			}
			counter_bis++;
		}
		counter++;
	}
}


void	topological_sort(Circuit* circ, bool flag_rearrange_components)
{
	int counter;

	if (!circ || circ->component_count <= 1)
	{
		return;
	}

	// Propagation from all the root nodes (level == 0)
	counter = 0;
	while (counter < circ->component_count)
	{
		if (circ->components[counter]->coordinates.level == 0)
		{
			propagate_level(circ->components[counter], &(circ->max_level), flag_rearrange_components);
		}
		counter+=1;
	}
}