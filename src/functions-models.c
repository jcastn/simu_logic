//functions-models.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "prototypes.h"
#include "structures.h"

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
		for (int i = 0; i < model->circuits_count; i++)
		{
			if (model->circuits[i])
			{
				delete_circuit(model->circuits[i]);
			}
		}
		free(model->circuits);
	}

	free(model);
}

void	simulate_model(Model *model)
{
	int i;
	
	i = 0;
	while(i < model->circuits_count)
	{
		simulate_circuit(model->circuits[i]);
		i++;
	}
}