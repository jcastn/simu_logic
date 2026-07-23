//functions-helper.c
#include "../../include/prototypes.h"

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
		if (strcmp(type_str, ComponentNames[counter])== 0)
		{
			*found = true;
			return (TypeComponent)counter;
		}
		counter++;
	}
	printf(MESS_SYNTAX"TypeComponent not found !\n");
	return SOURCE;
}


bool			read_parent_status(Component* comp, int port_number)
{
	if (!comp || port_number < 0 || port_number >= comp->nb_in_links)
	{
		return false; 
	}
	if (comp->in_links[port_number] != NULL)
	{
		return comp->in_links[port_number]->src->out_status.out;
	}
	return false;
}

bool			check_path(const char* path){
	if (path == NULL || path[0] == '\0')
	{
		printf(MESS_ERROR"The file path is NULL !");
		return false;
	}

	FILE* file = fopen(path, "r");
	if (file != NULL)
	{
		fclose(file);
		return true;
	}

	printf(MESS_ERROR"The file path '%s' is invalid !", path);
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
void			replace_active_keyword(Model* model, char** args, int arg_count)
{
	// If there's the "active" keyword in the command and there's an active cirucit
	// it will edit the args[2] value to the circuit name of the active circuit
	if ((arg_count >= 3) && (strcmp(args[2], "active") == 0))
	{
		if(model->active_circuit != NULL)
		{
			args[2] = model->active_circuit->label;
		}
		else
		{
			printf(MESS_INFO"There's no active circuits, please use "OPTION_COM(circuit select)" command to set an "KEYWORD_ACTIVE" circuit.\n");
		}
	}
}