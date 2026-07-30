//functions-components.c
#include "../../include/prototypes.h"



// 	comp->out_ports = calloc(1, sizeof(OutPort));

const ComponentMap COMPONENT_MAP[COMPONENTS_COUNT] = {
	// [comp_type]  = { "name",             color,       in_ports_min,       in_ports_max,  out_ports_min,      out_ports_max},
	[SOURCE]		= { "SOURCE",			TERMINAL_CYAN,			0,					0,				1, 					1},
	[CONST_ON]		= { "CONST_ON",			TERMINAL_CYAN,			0,					0,				1, 					1},
	[CONST_OFF]		= { "CONST_OFF",		TERMINAL_CYAN,			0,					0,				1, 					1},
	[DIODE]			= { "DIODE",			TERMINAL_BLUE,			1,					1,				0, 					0},
	[DIODE_RGB]		= { "DIODE_RGB",		TERMINAL_BLUE,			3,					3,				0, 					0},
	[DISPLAY_HEX]	= { "DISPLAY_HEX",		TERMINAL_YELLOW,		8,					8,				0, 					0},
	[DISPLAY_DEC]	= { "DISPLAY_DEC",		TERMINAL_YELLOW,		8,					8,				0, 					0},
	[DISPLAY_CHAR]	= { "DISPLAY_CHAR",		TERMINAL_YELLOW,		8,					8,				0, 					0},
	[BUFFER]		= { "BUFFER",			TERMINAL_PINK,			1,					1,				1, 					1},
	[GATE_NOT]		= { "GATE_NOT",			TERMINAL_PINK,			1,					1,				1, 					1},
	[GATE_AND]		= { "GATE_AND",			TERMINAL_MAGENTA,		2, 	MAX_COMP_IN_PORTS,				1, 					1},
	[GATE_OR]		= { "GATE_OR",			TERMINAL_MAGENTA,		2,	MAX_COMP_IN_PORTS,				1, 					1},
	[GATE_XOR]		= { "GATE_XOR",			TERMINAL_MAGENTA,		2,	MAX_COMP_IN_PORTS,				1, 					1},
	[GATE_NAND]		= { "GATE_NAND",		TERMINAL_PURPLE,		2,	MAX_COMP_IN_PORTS,				1, 					1},
	[GATE_NOR]		= { "GATE_NOR",			TERMINAL_PURPLE,		2,	MAX_COMP_IN_PORTS,				1, 					1},
	[GATE_NXOR]		= { "GATE_NXOR",		TERMINAL_PURPLE,		2,	MAX_COMP_IN_PORTS,				1, 					1},
	[GATE_IMPLY]	= { "GATE_IMPLY",		TERMINAL_VIOLET,		2,					2,				1, 					1},
	[GATE_NIMPLY]	= { "GATE_NIMPLY",		TERMINAL_VIOLET,		2,					2,				1, 					1},
	[BUS_BUFFER]	= { "BUS_BUFFER",		TERMINAL_PINK,			1,	MAX_COMP_IN_PORTS,				1, 	MAX_COMP_IN_PORTS},
	[BUS_NOT]		= { "BUS_NOT",			TERMINAL_PINK,			1,	MAX_COMP_IN_PORTS,				1, 	MAX_COMP_IN_PORTS}
};

