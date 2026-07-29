// prototypes-commands.h
#pragma once
#include "structures.h"
#include "macros.h"


void					display_help				(char* command_name, char* command_description);
void					command_circuit				(char* args[MAX_COMMAND_ARGS], Model* model, int arg_count);
void					command_component			(char* args[MAX_COMMAND_ARGS], Model* model, int arg_count);
void					command_link				(char* args[MAX_COMMAND_ARGS], Model* model, int arg_count);
void					command_list				(char* args[MAX_COMMAND_ARGS], Model* model, int arg_count);


// Circuit options 

void					command_circuit_create		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_circuit_delete		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_circuit_rename		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_circuit_duplicate	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_circuit_clear		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_circuit_import		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_circuit_export		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_circuit_rearrange	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_circuit_simulate	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_circuit_select		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_circuit_unselect	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);

// Components options 

void					command_component_create	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_component_delete	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_component_rename	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_component_move		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_component_toggle	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);	
void					command_component_set		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_component_show		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);


// Link options 

void					command_link_create			(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_link_delete			(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);


// List options 

void					command_list_circuit		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_list_components		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_list_links			(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);


// Commands 

void					command_help	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_hello	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_reset	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_run		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_quit	(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
