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
    Component* s1 = create_component(1, SOURCE);
    Component* s2 = create_component(2, SOURCE);
    Component* and_gate1 = create_component(3, GATE_AND);
	Component* not_gate1 = create_component(4, GATE_NOT);

    if (!s1 || !s2 || !and_gate1 || !not_gate1)
        return (1);

    //Manual activation of the sources
    s1->out = true;
    s2->out = true;

    //Dynamic link connexions
    Link* l1 = connect_components(s1, and_gate1, 0);
    Link* l2 = connect_components(s2, and_gate1, 1);
	Link* l3 = connect_components(and_gate1, not_gate1, 0);

    //manual evaluation of the logic gates
    bool in_1 = (and_gate1->in[0] != NULL) ? and_gate1->in[0]->src->out : false;
    bool in_2 = (and_gate1->in[1] != NULL) ? and_gate1->in[1]->src->out : false;
    and_gate1->out = in_1 && in_2;

	bool in_3 = (not_gate1->in[0] != NULL) ? not_gate1->in[0]->src->out : false;
	not_gate1->out = !in_3;
    printf("Output of the NOT gate : %d \n", not_gate1->out);

    //cleaning of the heap
    free(l1);
    free(l2);
	free(l3);
    free(s1);
    free(s2);
    free(and_gate1);
	free(not_gate1);

    return (0);
}


