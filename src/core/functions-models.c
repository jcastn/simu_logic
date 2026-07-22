//functions-models.c
#include "../../include/prototypes.h"

Model*		create_model()
{
	Model* model = malloc(sizeof(Model));
	if (model == NULL)
	{
		return NULL;
	}
	model->circuits = NULL;
	model->circuits_count = 0;
	model->active_circuit = NULL;
	
	return model;
}

void		delete_model(Model* model)
{
	if (!model)
	{
		return;
	}

	if (model->circuits)
	{
		while(model->circuits_count > 0)
		{
			if (model->circuits[0])
			{
				printf("\n"MESS_CIRC"Circuit deleted : \"%s\"\n", model->circuits[0]->label);
				delete_circuit(model, model->circuits[0], true);
			}
		}
	}
	printf("\n"MESS_CIRC"All loaded circuits are deleted.\n\n");
	free(model);
}

void	simulate_model(Model *model)
{
	int counter;
	
	counter = 0;
	while(counter < model->circuits_count)
	{
		simulate_circuit(model->circuits[counter]);
		counter++;
	}
}