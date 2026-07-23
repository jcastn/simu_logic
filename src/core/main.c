//main.c
#include "../../include/prototypes.h"

int main(void)
{
	init_platform();

	Model* my_model = create_model();

	run_loop(my_model);

	delete_model(my_model);

	return (0);
}