//structures.h
#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"


typedef struct	Coordinates Coordinates;
typedef struct	Component Component;
typedef struct	Link Link;
typedef struct	TypeCounter TypeCounter;
typedef struct	Circuit Circuit;
typedef struct	Model Model;
typedef struct	CommandMap CommandMap;
typedef struct	ColorStatus ColorStatus;
typedef union	CompStatus CompStatus;

// Enumerations 


// Types of components 
// accepted values : (SOURCE, DIODE, DIODE_RGB, BUFFER, GATE_NOT, GATE_AND, GATE_OR, GATE_NAND, GATE_NOR, GATE_XOR, GATE_NXOR, GATE_IMPLY, GATE_NIMPLY)
// Please edit COMPONENTS_COUNT and ComponentsNames[] when adding or removing a component ! 
typedef enum {
	SOURCE,
	DIODE,
	DIODE_RGB,
	BUFFER,
	GATE_NOT,
	GATE_AND,
	GATE_OR,
	GATE_XOR,
	GATE_NAND,
	GATE_NOR,
	GATE_NXOR,
	GATE_IMPLY,
	GATE_NIMPLY
} TypeComponent;



// Translation of TypeComponent enumeration to strings
static const char* ComponentNames[] = {
	"SOURCE",
	"DIODE",
	"DIODE_RGB",
	"BUFFER",
	"GATE_NOT",
	"GATE_AND",
	"GATE_OR",
	"GATE_XOR",
	"GATE_NAND",
	"GATE_NOR",
	"GATE_NXOR",
	"GATE_IMPLY",
	"GATE_NIMPLY"
};

typedef enum
{
	STATE_NONE,
	STATE_COMPONENTS,
	STATE_INVERSIONS,
	STATE_LINKS
} ParseState;

typedef enum
{
	IMPORT,
	EXPORT
} FileMode;

typedef void (*Command)(char* args[MAX_COMMAND_ARGS], Model* model, int word_count);




// Structures 
struct	Coordinates 
{
	int				x;
	int				y;
	int				level;
	int				alignment;
};


struct	Link 
{
	Component*		src;	
	Component*		dest;
	int				port_number;
};

struct	ColorStatus
{
	unsigned char	r : 1;
	unsigned char	g : 1;
	unsigned char	b : 1;
};

union CompStatus
{
    bool 		out;
	ColorStatus	rgb;
};


struct	Component
{
	TypeComponent	type;
	int				id;
	Coordinates*	coordinates;
	int				nb_in;
	int				nb_out;
	Link**			out_links;
	Link**			in_links;
	char			label[LABEL_SIZE_NUM+1];
	CompStatus		out_status;	
};

struct	TypeCounter
{
	int				count;
};

struct	Circuit
{
	int 			id;
	Component**		components;
	int				component_count;
	Link**			links;
	int				link_count;
	int				max_level;
	TypeCounter		type_counter[COMPONENTS_COUNT];
	char			label[LABEL_SIZE_NUM+1];
};

struct	Model
{
	int				circuits_count;
	Circuit**		circuits;
	char			label[LABEL_SIZE_NUM+1];
	bool			run_loop;
	Circuit*		active_circuit;
};

struct 	CommandMap
{
	char*			command;
	Command			function;
	int				needed_args;
	bool			is_alias;
};