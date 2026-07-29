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


	// If the comp_label is available and not default, the circuit is created, 
	if ((check_circuit_label(model, label) == true) && (strcmp(label, "default") != 0))
	{
		snprintf(circ->label, sizeof(circ->label), "%s", label);
	}

	Circuit** tmp = realloc(model->circuits, sizeof(Circuit*) * (model->circuits_count + 1));
	if (tmp == NULL)
	{
		free(circ);
		return NULL;
	}
	model->circuits = tmp;

	model->circuits[model->circuits_count] = circ;
	model->circuits_count += 1; 

	return circ;
}

bool rename_circuit(Model *model, Circuit* circ, const char* new_label)
{
	if (!model || !circ || !new_label)
	{
		return false;
	}

	if ((strcmp(circ->label, new_label) != 0) && (check_circuit_label(model, new_label) == true))
	{
		strncpy(circ->label, new_label, sizeof(circ->label) - 1);
		circ->label[sizeof(circ->label) - 1] = '\0';
		
		return true;
	}
	return false;
}


// Function to check if a circ label is already used in a circuit
// If it already exist, the function returns false. If it doesn't exit, the function returns true
bool	check_circuit_label(Model* model, const char* new_label)
{
	if (!model || !new_label)
	{
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
	return NULL;
}

bool	delete_circuit(Model *model, Circuit *circ, bool flag_free_circuit)
{
	if (!model || !circ)
	{
		return false;
	}

	int index = get_circuit_number_in_model(model, circ);
	if (index == -1)
	{
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
		Component* comp = circ->components[counter];
		if (comp->coordinates.level == 0)
		{
			component_eval(comp);
			propagate_eval_from_component(comp);
		}
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

	// Duplicate all circuit components
	counter = 0;
	while(counter < src_circ->component_count)
	{
		Component* comp = create_component(dest_circ, src_circ->components[counter]->type, src_circ->components[counter]->label, src_circ->components[counter]->nb_in_ports, src_circ->components[counter]->nb_out_ports);
		if (comp != NULL)
		{
			comp->coordinates.x = src_circ->components[counter]->coordinates.x;
			comp->coordinates.y = src_circ->components[counter]->coordinates.y;

			if ((src_circ->components[counter]->type == SOURCE) && (src_circ->components[counter]->status.binary == true))
			{
				invert_source_state(comp);
			}
		}
		counter++;
	}

	// Duplicate all circuit links
	counter = 0;
	while(counter < src_circ->link_count)
	{
		int comp_src = get_component_number_in_circuit(src_circ, src_circ->links[counter]->src);
		int comp_dest = get_component_number_in_circuit(src_circ, src_circ->links[counter]->dest);

		if ((comp_src != -1) && (comp_dest != -1))
		{
			create_link(dest_circ, dest_circ->components[comp_src], src_circ->links[counter]->src_port_number, dest_circ->components[comp_dest], src_circ->links[counter]->dest_port_number);
		}
		counter++;
	}
	simulate_circuit(dest_circ);
	return dest_circ;
}

