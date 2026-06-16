#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

// Function used to shift the right part content of a pointer array to the left because a pointer was removed from the array (very specific)
void	shift_pointer_array(void** array, int start_index, int current_count)
{
    int i = start_index;
    while (i < current_count - 1)
    {
        array[i] = array[i + 1];
        i++;
    }
}

static void	propagate_level(Component* comp)
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
			next_level = comp->level + 1;

			if (next_level > dest->level)
			{
				dest->level = next_level;
				propagate_level(dest);
			}
		}
		i++;
	}
}


void	topological_sort(Circuit* circ)
{
	int i;

	if (!circ || circ->component_count <= 1)
		return;

	// Init of every root nodes (level == 0)
	i = 0;
	while (i < circ->component_count)
	{
		Component* comp = circ->components[i];
		if (comp->nb_in == 0 || comp->in_links == NULL)
		{
			comp->level = 0; 
		}
		i+=1;
	}

	// Propagation from all the root nodes (level == 0)
	i = 0;
	while (i < circ->component_count)
	{
		if (circ->components[i]->level == 0)
		{
			propagate_level(circ->components[i]);
		}
		i+=1;
	}
}