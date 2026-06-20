//functions.h
#include "structures.h"

//Prototypes


//Functions-models 
Model*			create_model();
void			delete_model(Model* model);

//Functions-circuits
Circuit*		create_circuit(int id, Model* model);
void			rename_circuit(Circuit* circuit, const char* new_name);
Circuit*		get_circuit_by_label(const char* label, Model* model);
void			delete_circuit(Circuit *circ);
void			simulate_circuit(Circuit* circ);

//Functions-components
Component*		create_component(TypeComponent type, int in_nbr, Circuit* circ);
bool			delete_component(Circuit* circ, Component* comp);
void			rename_component(Component* comp, const char* new_name);
Component*		get_component_by_label(const char* label, Circuit* circ);
Component*		invert_source_state(Component* comp);

//Functions-links 
Link*			create_link(Component* src, Component* dest, int port_number, Circuit* circ);
bool			delete_link(Circuit* circ, Link* link);



// Eval
bool			generic_eval(Component* comp);
void			propagate_evaluation(Component* comp);



// In/Out 
void			print_circuit_diodes(Circuit* circ);
void			print_circuit_components(Circuit* circ);
int				import_file(char* file_path);


//void			save_circuit(Circuit *circ);
//void			import_circuit(Circuit *circ);

// Helper 
void			shift_pointer_array(void** array, int start_index, int current_count);




// Rearrange circuit
Component*		update_coordinates(Component* comp, int x, int y);
void			rearange_circuit(Circuit* circ);
void			topological_sort(Circuit* circ);
//void    		new_topological_sort(Circuit* circ);
