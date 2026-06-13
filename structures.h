//structures.h
#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Component Component;
typedef struct Link Link; 

typedef enum { SOURCE, DIODE, GATE_AND, GATE_OR, GATE_NOT } TypeComponent ;

struct Link {
    Component* src;
    Component* dest;
};

struct Component {
	TypeComponent type;
	int	id;
	int	x;
	int	y;
	int nb_in;
	Link* in[2];
	bool out;
};
