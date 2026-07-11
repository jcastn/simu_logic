//main.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "prototypes.h"

int main(void)
{
	Model* my_model = create_model();

	run_loop(my_model);

	delete_model(my_model);


	return (0);
}