//main.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "prototypes.h"

int main(void) 
{
	/*
	// Init of the circuit
	Circuit* circuit1 = create_circuit(1);
	if (!circuit1)
		return (1);

	// Creation of the components
    Component* source1 = create_component(SOURCE, 0, circuit1);
    Component* source2 = create_component(SOURCE, 0, circuit1);
	Component* source3 = create_component(SOURCE, 0, circuit1);
    Component* and_gate1 = create_component(GATE_AND, 2, circuit1);
	Component* or_gate1 = create_component(GATE_OR, 3, circuit1);
	Component* diode1 = create_component(DIODE, 1, circuit1);

	
	if (!source1 || !source2 || !source3 || !and_gate1 || !or_gate1 || !diode1)
	{
		delete_circuit(circuit1);
        return (1);
	}

    // Manual activation of the sources
    invert_source_state(source1);
    invert_source_state(source3);


    // Creation of the links and addition to the circuit
    create_link(source1, and_gate1, 0, circuit1);
    create_link(source2, and_gate1, 1, circuit1);
	create_link(and_gate1, or_gate1, 0, circuit1);
	create_link(source2, or_gate1, 1, circuit1);
	create_link(source3, or_gate1, 2, circuit1);
	create_link(or_gate1, diode1, 0, circuit1);

	print_circuit_components(circuit1);
	
	simulate_circuit(circuit1);

	printf("\nAfter the circuit simulation :\n");
	print_circuit_components(circuit1);


	// Addition of components after the simulation
	Component* and_gate2 = create_component(GATE_AND, 2, circuit1);
	Component* source4 = create_component(SOURCE, 2, circuit1);
	Component* diode2 = create_component(DIODE, 1, circuit1);
    invert_source_state(source4);
	create_link(or_gate1, and_gate2, 0, circuit1);
	create_link(source4, and_gate2, 1, circuit1);
	create_link(and_gate2, diode2, 0, circuit1);

	simulate_circuit(circuit1);
	//compute_alignment(circuit1);

	printf("\n\n New components, test of the propagation of the evalution :\n");
	print_circuit_components(circuit1);



	// Print diodes status
	// print_circuit_diodes(circuit1);
	
    // Cleaning of the circuit
    delete_circuit(circuit1);
	*/

	Circuit* circuit2 = create_circuit(2);

	Component* sourceA = create_component(SOURCE, 0, circuit2);
    Component* sourceB = create_component(SOURCE, 0, circuit2);
	Component* not_gateA = create_component(GATE_NOT, 1, circuit2);
	Component* not_gateB = create_component(GATE_NOT, 1, circuit2);

	Component* and_gateA = create_component(GATE_AND, 2, circuit2);
	Component* and_gateB = create_component(GATE_AND, 2, circuit2);

	Component* or_gateA = create_component(GATE_OR, 2, circuit2);
	Component* diodeA = create_component(DIODE, 1, circuit2);

	invert_source_state(sourceA);
    //invert_source_state(sourceB);



	create_link(sourceA, not_gateA, 0, circuit2);
	create_link(sourceB, not_gateB, 0, circuit2);
	create_link(sourceA, and_gateB, 0, circuit2);
	create_link(sourceB, and_gateA, 0, circuit2);
	create_link(not_gateA, and_gateA, 1, circuit2);
	create_link(not_gateB, and_gateB, 1, circuit2);
	create_link(and_gateA, or_gateA, 0, circuit2);
	create_link(and_gateB, or_gateA, 1, circuit2);
	create_link(or_gateA, diodeA, 0, circuit2);

	rearange_circuit(circuit2);
	simulate_circuit(circuit2);
	print_circuit_components(circuit2);

	delete_circuit(circuit2);

	//import_file(NULL);
	import_file("templates library/test.txt");
    return (0);
}