// Function to create a component with : 
// • its Type (SOURCE, DIODE, NOT / AND / OR / NAND / NOR / XOR / NXOR gates),
// • its Label (set it to "default" to auto-generate a new label with the circuit type_counter)
// • its number of inbound links 
// • the circuit where the component is included
Component*	create_component(Circuit* circ, TypeComponent type, const char* comp_label, int in_nbr, int out_nbr)
{
	int	counter;
	static int next_comp_id = 0;

	Component* comp = malloc(sizeof(Component));

	if (!circ || !comp)
	{
		free(comp);
		return NULL;
	}

	// Component values init
	comp->id = next_comp_id += 1;
	comp->type = type;

	// Component out ports init
	if (out_nbr < COMPONENT_MAP[comp->type].nb_out_ports_min)
	{
		comp->nb_out_ports = COMPONENT_MAP[comp->type].nb_out_ports_min;
	}
	else if (out_nbr > COMPONENT_MAP[comp->type].nb_out_ports_max)
	{
		comp->nb_out_ports = COMPONENT_MAP[comp->type].nb_out_ports_max;
	}
	else
	{
		comp->nb_out_ports = out_nbr;
	}

	if (comp->nb_out_ports > 0)
	{
		comp->out_ports = malloc(sizeof(OutPort*) * comp->nb_out_ports);
		if (!comp->out_ports)
		{
			free(comp);
			return NULL;
		}

		counter = 0;
		while (counter < comp->nb_out_ports)
		{
			comp->out_ports[counter] = calloc(1, sizeof(OutPort));
			comp->out_ports[counter]->out_links = NULL;
			comp->out_ports[counter]->nb_out_links = 0;
			counter++;
		}
	}
	else
	{
		comp->out_ports = NULL;
	}
	
	// Component in ports init 
	if (in_nbr < COMPONENT_MAP[comp->type].nb_in_ports_min)
	{
		comp->nb_in_ports = COMPONENT_MAP[comp->type].nb_in_ports_min;
	}
	else if (in_nbr > COMPONENT_MAP[comp->type].nb_in_ports_max)
	{
		comp->nb_in_ports = COMPONENT_MAP[comp->type].nb_in_ports_max;
	}
	else
	{
		comp->nb_in_ports = in_nbr;
	}

	// Component coordinates init
	comp->coordinates.x = 0;
	comp->coordinates.y = 0;
	comp->coordinates.level = 0;
	comp->coordinates.alignment = 0;

	// Component status init 
	switch (comp->type)
	{
		case DIODE_RGB : 
			comp->nb_in_ports = 3;
			comp->status.rgb.r = 0;
			comp->status.rgb.g = 0;
			comp->status.rgb.b = 0;
			break;

		case DISPLAY_DEC : case DISPLAY_HEX : 
			comp->status.number = 0;
			break;
		
		case DISPLAY_CHAR :
			comp->status.character = ' ';
			break;

		case CONST_ON : 
			comp->status.binary = true;
			break;

		default : 
			comp->status.binary = false;
	}

	//In links pointers allocation 
	if (comp->nb_in_ports > 0)
	{
		comp->in_links = malloc(sizeof(Link*) * comp->nb_in_ports);
		if (!comp->in_links)
		{
			free(comp->out_ports);
			free(comp);
			return NULL;
		}
		counter = 0;
		while (counter < comp->nb_in_ports)
		{
			comp->in_links[counter] = NULL;
			counter++;
		}
	}
	else 
	{
		comp->in_links = NULL;
	}

	// Dynamic enlargement of the array containing the in links pointers
	Component** tmp = realloc(circ->components, sizeof(Component*) * (circ->component_count + 1));
	if (!tmp)
	{
		free(comp->in_links);
		free(comp);
		return NULL;
	}
	circ->components = tmp;

	circ->components[circ->component_count] = comp;
	circ->component_count += 1;
	circ->type_counter[type].count += 1;
	

	// Component Label init 

	// By default, we generate a label for the component with the component id
	snprintf(comp->label, sizeof(comp->label), "%s_%d", COMPONENT_MAP[type].name, circ->type_counter[type].count);

	if ((comp_label != NULL) &&
		(strlen(comp_label) >= 1) &&
		(strcmp(comp_label, "default") != 0) &&
		(check_component_label(circ, comp, comp_label) == true))
	{
		snprintf(comp->label, sizeof(comp->label), "%s", comp_label);
	}

	return comp;
}

