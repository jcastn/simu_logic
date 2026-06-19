//functions.h
#include "structures.h"

//Prototypes
Component*	create_component(TypeComponent type, int in_nbr, Circuit* circ);
Link*		create_link(Component* src, Component* dest, int port_number, Circuit* circ);
Circuit*	create_circuit(int id);
bool		add_component_to_circuit(Circuit* circ, Component* comp);
bool		add_link_to_circuit(Circuit* circ, Link* link);
bool		delete_link(Circuit* circ, Link* link);
bool		delete_component(Circuit* circ, Component* comp);
void		delete_circuit(Circuit *circ);
Component*	invert_source_state(Component* comp);
void		simulate_circuit(Circuit* circ);


// Eval
bool		generic_eval(Component* comp);
void		propagate_evaluation(Component* comp);



// In/Out 
void		print_circuit_diodes(Circuit* circ);
void		print_circuit_components(Circuit* circ);
int			import_file(char* file_path);


//void		save_circuit(Circuit *circ);
//void		import_circuit(Circuit *circ);

// Helper 
void		shift_pointer_array(void** array, int start_index, int current_count);




// Rearrange circuit
Component*	update_coordinates(Component* comp, int x, int y);
void		rearange_circuit(Circuit* circ);
void		topological_sort(Circuit* circ);
//void    	new_topological_sort(Circuit* circ);
