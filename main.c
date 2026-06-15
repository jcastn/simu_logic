//main.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "functions-prototypes.h"

int main(void) 
{
	// Init of the circuit
	Circuit* circuit1 = create_circuit(1);
	if (!circuit1)
		return (1);

	// Creation of the components
    Component* source1 = create_component(1, SOURCE, 0);
    Component* source2 = create_component(2, SOURCE, 0);
	Component* source3 = create_component(3, SOURCE, 0);
	Component* source4 = create_component(4, SOURCE, 0);
    Component* and_gate1 = create_component(5, GATE_AND, 4);
	Component* diode1 = create_component(6, DIODE, 1);

	if (!source1 || !source2 || !source3 || !source4 || !and_gate1 || !diode1)
	{
		delete_circuit(circuit1);
        return (1);
	}

	// Addition of the components to the circuit
	add_component_to_circuit(circuit1, source1);
	add_component_to_circuit(circuit1, source2);
	add_component_to_circuit(circuit1, source3);
	add_component_to_circuit(circuit1, source4);
	add_component_to_circuit(circuit1, and_gate1);
	add_component_to_circuit(circuit1, diode1);

    // Manual activation of the sources
    source1->out_status = true;
    source2->out_status = true;
    source3->out_status = true;
    source4->out_status = true;

    // Creation of the links and addition to the circuit
    Link* link1 = create_link(source1, and_gate1, 0);
    Link* link2 = create_link(source2, and_gate1, 1);
	Link* link3 = create_link(source3, and_gate1, 2);
	Link* link4 = create_link(source4, and_gate1, 3);
	Link* link5 = create_link(and_gate1, diode1, 0);

	add_link_to_circuit(circuit1, link1);
	add_link_to_circuit(circuit1, link2);
	add_link_to_circuit(circuit1, link3);
	add_link_to_circuit(circuit1, link4);
	add_link_to_circuit(circuit1, link5);

    // Evaluation of the logic gates
	generic_eval(and_gate1);
	generic_eval(diode1);

	// Print diodes status
	print_circuit_diodes(circuit1);
	
    // Cleaning of the circuit
    delete_circuit(circuit1);

    return (0);
}