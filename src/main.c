//main.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "prototypes.h"

int main(void)
{
	Model* my_model = create_model();

	file_process("templates/circuit-xor.txt", IMPORT, my_model, -1);
	file_process("templates/circuit-rgb.txt", IMPORT, my_model, -1);

	int counter = 0;
	while(counter < my_model->circuits_count)
	{
		rearange_circuit(my_model->circuits[0]);
		counter++;
	}

	//file_process(NULL, EXPORT, my_model, -1);

	//show_components_from_model(my_model);
	
	run_loop(my_model);
	
	//file_process(NULL, EXPORT, my_model, -1);

	

	delete_model(my_model);


	return (0);
}