// src/core/helper.c
#include "../../include/prototypes-core.h"

#ifdef _WIN32
#include <windows.h>
#endif

void			init_platform(void)
{
#ifdef _WIN32
	// Force UTF-8 output for Windows
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif
}

// Function used to shift the right part content of a pointer array to the left because a pointer was removed from the array (very specific)
void			shift_pointer_array(void** array, int start_index, int current_count)
{
	int counter = start_index;
	while (counter < current_count - 1)
	{
		array[counter] = array[counter + 1];
		counter++;
	}
}

TypeComponent	string_to_typecomponent(const char* type_str, bool* found)
{
	*found = false;
	int counter;

	counter = 0;
	while (counter < COMPONENTS_COUNT)
	{
		if (strcmp(type_str, COMPONENT_MAP[counter].name) == 0)
		{
			*found = true;
			return (TypeComponent)counter;
		}
		counter++;
	}
	// If the TypeComponent is not found, by default we return a NULL TypeComponent and the *found bool is keeped flase
	return NULL;
}


bool			read_parent_status(Component* comp, int src_port_number)
{
	if (!comp || src_port_number < 0 || src_port_number >= comp->nb_in_ports)
	{
		return false; 
	}
	if (comp->in_links[src_port_number] != NULL)
	{
		Component* src = comp->in_links[src_port_number]->src;
		int out_port = comp->in_links[src_port_number]->src_port_number;
		
		if (src->out_ports && src->nb_out_ports > out_port && src->out_ports[out_port])
		{
			return src->out_ports[out_port]->status.binary;
		}
		
		return src->status.binary;
	}
	return false;
}

bool			check_path(const char* path){
	if (!path || path[0] == '\0')
	{
		return false;
	}

	FILE* file = fopen(path, "r");
	if (file != NULL)
	{
		fclose(file);
		return true;
	}

	return false;
}


int				string_to_int(const char* string)
{
	char *endptr;
	int number;

	number = strtol(string, &endptr, 10);

	if (endptr == string || *endptr != '\0') {
		number =  -1; 
	}
	
	return number;
}

// Function to replace the "active" keyword argument from a command by the name of the active circuit
bool			replace_active_keyword(Model* model, char** args, int arg_count)
{
	// If there's the "active" keyword in the command and if there's an active cirucit
	// it will edit the args[2] value to the circuit name of the active circuit
	if ((arg_count >= 3) && (strcmp(args[2], "active") == 0))
	{
		if(model->active_circuit != NULL)
		{
			args[2] = model->active_circuit->label;
			return true;
		}
		else
		{
			return false;
		}
	}
}