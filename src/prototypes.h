//prototypes.h
#pragma once
#include "structures.h"

//Prototypes


//Functions-models 
Model*			create_model();
void			delete_model(Model* model);
void			simulate_model(Model *model);


//Functions-circuits
Circuit*		create_circuit(Model* model, const char* circ_label);
bool			delete_circuit(Model *model, Circuit *circ);
void			rename_circuit(Model *model, Circuit* circuit, const char* new_label);
Circuit*		get_circuit_by_label(const char* label, Model* model);
void			simulate_circuit(Circuit* circ);
bool			check_circuit_label(Model* model, Circuit* circ, const char* new_label);

//Functions-components
Component*		create_component(TypeComponent type, const char* label, int in_nbr, Circuit* circ);
bool			delete_component(Circuit* circ, Component* comp);
void			rename_component(Circuit* circ, Component* comp, const char* new_label);
Component*		get_component_by_label(const char* label, Circuit* circ);
Component*		invert_source_state(Component* comp);
bool			check_component_label(Circuit* circ, Component* comp, const char* new_label);

//Functions-links 
Link*			create_link(Component* src, Component* dest, int port_number, Circuit* circ);
void			delete_link(Circuit* circ, Link* link);


// Eval
CompStatus		generic_eval(Component* comp);
void			propagate_evaluation(Component* comp);



// In/Out 
void			show_components_from_circuit(Circuit* circ);
void			show_components_from_model(Model *model);
void			list_loaded_circuits(Model *model);
void			file_process(char* file_path, FileMode file_mode, Model* model);

// Run-loop
void			run_loop(Model *model);

// Commands
void			exec_command(char* words[MAX_COMMAND_WORDS], Model *model, int word_count);


// Helper 
void			shift_pointer_array(void** array, int start_index, int current_count);
TypeComponent	string_to_typecomponent(const char* type_str, bool* found);
bool			read_parent_status(Component* comp, int port_number);
bool			check_path(const char* path);


// Rearrange circuit
Component*		update_coordinates(Component* comp, int x, int y);
void			rearange_circuit(Circuit* circ);
void			topological_sort(Circuit* circ);
//void			new_topological_sort(Circuit* circ);
