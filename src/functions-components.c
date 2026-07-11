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
	else if (type == DIODE_RGB)
	{
		return 3;
	}
	else if ((type == GATE_IMPLY) || (type == GATE_NIMPLY) || (in_nbr <= 2))
	{
		return 2;
	}
	else if (in_nbr >= COMPONENTS_IN_PORTS)
	{
		return COMPONENTS_IN_PORTS;
	}
	else
	{
		return in_nbr;
	}
}

// Function to create a component with : 
// • its Type (SOURCE, DIODE, NOT / AND / OR / NAND / NOR / XOR / NXOR gates),
// • its Label (set it to "default" to auto-generate a new label with the circuit type_counter)
// • its number of inbound links 
// • the circuit where the component is included
Component*	create_component(TypeComponent type, const char* comp_label, int in_nbr, Circuit* circ)
{
	int	counter;
	static int next_comp_id = 0;

	Component* comp = malloc(sizeof(Component));

	if (!circ || !comp)
	{
		free(comp);
		printf(MESS_ERROR"Circuit or component are missing (Function create_component)\n");
		return NULL;
	}

	comp->coordinates = malloc(sizeof(Coordinates));
	if (!comp->coordinates)
	{
		free(comp);
		return NULL;
	}

	

	//Init of the component values
	comp->id = next_comp_id += 1;
	comp->type = type;
	comp->out_status.out = false;
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
		counter = 0;
		while (counter < comp->nb_in)
		{
			comp->in_links[counter] = NULL;
			counter++;
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
		printf(MESS_ERROR"Realloc of link pointers array failed (function create_component)\n");
		return NULL;
	}
	circ->components = tmp;

	circ->components[circ->component_count] = comp;
	circ->component_count += 1;
	circ->type_counter[type].count += 1;
	
	// By default, we generate a label for the component with the component id
	snprintf(comp->label, sizeof(comp->label), "%s_%d", ComponentNames[type], circ->type_counter[type].count);

	if ((comp_label == NULL) || (strlen(comp_label) == 0))
	{
		printf(MESS_ERROR"There's no value for the component label, so we'll keep the auto-generated label : '%s'\n", comp->label);
	}
	else if (strcmp(comp_label, "default") == 0)
	{
		printf(MESS_INFO"The component will use the auto-generated label : '%s'\n", comp->label);
	}
	else if (check_component_label(circ, comp, comp_label) == false)
	{
		printf(MESS_ERROR"The label '%s' is already used in this circuit. We'll keep the auto-generated label : '%s'\n", comp_label, comp->label);
	}
	else
	{
		snprintf(comp->label, sizeof(comp->label), "%s", comp_label);
	}

	printf(MESS_COMP"%s component created with label '%s', it contains %d inbound ports. \n", ComponentNames[type], comp->label, comp->nb_in);

	return comp;
}

bool	delete_component(Circuit* circ, Component* comp)
{
	int counter;
	int	index;

	if (!circ || !comp)
	{
		printf(MESS_ERROR"Circuit or component not find (Function delete_component)\n");
		return false;
	}

	// Get the index of the component in the circuit
	index = -1;
	counter = 0;
	while(counter < circ->component_count) 
	{
		if (circ->components[counter] == comp)
		{
			index = counter;
			break;
		}
		counter++;
	}
	if (index == -1)
	{
		printf(MESS_ERROR"Component not find in the Circuit (Function delete_component)\n");
		return false;
	}	

	// Delete all inbound and outbound links of the component
	delete_all_component_links(circ, comp, true);

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

void rename_component(Circuit* circ, Component* comp, const char* new_label)
{
	if (!circ || !comp || !new_label)
	{
		return;
	}

	if (check_component_label(circ, comp, new_label) == false)
	{
		printf(MESS_ERROR"In the circuit '%s' (id:%d), a component is already named '%s'.\nRename operation of '%s' aborted.\n", circ->label, circ->id, new_label, comp->label);
		return;
	}

	strncpy(comp->label, new_label, sizeof(comp->label) - 1);
	comp->label[sizeof(comp->label) - 1] = '\0'; 

	printf(MESS_COMP"Component renamed : %s\n", comp->label);
}

// Function to check if a component label already exist in a circuit
bool	check_component_label(Circuit* circ, Component* comp, const char* new_label)
{
	if (!circ || !comp || !new_label)
	{
		printf(MESS_ERROR"Problem with the circuit, the component or the new_label. (function check_component_label()).\n ");
		return false;
	}

	int counter;

	counter = 0;
	while(counter < circ->component_count)
	{
		if ((circ->components[counter] != comp) && (strcmp(circ->components[counter]->label, new_label) == 0))
		{
			return false;
		}
		counter++;
	}
	return true;
}

// Function to get a component from its label and its circuit
Component* get_component_by_label(const char* given_label, Circuit* circ)
{
	int counter;
	if (!circ || !given_label)
	{
		printf(MESS_ERROR"Circuit or label is missing.\n");
		return NULL;
	}

	counter = 0;
	while(counter < circ->component_count)
	{
		// We check if the component and the label exist
		if (circ->components[counter])
		{
			if (strcmp(circ->components[counter]->label, given_label) == 0)
			{
				return circ->components[counter];
			}
		}
		counter++;
	}

	printf(MESS_ERROR"Component with label '%s' not found.\n", given_label);
	return NULL;
}

Component*	invert_source_state(Component* comp)
{
	if (comp->type == SOURCE)
	{
		comp->out_status.out = !comp->out_status.out;
		printf(MESS_COMP"Component status inverted : %s\n", comp->label);
	}
	return comp;
}

// Function to delete all inbound and outbound links with delete_link()
void		delete_all_component_links(Circuit* circ, Component* comp, bool free_all)
{
	int counter; 

	if (comp->in_links) 
	{
		counter = 0;
		while(counter < comp->nb_in) 
		{
			if (comp->in_links[counter]) 
			{
				delete_link(circ, comp->in_links[counter]);
			}
			counter+=1;
		}
		if (free_all)
		{
			free(comp->in_links);
		}
	}

	if (comp->out_links) 
	{
		counter = comp->nb_out - 1;
		while(counter >= 0) 
		{
			if (comp->out_links[counter])
			{
				delete_link(circ, comp->out_links[counter]);
			}
			counter-=1;
		}
		if (free_all)
		{
			free(comp->out_links);
		}
	}

}