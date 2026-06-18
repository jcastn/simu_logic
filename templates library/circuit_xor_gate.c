//main.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "functions-prototypes.h"

int main(void) 
{
	Circuit* circuit_xor_gate = create_circuit(2);
	Component* sourceA = create_component(SOURCE, 0, circuit_xor_gate);
    Component* sourceB = create_component(SOURCE, 0, circuit_xor_gate);
	Component* not_gateA = create_component(GATE_NOT, 1, circuit_xor_gate);
	Component* not_gateB = create_component(GATE_NOT, 1, circuit_xor_gate);
	Component* and_gateA = create_component(GATE_AND, 2, circuit_xor_gate);
	Component* and_gateB = create_component(GATE_AND, 2, circuit_xor_gate);
	Component* or_gateA = create_component(GATE_OR, 2, circuit_xor_gate);
	Component* diodeA = create_component(DIODE, 1, circuit_xor_gate);

	invert_source_state(sourceA);
    //invert_source_state(sourceB);



	create_link(sourceA, not_gateA, 0, circuit_xor_gate);
	create_link(sourceB, not_gateB, 0, circuit_xor_gate);
	create_link(sourceA, and_gateB, 0, circuit_xor_gate);
	create_link(sourceB, and_gateA, 0, circuit_xor_gate);
	create_link(not_gateA, and_gateA, 1, circuit_xor_gate);
	create_link(not_gateB, and_gateB, 1, circuit_xor_gate);
	create_link(and_gateA, or_gateA, 0, circuit_xor_gate);
	create_link(and_gateB, or_gateA, 1, circuit_xor_gate);
	create_link(or_gateA, diodeA, 0, circuit_xor_gate);

	rearange_circuit(circuit_xor_gate);
	simulate_circuit(circuit_xor_gate);
	print_circuit_components(circuit_xor_gate);

	delete_circuit(circuit_xor_gate);
    return (0);
}