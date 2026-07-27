//structures.h
#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"


typedef struct	Coordinates		Coordinates;
typedef struct	Component		Component;
typedef struct	Link			Link;
typedef struct	TypeCounter		TypeCounter;
typedef struct	Circuit			Circuit;
typedef struct	Model			Model;
typedef struct	CommandMap		CommandMap;
typedef struct	SubCommandMap	SubCommandMap;
typedef struct	ColorStatus		ColorStatus;
typedef	struct	ComponentMap	ComponentMap;
typedef union	CompStatus		CompStatus;


// Enumerations 


// Types of components 
// Please edit COMPONENTS_COUNT and ComponentsNames[] when adding or removing a component ! 
typedef enum {
	SOURCE,
	CONST_ON,
	CONST_OFF,
	DIODE,
	DIODE_RGB,
	DISPLAY_HEX,
	DISPLAY_DEC,
	DISPLAY_CHAR,
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
	bool 			out;
	ColorStatus		rgb;
	uint8_t			number;
	uint8_t			raw_value;
	unsigned char	character;
};

struct	Component
{
	TypeComponent	type;
	int				id;
	Coordinates*	coordinates;
	Link**			in_links;
	int				nb_in_links;
	Link**			out_links;
	int				nb_out_links;
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
	Circuit*		active_circuit;
	Circuit**		circuits;
	int				circuits_count;
	char			label[LABEL_SIZE_NUM+1];
	bool			run_loop;

};

typedef void (*Command)(char* args[MAX_COMMAND_ARGS], Model* model, int word_count);

struct CommandMap
{
	char*						command;
	char*						description;
	Command						function;
	int							needed_args;
	bool						is_alias;
	const struct SubCommandMap*	sub_commands;
	int							sub_commands_count;
};

struct 	SubCommandMap
{
	char*			command;
	Command			function;
	int				needed_args;
	bool			is_alias;
};

struct ComponentMap
{
	const char*		name;
	const char*		color;
};

extern const ComponentMap COMPONENT_MAP[];