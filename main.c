//main.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "functions.h"
#include "functions.c"

int main(void) 
{
	//Allocation of the components on the heap memory
    Component* source1 = create_component(1, SOURCE, 0);
    Component* source2 = create_component(2, SOURCE, 0);
	Component* source3 = create_component(3, SOURCE, 0);
	Component* source4 = create_component(4, SOURCE, 0);
    Component* and_gate1 = create_component(5, GATE_AND, 4);
	//Component* not_gate1 = create_component(X, GATE_NOT);
	Component* diode1 = create_component(6, DIODE, 1);

    if (!source1 || !source2 || !source3 || !source4 || !and_gate1 || !diode1)
        return (1);

    //Manual activation of the sources
    source1->out_status = true;
    source2->out_status = false;
    source3->out_status = true;
    source4->out_status = true;

    //Dynamic link connexions
    Link* link1 = connect_components(source1, and_gate1, 0);
    Link* link2 = connect_components(source2, and_gate1, 1); // 1 because the 2nd link is wired on the 2nd port of the AND gate
	Link* link3 = connect_components(source3, and_gate1, 2); // 2 because the 3rd link is wired on the 3rd port of the AND gate
	Link* link4 = connect_components(source4, and_gate1, 3); // 3 because the 4th link is wired on the 4th port of the AND gate
	Link* link5 = connect_components(and_gate1, diode1, 0);

    //Evaluation of the logic gates
	generic_eval(and_gate1);
	generic_eval(diode1);


    //Cleaning of the heap
    free(link1);
    free(link2);
	free(link3);
	free(link4);
	free(link5);


	free(source1->out_links);
    free(source1);
    free(source2->out_links);
    free(source2);
    free(source3->out_links);
    free(source3);
    free(source4->out_links);
    free(source4);


	free(and_gate1->in_links); 
    free(and_gate1->out_links); 
    free(and_gate1);

	free(diode1->in_links);
	free(diode1);

    return (0);
}