//structures.h
#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define TERMINAL_BLACK		"\e[0;30m"
#define TERMINAL_RED		"\e[0;31m"
#define TERMINAL_GREEN		"\e[0;32m"
#define TERMINAL_YELLOW		"\e[0;33m"
#define	TERMINAL_BLUE		"\e[0;34m"
#define TERMINAL_MAGENTA	"\e[0;35m"
#define TERMINAL_CYAN		"\e[0;36m"
#define TERMINAL_WHITE		"\e[0;37m"
#define	TERMINAL_ORANGE		"\e[38;5;173m"
#define TERMINAL_DEFAULT	"\e[0;0m"

#define COMPONENTS_COUNT	15
#define MAX_COMMAND_WORDS	5

#define APP_NAME			"simu-logic"
#define APP_VERSION			"v0.11.7"
#define APP_PROMPT			"\n" TERMINAL_CYAN "[" APP_NAME " " APP_VERSION "] > "TERMINAL_DEFAULT

#define MESS_ERROR			TERMINAL_RED"\n/!\\ ERROR : "TERMINAL_DEFAULT
#define MESS_SYNTAX			TERMINAL_RED"\n/!\\ WRONG SYNTAX : "TERMINAL_DEFAULT
#define MESS_INFO			TERMINAL_BLUE"\n(i) INFO : "TERMINAL_DEFAULT
#define MESS_TIP			TERMINAL_MAGENTA"\n(⟐) "TERMINAL_DEFAULT

// LABEL_SIZE_NUM is a number coverted to a string, we can use it as "15" or add numbers to it (LABEL_SIZE_NUM + 1 = "16")
#define LABEL_SIZE_NUM		20
#define STRINGIFY(x)		#x
#define TOSTRING(x)			STRINGIFY(x)
#define LABEL_SIZE			TOSTRING(LABEL_SIZE_NUM)

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

typedef void (*Command)(char* words[MAX_COMMAND_WORDS], Model* model, int word_count);




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
	//bool			out_status;
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
	int				needed_words;
};