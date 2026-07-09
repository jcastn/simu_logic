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
	int counter;
	if (!model)
	{
		return;
	}

	if (model->circuits)
	{
		counter = 0;
		while(counter < model->circuits_count)
		{
			if (model->circuits[counter])
			{
				delete_circuit(model, model->circuits[counter]);
			}
			counter++;
		}
		free(model->circuits);
	}

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