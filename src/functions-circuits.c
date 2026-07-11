//functions-circuits.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prototypes.h"
#include "structures.h"

Circuit*	create_circuit(Model* model, const char* circ_label)
{
	Circuit* circ = malloc(sizeof(Circuit));
	if (!model || !circ_label || circ == NULL)
	{
		return NULL;
	}

	static int next_circ_id = 0;
	circ->id = next_circ_id +=1;
	circ->components = NULL;
	circ->component_count = 0;
	circ->links = NULL ;
	circ->link_count = 0;

	snprintf(circ->label, sizeof(circ->label), "CIRCUIT_%d", circ->id);

	// If the comp_label is not default or if the comp_label is aleready attribuate to another component, 

	
	if ((strcmp(circ_label, "default") == 0) || (check_circuit_label(model, circ, circ_label) == true))
	{
		snprintf(circ->label, sizeof(circ->label), "%s", circ_label);
	}
	else 
	{
		printf(MESS_ERROR"A circuit is already named '%s', the created circuit will keep its default label '%s'.\n", circ_label, circ->label);
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

void rename_circuit(Model *model, Circuit* circuit, const char* new_label)
{
	if (!model || !circuit || !new_label)
	{
		printf(MESS_ERROR "No model, no circuit or no new name find (Function rename_circuit)\n");
		return;
	}

	if (check_circuit_label(model, circuit, new_label) == false)
	{
		printf(MESS_ERROR"A circuit is already named '%s'. Rename operation of circuit '%s' (id:%d) is aborted.\n", new_label, circuit->label, circuit->id);
		return;
	}

	strncpy(circuit->label, new_label, sizeof(circuit->label) - 1);
	circuit->label[sizeof(circuit->label) - 1] = '\0';

	printf(MESS_CIRC"Circuit renamed : '%s'\n", circuit->label);

}


// Function to check if a component label already exist in a circuit
bool	check_circuit_label(Model* model, Circuit* circuit, const char* new_label)
{
	if (!model || !circuit || !new_label)
	{
		printf(MESS_ERROR"Problem with the model, the circuit or the new_label. (function check_circuit_label()).\n ");
		return false;
	}

	int counter;

	counter = 0;
	while(counter < model->circuits_count)
	{
		if (model->circuits[counter] != circuit && strcmp(model->circuits[counter]->label, new_label) == 0)
		{
			return false;
		}
		counter++;
	}
	return true;
}

Circuit* get_circuit_by_label(const char* given_label, Model* model)
{
	int counter;

	if (!given_label || !model->circuits)
	{
		printf(MESS_ERROR"Circuit with label '%s' not found\n", given_label);
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

	printf(MESS_ERROR"Circuit with label '%s' not found, check circuit names with 'circuit list'.\n", given_label);
	return NULL;
}

bool	delete_circuit(Model *model, Circuit *circ)
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

	free(circ);

	shift_pointer_array((void**)model->circuits, index, model->circuits_count);
	model->circuits_count -= 1;

	if (model->circuits_count == 0)
	{
		free(model->circuits);
		model->circuits = NULL;
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

int		get_circuit_number_in_model(Circuit* circ, Model* model)
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