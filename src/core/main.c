//main.c
#include "../../include/prototypes.h"

int main(void)
{
	init_platform();

	Model* model = create_model();

	run_loop(model);

	delete_model(model, true);

	return (0);
}