bool	delete_component(Circuit* circ, Component* comp)
{
	int counter;
	int	index;

	if (!circ || !comp)
	{
		return false;
	}

	// Get the index of the component in the circuit
	index = -1;
	counter = 0;
	while(counter < circ->component_count) 
	{
		if (circ->components[counter] == comp)
		{
			index = counter;
			break;
		}
		counter++;
	}
	if (index == -1)
	{
		return false;
	}	

	// Delete all inbound and outbound links of the component
	delete_all_component_links(circ, comp, true);

	free(comp);

	// Shift of the pointer array to the left
	shift_pointer_array((void**)circ->components, index, circ->component_count);
	
	circ->component_count-=1;
	if (circ->component_count == 0)
	{
		free(circ->components);
		circ->components = NULL;			
	}
	return true;
}

bool rename_component(Circuit* circ, Component* comp, const char* new_label)
{
	if (!circ || !comp || !new_label)
	{
		return false;
	}

	if (check_component_label(circ, comp, new_label) == false)
	{
		return false;
	}

	strncpy(comp->label, new_label, sizeof(comp->label) - 1);
	comp->label[sizeof(comp->label) - 1] = '\0'; 
	return true;
}

// Function to check if a component label already exist in a circuit
bool	check_component_label(Circuit* circ, Component* comp, const char* new_label)
{
	if (!circ || !comp || !new_label)
	{
		return false;
	}

	int counter;

	counter = 0;
	while(counter < circ->component_count)
	{
		if ((circ->components[counter] != comp) && (strcmp(circ->components[counter]->label, new_label) == 0))
		{
			return false;
		}
		counter++;
	}
	return true;
}

// Function to get a component from its label and its circuit
Component* get_component_by_label(const char* given_label, Circuit* circ)
{
	int counter;
	if (!circ || !given_label)
	{
		return NULL;
	}

	counter = 0;
	while(counter < circ->component_count)
	{
		// We check if the component and the label exist
		if (circ->components[counter])
		{
			if (strcmp(circ->components[counter]->label, given_label) == 0)
			{
				return circ->components[counter];
			}
		}
		counter++;
	}
	return NULL;
}

Component*	invert_source_state(Component* comp)
{
	if (!comp)
	{
		return NULL;
	}

	if (comp->type == SOURCE)
	{
		comp->status.binary = !comp->status.binary;
	}
	component_eval(comp);
	propagate_eval_from_component(comp);

	return comp;
}

// Function to delete all inbound and outbound links with delete_link()
void		delete_all_component_links(Circuit* circ, Component* comp, bool free_all)
{
	int counter;
	int counter_bis;

	if (comp->in_links) 
	{
		counter = 0;
		while(counter < comp->nb_in_ports) 
		{
			if (comp->in_links[counter]) 
			{
				delete_link(circ, comp->in_links[counter]);
			}
			counter+=1;
		}
		if (free_all)
		{
			free(comp->in_links);
		}
	}

	if (comp->out_ports) 
	{
		counter = 0;
		while (counter < comp->nb_out_ports)
		{
			if (comp->out_ports[counter])
			{
				counter_bis = comp->out_ports[counter]->nb_out_links - 1;
				while(counter_bis >= 0) 
				{
					if (comp->out_ports[counter]->out_links && comp->out_ports[counter]->out_links[counter_bis])
					{
						delete_link(circ, comp->out_ports[counter]->out_links[counter_bis]);
					}
					counter_bis-=1;
				}
				if (free_all)
				{
					if (comp->out_ports[counter]->out_links)
					{
						free(comp->out_ports[counter]->out_links);
						comp->out_ports[counter]->out_links = NULL;
					}
					free(comp->out_ports[counter]);
					comp->out_ports[counter] = NULL;
				}
			}
			counter++;
		}
		if (free_all)
		{
			free(comp->out_ports);
			comp->out_ports = NULL;
		}
	}
}

int		get_component_number_in_circuit(Circuit* circ, Component* comp)
{
	int counter;

	if (!circ || !comp)
	{
		return -1;
	}

	counter = 0;
	while (counter < circ->component_count)
	{
		if (comp == circ->components[counter])
		{
			return counter;
		}
		counter++;
	}
	return -1;
}