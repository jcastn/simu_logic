// include/prototypes.h
#pragma once
#include "structures.h"



//Functions-models 
Model*			create_model					();
bool			delete_model					(Model* model,		bool flag_free_model);
void			simulate_model					(Model* model);


//Functions-circuits
Circuit*		create_circuit					(Model* model,		const char* label);
bool			delete_circuit					(Model* model,		Circuit *circ,			bool flag_free_circuit);
bool			rename_circuit					(Model* model,		Circuit* circ,			const char* new_label);
Circuit*		get_circuit_by_label			(Model* model,		const char* label);
void			simulate_circuit				(Circuit* circ);
bool			check_circuit_label				(Model* model,		const char* new_label);
int				get_circuit_number_in_model		(Model* model,		Circuit* circ);
Circuit*		duplicate_circuit				(Model* model,		Circuit* src_circ, 		const char* new_label);


//Functions-components
Component*		create_component				(Circuit* circ,		TypeComponent type, 	const char* label,		int in_nbr,			int out_nbr);
bool			delete_component				(Circuit* circ,		Component* comp);
bool			rename_component				(Circuit* circ,		Component* comp,		const char* new_label);
Component*		get_component_by_label			(const char* label,	Circuit* circ);
Component*		invert_source_state				(Component* comp);
bool			check_component_label			(Circuit* circ,		Component* comp,		const char* new_label);
void			delete_all_component_links		(Circuit* circ,		Component* comp,		bool free_all);
int				get_component_number_in_circuit	(Circuit* circ,		Component* comp);

//Functions-links 
Link*			create_link						(Circuit* circ, 	Component* src,			int src_port_number,	Component* dest,	int dest_port_number);
void			delete_link						(Circuit* circ, 	Link* link);
Link*			get_link						(Circuit* circ,		Component* src,			int src_port_number,	Component* dest,	int dest_port_number);



// Eval
void			component_eval					(Component* comp);
void			propagate_eval_from_component	(Component* comp);


// In / Out
void			file_process					(char* file_path, 	FileMode file_mode, 	Model* model, 	int circuit_index, 	void (*process_line)(Model* , char*));

// Helper 
void			init_platform					(void);
void			shift_pointer_array				(void** array,			int start_index,	int current_count);
TypeComponent	string_to_typecomponent			(const char* type_str,	bool* found);
bool			read_parent_status				(Component* comp,		int src_port_number);
bool			check_path						(const char* path);
int				string_to_int					(const char* string);
void			replace_active_keyword			(Model* model, char** args, int arg_count);


// Rearrange circuit
Component*		update_coordinates				(Component* comp, int x, int y);
void			rearrange_circuit				(Circuit* circ, bool flag_rearrange_components);
void			topological_sort				(Circuit* circ, bool flag_rearrange_components);
//void			new_topological_sort			(Circuit* circ);
