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

	snprintf(circ->label, sizeof(circ->label), "CIRCUIT_%d", circ->id);

	// If the comp_label is not NULL or if the comp_label is aleready attribuate to another component, 

	
	if ((strcmp(circ_label, "NULL") == 0) || (check_circuit_label(model, circ, circ_label) == true))
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

	printf("(◌) Circuit created : '%s'\n", circ->label);
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

	printf("(◌) Circuit renamed : '%s'\n", circuit->label);

}


// Function to check if a component label already exist in a circuit
bool	check_circuit_label(Model* model, Circuit* circuit, const char* new_label)
{
	if (!model || !circuit || !new_label)
	{
		printf(MESS_ERROR"Problem with the model, the circuit or the new_label. (function check_circuit_label()).\n ");
		return false;
	}

	int i;

	i = 0;
	while(i < model->circuits_count)
	{
		if (model->circuits[i] != circuit && strcmp(model->circuits[i]->label, new_label) == 0)
		{
			return false;
		}
		i++;
	}
	return true;
}









Circuit* get_circuit_by_label(const char* given_label, Model* model)
{
	if (!given_label || !model->circuits)
	{
		printf(MESS_ERROR"Circuit with label '%s' not found.\n", given_label);
		return NULL;
	}

	for (int i = 0; i < model->circuits_count; i++)
	{
		// Check if the component and the label exist
		if (model->circuits[i])
		{
			if (strcmp(model->circuits[i]->label, given_label) == 0)
			{
				return model->circuits[i];
			}
		}
	}

	printf(MESS_ERROR"Circuit with label '%s' not found.\n", given_label);
	return NULL;
}


void	delete_circuit(Circuit *circ)
{
	if (!circ)
	{
		return;
	}

	// Delete all the circuit links 
	while (circ->link_count > 0)
	{
		delete_link(circ, circ->links[0]);
	}

	// Delete all the circuit components 
	while (circ->component_count > 0)
	{
		delete_component(circ, circ->components[0]);
	}

	free(circ);
	printf(MESS_INFO"The circuit is deleted.\n");
	return;
}

void	simulate_circuit(Circuit* circ)
{
	if (!circ)
		return;

	for (int i = 0; i < circ->component_count; i++)
	{
		generic_eval(circ->components[i]);
	}
}