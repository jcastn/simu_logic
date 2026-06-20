//main.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "prototypes.h"

int main(void) 
{

	Model* my_model = create_model();

	create_circuit(2, my_model);

	create_component(SOURCE, 0, get_circuit_by_label("CIRCUIT_2", my_model));
    create_component(SOURCE, 0, get_circuit_by_label("CIRCUIT_2", my_model));
	create_component(GATE_NOT, 1, get_circuit_by_label("CIRCUIT_2", my_model));
	create_component(GATE_NOT, 1, get_circuit_by_label("CIRCUIT_2", my_model));
	create_component(GATE_AND, 2, get_circuit_by_label("CIRCUIT_2", my_model));
	create_component(GATE_AND, 2, get_circuit_by_label("CIRCUIT_2", my_model));
	create_component(GATE_OR, 2, get_circuit_by_label("CIRCUIT_2", my_model));
	create_component(DIODE, 1, get_circuit_by_label("CIRCUIT_2", my_model));

	invert_source_state(get_component_by_label("SOURCE_1", get_circuit_by_label("CIRCUIT_2", my_model)));
    //invert_source_state(sourceB);



	create_link(get_component_by_label("SOURCE_1", get_circuit_by_label("CIRCUIT_2", my_model)), get_component_by_label("GATE_NOT_1", get_circuit_by_label("CIRCUIT_2", my_model)), 0, get_circuit_by_label("CIRCUIT_2", my_model));
	create_link(get_component_by_label("SOURCE_2", get_circuit_by_label("CIRCUIT_2", my_model)), get_component_by_label("GATE_NOT_2", get_circuit_by_label("CIRCUIT_2", my_model)), 0, get_circuit_by_label("CIRCUIT_2", my_model));

	create_link(get_component_by_label("SOURCE_1", get_circuit_by_label("CIRCUIT_2", my_model)), get_component_by_label("GATE_AND_2", get_circuit_by_label("CIRCUIT_2", my_model)), 0, get_circuit_by_label("CIRCUIT_2", my_model));
	create_link(get_component_by_label("SOURCE_2", get_circuit_by_label("CIRCUIT_2", my_model)), get_component_by_label("GATE_AND_1", get_circuit_by_label("CIRCUIT_2", my_model)), 0, get_circuit_by_label("CIRCUIT_2", my_model));

	create_link(get_component_by_label("GATE_NOT_1", get_circuit_by_label("CIRCUIT_2", my_model)), get_component_by_label("GATE_AND_1", get_circuit_by_label("CIRCUIT_2", my_model)), 1, get_circuit_by_label("CIRCUIT_2", my_model));
	create_link(get_component_by_label("GATE_NOT_2", get_circuit_by_label("CIRCUIT_2", my_model)), get_component_by_label("GATE_AND_2", get_circuit_by_label("CIRCUIT_2", my_model)), 1, get_circuit_by_label("CIRCUIT_2", my_model));

	create_link(get_component_by_label("SOURCE_2", get_circuit_by_label("CIRCUIT_2", my_model)), get_component_by_label("GATE_NOT_2", get_circuit_by_label("CIRCUIT_2", my_model)), 0, get_circuit_by_label("CIRCUIT_2", my_model));

	create_link(get_component_by_label("GATE_AND_1", get_circuit_by_label("CIRCUIT_2", my_model)), get_component_by_label("GATE_OR_1", get_circuit_by_label("CIRCUIT_2", my_model)), 0, get_circuit_by_label("CIRCUIT_2", my_model));
	create_link(get_component_by_label("GATE_AND_2", get_circuit_by_label("CIRCUIT_2", my_model)), get_component_by_label("GATE_OR_1", get_circuit_by_label("CIRCUIT_2", my_model)), 1, get_circuit_by_label("CIRCUIT_2", my_model));

	create_link(get_component_by_label("GATE_OR_1", get_circuit_by_label("CIRCUIT_2", my_model)), get_component_by_label("DIODE_1", get_circuit_by_label("CIRCUIT_2", my_model)), 0, get_circuit_by_label("CIRCUIT_2", my_model));

	rearange_circuit(get_circuit_by_label("CIRCUIT_2", my_model));
	simulate_circuit(get_circuit_by_label("CIRCUIT_2", my_model));
	print_circuit_components(get_circuit_by_label("CIRCUIT_2", my_model));

	delete_model(my_model);

	// To import a file : 
	//import_file(NULL);
	//import_file("templates library/test.txt");

    return (0);
}