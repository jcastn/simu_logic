//functions-circuits.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prototypes.h"
#include "structures.h"

Circuit*	create_circuit(Model* model)
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

	Circuit** tmp = realloc(model->circuits, sizeof(Circuit*) * (model->circuits_count + 1));
	if (tmp == NULL)
	{
		printf("/!\\ ERROR : Realloc of circuits array failed (Function create_circuit)\n");
		free(circ);
		return NULL;
	}
	model->circuits = tmp;

	model->circuits[model->circuits_count] = circ;
	model->circuits_count += 1; 

	printf("◌ Circuit created : %s\n", circ->label);
	return circ;
}

void rename_circuit(Circuit* circuit, const char* new_name)
{
    if (!circuit || !new_name)
    {
		printf("/!\\ ERROR : No circuit or no new name find (Function rename_circuit)\n");
        return;
    }

    strncpy(circuit->label, new_name, sizeof(circuit->label) - 1);
    
    circuit->label[sizeof(circuit->label) - 1] = '\0'; 
	printf("◌ Circuit renamed : %s\n", circuit->label);

}

Circuit* get_circuit_by_label(const char* given_label, Model* model)
{
    if (!given_label || !model->circuits)
    {
		printf("/!\\ ERROR : Circuit not found in get_circuit_by_label() function.\n");
        return NULL;
    }

    for (int i = 0; i < model->circuits_count; i++)
    {
        // On vérifie que le composant et son label existent bien
        if (model->circuits[i])
        {
            if (strcmp(model->circuits[i]->label, given_label) == 0)
            {
                return model->circuits[i];
            }
        }
    }

    printf("/!\\ ERROR : Component with label '%s' not found in get_component_by_label() function.\n", given_label);
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