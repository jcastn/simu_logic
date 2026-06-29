//functions-components.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prototypes.h"
#include "structures.h"

static int	auto_nb_in(TypeComponent type, int in_nbr)
{
	if (type == SOURCE)
	{
		return 0;
	}
	else if ((type == DIODE) || (type == GATE_NOT) || (type == BUFFER ) || (type == INPUT) || (type == OUTPUT))
	{
		return 1;
	}
	else if ((type == GATE_IMPLY) || (type == GATE_NIMPLY))
	{
		return 2;
	}
	else if (type == DIODE_RGB)
	{
		return 3;
	}
	else if (in_nbr >= 10)
	{
		return 10;
	}
	else
	{
		return in_nbr;
	}
}

// Function to create a component with : 
// • its Type (SOURCE, DIODE, NOT / AND / OR / NAND / NOR / XOR / NXOR gates),
// • its number of inbound links 
// • the circuit where the component is included
Component*	create_component(TypeComponent type, int in_nbr, Circuit* circ)
{
	int	i;
	static int next_comp_id = 0;

	Component* comp = malloc(sizeof(Component));
	comp->coordinates = malloc(sizeof(Coordinates));
	if (!circ || !comp || (comp->coordinates == NULL) )
	{
		printf("/!\\ ERROR : Circuit or component not find (Function create_component)\n");
		return NULL;
	}

	//Init of the component values
	comp->id = next_comp_id += 1;
	comp->type = type;
	comp->out_status = false;
	comp->nb_out = 0;
	comp->out_links = NULL;
	comp->nb_in = auto_nb_in(type, in_nbr);

	//Allocation of the table of link pointers 
	if (comp->nb_in > 0)
	{
		comp->in_links = malloc(sizeof(Link*) * comp->nb_in);
		if (comp->in_links == NULL)
		{
			free(comp);
			return NULL;
		}
		i = 0;
		while (i < comp->nb_in)
		{
			comp->in_links[i] = NULL;
			i++;
		}
	}
	else 
	{
		comp->in_links = NULL;
	}

	// Dynamic enlargement of the array of link pointers
	Component** tmp = realloc(circ->components, sizeof(Component*) * (circ->component_count + 1));
	if (tmp == NULL)
	{
		printf("/!\\ ERROR : Realloc of the array of link pointers failed (function create_component)\n");
		return NULL;
	}
	circ->components = tmp;

	circ->components[circ->component_count] = comp;
	circ->component_count += 1;
	circ->type_counter[type].count += 1;
	
	snprintf(comp->label, sizeof(comp->label), "%s_%d", ComponentNames[type], circ->type_counter[type].count);

	printf("▷ Component created : %s\n", comp->label);

	return comp;
}

bool	delete_component(Circuit* circ, Component* comp)
{
	int i;
	int	index;

	if (!circ || !comp)
	{
		return false;
	}

	index = -1;
	i = 0;
	while(i < circ->component_count) 
	{
		if (circ->components[i] == comp)
		{
			index = i;
			break;
		}
		i+=1;
	}
	if (index == -1)
	{
		return false;
	}	


	// Loops to delete inbound and outbound links with delete_link()
	if (comp->in_links) 
	{
		i = 0;
		while(i < comp->nb_in) 
		{
			if (comp->in_links[i]) 
			{
				delete_link(circ, comp->in_links[i]);
			}
			i+=1;
		}
		free(comp->in_links);
	}
	if (comp->out_links) 
	{
		i = comp->nb_out - 1;
		while(i >= 0) 
		{
			if (comp->out_links[i])
			{
				delete_link(circ, comp->out_links[i]);
			}
			i-=1;
		}
		free(comp->out_links);
	}

	// Deletion of the component et retrait du circuit
	free(comp->coordinates);
	free(comp);

	// Shift of the pointer array to the left
	shift_pointer_array((void**)circ->components, index, circ->component_count);
	
	circ->component_count-=1;
	if (circ->component_count == 0)
	{
		free(circ->components);
		circ->components = NULL;			
	}

	return true;
}

void rename_component(Component* comp, const char* new_name)
{
	if (!comp || !new_name)
	{
		return;
	}

	strncpy(comp->label, new_name, sizeof(comp->label) - 1);

	comp->label[sizeof(comp->label) - 1] = '\0'; 
	printf("▷ Component renamed : %s\n", comp->label);
}

Component* get_component_by_label(const char* given_label, Circuit* circ)
{
	if (!circ || !given_label || !circ->components)
	{
		printf("/!\\ ERROR : Circuit, or component is not found in get_component_by_label() function.\n");
		return NULL;
	}

	for (int i = 0; i < circ->component_count; i++)
	{
		// On vérifie que le composant et son label existent bien
		if (circ->components[i])
		{
			if (strcmp(circ->components[i]->label, given_label) == 0)
			{
				return circ->components[i];
			}
		}
	}

	printf("/!\\ ERROR : Component with label '%s' not found in get_component_by_label() function.\n", given_label);
	return NULL;
}

Component*	invert_source_state(Component* comp)
{
	if (comp->type == SOURCE)
	{
		comp->out_status = !comp->out_status;
		printf("▷ Component status inverted  : %s\n", comp->label);
	}
	return comp;
}