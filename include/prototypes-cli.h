// include/prototypes-cli.h
#pragma once
#include "structures.h"
#include "macros.h"



// Run-loop
void 					scan_user_entry					(Model* model, char* command_user);
void					run_loop						();

// User entry
void 					scan_user_entry					(Model* model, char* command_user);

#ifndef DEBUG_MODE
#include "../third_party/linenoise/linenoise.h"
void					auto_complete_entry				(const char *user_entry, linenoiseCompletions *lc);
#endif

// console-output
void					show_components_from_circuit	(Circuit* circ);
void					show_components_from_model		(Model* model);
void					show_links_from_circuit			(Circuit* circ);
void					show_component					(Component* comp);
void					list_loaded_circuits			(Model* model);
void					cli_logger						(const char* message);
void					show_truth_table				(Circuit *circ);



// Commands
void					exec_command					(char* args[MAX_COMMAND_ARGS], Model* model, int word_count);
const	CommandMap*		get_command_map					(int* count);
const	SubCommandMap*	get_sub_command_map				(const char* command_name, int* count);



// Commands 

void					display_help				(char* command_name, char* command_description);
void					command_circuit				(char* args[MAX_COMMAND_ARGS], Model* model, int arg_count);
void					command_component			(char* args[MAX_COMMAND_ARGS], Model* model, int arg_count);
void					command_link				(char* args[MAX_COMMAND_ARGS], Model* model, int arg_count);
void					command_list				(char* args[MAX_COMMAND_ARGS], Model* model, int arg_count);
void					command_help				(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_hello				(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_reset				(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_run					(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void					command_quit				(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);


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
void					command_circuit_show		(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);


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
