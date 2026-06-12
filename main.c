#include <stdbool.h>
#include <stdio.h>
#include "structures.h"
#include "fonctions.h"

int main(void) 
{
	bool element_source1 = source(1);
	bool element_source2 = source(1);
	
	bool element_link1 = link(element_source1);
	bool element_link2 = link(element_source2);

	bool element_AND1 = gate_and(element_link1, element_link2);

	bool element_link3 = link(element_AND1);

	bool element_NOT1 = gate_not(element_link3);

	bool element_link4 = link(element_NOT1);

	diode(element_link4);


}