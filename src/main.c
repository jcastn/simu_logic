// src/main.c
#include "../include/prototypes-core.h"
#include "../include/prototypes-cli.h"


int main(void)
{
	init_platform();

	run_loop();

	return (0);
}
