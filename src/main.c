//main.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "prototypes.h"

int main(void) 
{

	Model* my_model = create_model();

	import_file("templates/circuit-xor.txt", my_model);


	print_model_components(my_model);
	simulate_model(my_model);
	rearange_circuit(my_model->circuits[0]);

	print_model_components(my_model);

	delete_model(my_model);


    return (0);
}