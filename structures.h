//structures.h
#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Component Component;
typedef struct Link Link; 

typedef enum { SOURCE, DIODE, GATE_NOT, GATE_AND, GATE_OR, GATE_NAND, GATE_NOR, GATE_XOR, GATE_NXOR } TypeComponent ;

struct Link {
    Component* src;
    Component* dest;
};

struct Component {
	TypeComponent type;
	int	id;
	int	x;
	int	y;
	bool out_status;
	Link** out_links;
	int nb_out; 
	Link** in_links;
	int nb_in;
};