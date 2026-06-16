//structures.h
#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Component Component;
typedef struct Link Link;
typedef struct Circuit Circuit;


// Types of components 
// accepted values : (SOURCE, DIODE, BUFFER, GATE_NOT, GATE_AND, GATE_OR, GATE_NAND, GATE_NOR, GATE_XOR, GATE_NXOR) 
typedef enum {						SOURCE,		DIODE,		BUFFER,		GATE_NOT,	GATE_AND,	GATE_OR,	GATE_NAND,		GATE_NOR,	GATE_XOR,	GATE_NXOR } TypeComponent ;

// Translation of TypeComponent enumeration to strings
static char* ComponentNames[] = {	"SOURCE",	"DIODE",	"BUFFER",	"GATE_NOT",	"GATE_AND",	"GATE_OR",	"GATE_NAND",	"GATE_NOR",	"GATE_XOR",	"GATE_NXOR" };


struct Link {
	Component*		src;
	Component*		dest;
};

struct Component {
	TypeComponent	type;
	int				id;
	int				x;
	int				y;
	int				level;
	int				nb_in;
	int				nb_out;
	bool			out_status;
	Link**			out_links;
	Link**			in_links;
};

struct Circuit {
	int 		id;
	Component**	components;
	int			component_count;
	Link**		links;
	int			link_count;
};