//structures.h
#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Component Component;
typedef struct Link Link;
typedef struct Circuit Circuit;

typedef enum { SOURCE, DIODE, GATE_NOT, GATE_AND, GATE_OR, GATE_NAND, GATE_NOR, GATE_XOR, GATE_NXOR } TypeComponent ;

struct Link {
	Component*		src;
	Component*		dest;
};

struct Component {
	TypeComponent	type;
	int				id;
	int				x;
	int				y;
	int				nb_in;
	int				nb_out;
	bool			out_status;
	Link**			out_links;
	Link**			in_links;
};

struct Circuit {
	int id;
	Component**	components;
	int			component_count;
	Link**		links;
	int			link_count;
};