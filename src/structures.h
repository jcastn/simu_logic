//structures.h
#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define COMPONENTS_COUNT 15
#define MAX_COMMAND_WORDS 5
#define APP_VERSION "v0.11"
#define APP_NAME "simu-logic"

typedef struct Coordinates Coordinates;
typedef struct Component Component;
typedef struct Link Link;
typedef struct TypeCounter TypeCounter;
typedef struct Circuit Circuit;
typedef struct Model Model;
typedef struct CommandMap CommandMap;



// Enumerations 


// Types of components 
// accepted values : (SOURCE, DIODE, DIODE_RGB, BUFFER, GATE_NOT, GATE_AND, GATE_OR, GATE_NAND, GATE_NOR, GATE_XOR, GATE_NXOR, GATE_IMPLY, GATE_NIMPLY)
// Please edit COMPONENTS_COUNT and ComponentsNames[] when adding, removing or editing a component ! 
typedef enum {
	SOURCE,
	DIODE,
	DIODE_RGB,
	BUFFER,
	GATE_NOT,
	GATE_AND,
	GATE_OR,
	GATE_NAND,
	GATE_NOR,
	GATE_XOR,
	GATE_NXOR,
	GATE_IMPLY,
	GATE_NIMPLY,
	INPUT,
	OUTPUT,
} TypeComponent;



// Translation of TypeComponent enumeration to strings
static char* ComponentNames[] = {
	"SOURCE",
	"DIODE",
	"DIODE_RGB",
	"BUFFER",
	"GATE_NOT",
	"GATE_AND",
	"GATE_OR",
	"GATE_NAND",
	"GATE_NOR",
	"GATE_XOR",
	"GATE_NXOR",
	"GATE_IMPLY",
	"GATE_NIMPLY",
	"INPUT",
	"OUTPUT",
};

typedef enum {
	STATE_NONE,
	STATE_COMPONENTS,
	STATE_INVERSIONS,
	STATE_LINKS
} ParseState;

typedef enum {
	IMPORT,
	EXPORT
} FileMode;

typedef void (*Command)(char* words[MAX_COMMAND_WORDS], Model* model);


// Structures 


struct	Coordinates {
	int				x;
	int				y;
	int				level;
	int				alignment;
};


struct Link {
	Component*		src;	
	Component*		dest;
	int				port_number;
};

struct Component {
	TypeComponent	type;
	int				id;
	Coordinates*	coordinates;
	int				nb_in;
	int				nb_out;
	bool			out_status;
	Link**			out_links;
	Link**			in_links;
	char			label[16];
};

struct TypeCounter{
	int				count;
};

struct Circuit {
	int 			id;
	Component**		components;
	int				component_count;
	Link**			links;
	int				link_count;
	int				max_level;
	TypeCounter		type_counter[COMPONENTS_COUNT];
	char			label[32];
};

struct Model {
	int				circuits_count;
	Circuit**		circuits;
	char			label[32];
	bool			run_loop;
};

struct CommandMap {
	const char*		name;
	Command 		function;
};