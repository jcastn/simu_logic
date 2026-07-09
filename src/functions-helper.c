//functions-helper.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

// Function used to shift the right part content of a pointer array to the left because a pointer was removed from the array (very specific)
void			shift_pointer_array(void** array, int start_index, int current_count)
{
	int i = start_index;
	while (i < current_count - 1)
	{
		array[i] = array[i + 1];
		i++;
	}
}

TypeComponent	string_to_typecomponent(const char* type_str, bool* found)
{
	*found = false;
	int i;

	i = 0;
	while (i < COMPONENTS_COUNT)
	{
		if (strcmp(type_str, ComponentNames[i])== 0)
		{
			*found = true;
			return (TypeComponent)i;
		}
		i++;
	}
	printf(MESS_ERROR"TypeComponent not found (string_to_typecomponent() function)");
	return SOURCE;
}


bool			read_parent_status(Component* comp, int port_number)
{
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