//functions-circuits.c
#include "../../include/prototypes.h"
#include <stdbool.h>

Circuit*	create_circuit(Model* model, const char* label)
{
	if (!model || !label)
	{
		return NULL;
	}

	Circuit* circ = calloc(1, sizeof(Circuit));
	if (circ == NULL)
	{
		return NULL;
	}

	static int next_circ_id = 0;
	circ->id = next_circ_id +=1;
	circ->components = NULL;
	circ->component_count = 0;
	circ->links = NULL ;
	circ->link_count = 0;

	snprintf(circ->label, sizeof(circ->label), "Circuit_%d", circ->id);

	// If the comp_label is not default or if the comp_label is aleready attribuate to another component, 

	
	if (check_circuit_label(model, label) == false)
	{
		printf(MESS_ERROR"A circuit is already named '%s', the created circuit will keep its default label '%s'.\n\n", label, circ->label);
	}
	else if (strcmp(label, "default") == 0)
	{
		printf(MESS_INFO"The circuit will use its default name : '%s'\n", circ->label);
	}
	else
	{
		snprintf(circ->label, sizeof(circ->label), "%s", label);
	}

	Circuit** tmp = realloc(model->circuits, sizeof(Circuit*) * (model->circuits_count + 1));
	if (tmp == NULL)
	{
		printf(MESS_ERROR"Realloc of circuits array failed (Function create_circuit)\n");
		free(circ);
		return NULL;
	}
	model->circuits = tmp;

	model->circuits[model->circuits_count] = circ;
	model->circuits_count += 1; 

	printf(MESS_CIRC"Circuit created : '%s'\n", circ->label);
	return circ;
}

void rename_circuit(Model *model, Circuit* circ, const char* new_label)
{
	if (!model || !circ || !new_label)
	{
		printf(MESS_ERROR "No model, no circuit or no new name find (Function rename_circuit)\n");
		return;
	}

	if (strcmp(circ->label, new_label) == 0)
	{
		printf(MESS_ERROR"The circuit is already named '%s'. Rename operation is aborted.\n", new_label);
		return; 
	}

	if (check_circuit_label(model, new_label) == false)
	{
		printf(MESS_ERROR"A circuit is already named '%s'. Rename operation of circuit '%s' (id:%d) is aborted.\n", new_label, circ->label, circ->id);
		return;
	}

	strncpy(circ->label, new_label, sizeof(circ->label) - 1);
	circ->label[sizeof(circ->label) - 1] = '\0';

	printf(MESS_CIRC"Circuit renamed : '%s'\n", circ->label);

}


// Function to check if a circ label is already used in a circuit
// If it already exist, the function returns false. If it doesn't exit, the function returns true
bool	check_circuit_label(Model* model, const char* new_label)
{
	if (!model || !new_label)
	{
		printf(MESS_ERROR"Problem with the model, the circuit or the new_label. (function check_circuit_label()).\n ");
		return false;
	}

	int counter;

	counter = 0;
	while(counter < model->circuits_count)
	{
		if (strcmp(model->circuits[counter]->label, new_label) == 0)
		{
			return false;
		}
		counter++;
	}
	return true;
}

Circuit* get_circuit_by_label(Model* model, const char* given_label)
{
	int counter;

	if (!given_label || !model || !model->circuits)
	{
		printf(MESS_SYNTAX"Circuit with label '%s' not found\n", given_label);
		return NULL;
	}

	counter = 0;
	while(counter < model->circuits_count)
	{
		// Check if the component and the label exist
		if (model->circuits[counter])
		{
			if (strcmp(model->circuits[counter]->label, given_label) == 0)
			{
				return model->circuits[counter];
			}
		}
		counter++;
	}

	printf(MESS_SYNTAX"Circuit with label '%s' not found, check circuit names with 'circuit list'.\n", given_label);
	return NULL;
}

bool	delete_circuit(Model *model, Circuit *circ, bool flag_free_circuit)
{
	int counter;
	int index; 

	if (!model || !circ)
	{
		printf(MESS_ERROR"Model or circuit not found (Function delete_circuit)\n");
		return false;
	}

	// Get the index of the circuit in the model
	index = -1;
	counter = 0;
	while(counter < model->circuits_count) 
	{
		if (model->circuits[counter] == circ)
		{
			index = counter;
			break;
		}
		counter++;
	}
	if (index == -1)
	{
		printf(MESS_ERROR"Circuit not found in the Model (Function delete_circuit)\n");
		return false;
	}

	// Delete all the circuit components (and the circuit links)
	while (circ->component_count > 0)
	{
		delete_component(circ, circ->components[0]);
	}

	if (flag_free_circuit)
	{
		free(circ);

		shift_pointer_array((void**)model->circuits, index, model->circuits_count);
		model->circuits_count -= 1;

		if (model->circuits_count == 0)
		{
			free(model->circuits);
			model->circuits = NULL;
		}
	}

	return true;
}

void	simulate_circuit(Circuit* circ)
{
	int counter;

	if (!circ)
	{
		return;
	}

	counter = 0;
	while (counter < circ->component_count)
	{
		circ->components[counter]->out_status = generic_eval(circ->components[counter]);
		counter++;
	}
}

int		get_circuit_number_in_model(Model* model,Circuit* circ)
{
	int counter;

	if (!circ || !model)
	{
		return -1;
	}

	counter = 0;
	while (counter < model->circuits_count)
	{
		if (circ == model->circuits[counter])
		{
			return counter;
		}
		counter++;
	}
	return -1;
}

Circuit*	duplicate_circuit(Model* model, Circuit* src_circ, const char* new_label)
{
	int counter;

	if (!model || !src_circ || !new_label)
	{
		return NULL;
	}

	Circuit* dest_circ = create_circuit(model, new_label);
	if (dest_circ == NULL)
	{
		return NULL;
	}

	dest_circ->max_level = src_circ->max_level;

	counter = 0;
	while(counter < src_circ->component_count)
	{
		Component* comp = create_component(dest_circ, src_circ->components[counter]->type, src_circ->components[counter]->label, src_circ->components[counter]->nb_in_links);
		if (comp != NULL)
		{
			comp->coordinates->x = src_circ->components[counter]->coordinates->x;
			comp->coordinates->y = src_circ->components[counter]->coordinates->y;

			if ((src_circ->components[counter]->type == SOURCE) && (src_circ->components[counter]->out_status.out == true))
			{
				invert_source_state(comp);
			}
		}
		counter++;
	}

	counter = 0;
	while(counter < src_circ->link_count)
	{
		int comp_src = get_component_number_in_circuit(src_circ, src_circ->links[counter]->src);
		int comp_dest = get_component_number_in_circuit(src_circ, src_circ->links[counter]->dest);

		if ((comp_src != -1) && (comp_dest != -1))
		{
			create_link(dest_circ->components[comp_src], dest_circ->components[comp_dest], src_circ->links[counter]->port_number, dest_circ);
		}
		counter++;
	}
	return dest_circ;
}

