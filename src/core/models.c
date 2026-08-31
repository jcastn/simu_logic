// src/core/models.c
#include "prototypes-core.h"

Model*		create_model()
{
	Model* model = malloc(sizeof(Model));
	if (!model)
	{
		return NULL;
	}
	model->circuits = NULL;
	model->circuits_count = 0;
	model->active_circuit = NULL;
	
	return model;
}

bool		delete_model(Model* model, bool flag_free_model)
{
	if (!model)
	{
		return false;
	}

	if (model->active_circuit != NULL)
	{
		model->active_circuit = NULL;
	}

	if (model->circuits)
	{
		while(model->circuits_count > 0)
		{
			if (model->circuits[0])
			{
				delete_circuit(model, model->circuits[0], true);
			}
		}
	}

	if (flag_free_model)
	{
		free(model);
	}
	return true;
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
