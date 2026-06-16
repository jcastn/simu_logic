//functions.h
#include "structures.h"

//Prototypes
Component*	create_component(int id, TypeComponent type, int in_nbr);
Link*		create_link(Component* src, Component* dest, int port_number);
Circuit*	create_circuit(int id);
bool		add_component_to_circuit(Circuit* circ, Component* comp);
bool		add_link_to_circuit(Circuit* circ, Link* link);
bool		delete_link(Circuit* circ, Link* link);
bool		delete_component(Circuit* circ, Component* comp);
void		delete_circuit(Circuit *circ);
Component*	update_coordinates(Component* comp, int x, int y);

// Eval
bool		generic_eval(Component* comp);



// In/Out 
void		print_circuit_diodes(Circuit* circ);
void		print_circuit_components(Circuit* circ);

//void		save_circuit(Circuit *circ);
//void		import_circuit(Circuit *circ);

// Helper 
void		shift_pointer_array(void** array, int start_index, int current_count);
void		topological_sort(Circuit* circ);