//prototypes.h
#pragma once
#include "structures.h"


//Functions-models 
Model*			create_model					();
void			delete_model					(Model* model);
void			simulate_model					(Model *model);


//Functions-circuits
Circuit*		create_circuit					(Model* model, const char* circ_label);
bool			delete_circuit					(Model *model, Circuit *circ);
void			rename_circuit					(Model *model, Circuit* circuit, const char* new_label);
Circuit*		get_circuit_by_label			(const char* label, Model* model);
void			simulate_circuit				(Circuit* circ);
bool			check_circuit_label				(Model* model, Circuit* circ, const char* new_label);
int				get_circuit_number_in_model		(Circuit* circ, Model* model);


//Functions-components
Component*		create_component				(TypeComponent type, const char* label, int in_nbr, Circuit* circ);
bool			delete_component				(Circuit* circ, Component* comp);
void			rename_component				(Circuit* circ, Component* comp, const char* new_label);
Component*		get_component_by_label			(const char* label, Circuit* circ);
Component*		invert_source_state				(Component* comp);
bool			check_component_label			(Circuit* circ, Component* comp, const char* new_label);
void			delete_all_component_links		(Circuit* circ, Component* comp, bool free_all);


//Functions-links 
Link*			create_link						(Component* src, Component* dest, int port_number, Circuit* circ);
void			delete_link						(Circuit* circ, Link* link);
Link*			get_link						(Circuit* circ, Component* src, Component* dest, int port_number);



// Eval
CompStatus		generic_eval					(Component* comp);
void			propagate_evaluation			(Component* comp);


// In / Out 
void			file_process					(char* file_path, FileMode file_mode, Model* model, int circuit_index);


// Console-output
void			show_components_from_circuit	(Circuit* circ);
void			show_components_from_model		(Model *model);
void			show_component_links			(Component* comp);
void			list_loaded_circuits			(Model *model);

// Run-loop
void			run_loop						(Model *model);

// Commands
void			exec_command					(char* args[MAX_COMMAND_ARGS], Model *model, int word_count);
void			exec_full_help					(char* command_name, const CommandMap command_map[], int command_count, Model* model);

void			command_circuit					(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void			command_component				(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);
void			command_link					(char* args[MAX_COMMAND_ARGS], Model *model, int arg_count);



// Helper 
void			shift_pointer_array				(void** array, int start_index, int current_count);
TypeComponent	string_to_typecomponent			(const char* type_str, bool* found);
bool			read_parent_status				(Component* comp, int port_number);
bool			check_path						(const char* path);
int				string_to_int					(const char* string);


// Rearrange circuit
Component*		update_coordinates				(Component* comp, int x, int y);
void			rearange_circuit				(Circuit* circ);
void			topological_sort				(Circuit* circ);
//void			new_topological_sort			(Circuit* circ);
