//functions-eval.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "prototypes.h"
#include "structures.h"

static bool	op_and(bool a, bool b) 
{
	return a && b; 
}
static bool	op_or(bool a, bool b) 
{
	return a || b; 
}
static bool	op_xor(bool a, bool b) 
{
	return a ^ b;
}
static bool	op_imply(bool a, bool b) 
{
	return !a || b;
}

CompStatus	generic_eval(Component* comp)
{
	int		counter;
	bool	result;
	bool	not_flag;
	bool	(*operation)(bool, bool) = NULL;

	if (!comp )
	{
		CompStatus	empty = {0};
		return empty;
	}

	not_flag = false;
	switch (comp->type)
	{
		case DIODE : case BUFFER : case INPUT : case OUTPUT :
			comp->out_status.out = read_parent_status(comp, 0);
			return comp->out_status;
		case GATE_NOT :
			comp->out_status.out = !read_parent_status(comp, 0);
			return comp->out_status;
		case GATE_AND :
			result = true;
			operation = op_and;
			break;
		case GATE_OR :
			result = false;
			operation = op_or;
			break;
		case GATE_NAND :
			result = true;
			operation = op_and;
			not_flag = true;
			break;
		case GATE_NOR :
			result = false;
			operation = op_or;
			not_flag = true;
			break;
		case GATE_XOR :
			result = false;
			operation = op_xor;
			break;
		case GATE_NXOR :
			result = false;
			operation = op_xor;
			not_flag = true;
			break;
		case GATE_IMPLY :
			comp->out_status.out = op_imply(read_parent_status(comp, 0), read_parent_status(comp, 1));
			return comp->out_status;
		case GATE_NIMPLY :
			comp->out_status.out = !op_imply(read_parent_status(comp, 0), read_parent_status(comp, 1));
			return comp->out_status;
		case DIODE_RGB :
			comp->out_status.rgb.r = read_parent_status(comp, 0);
			comp->out_status.rgb.g = read_parent_status(comp, 1);
			comp->out_status.rgb.b = read_parent_status(comp, 2);
			return comp->out_status;
		case SOURCE : 
			return comp->out_status;
		default:
			comp->out_status.out = false;
			return comp->out_status;
	}

	counter = 0;
	while (counter < comp->nb_in)
	{
		// The logic evaluation only happen if the port is linked to a component (the status can be true or false, but not NULL)
		if (comp->in_links[counter] != NULL)
		{
			result = operation(result, read_parent_status(comp, counter));
		}
		counter++;
	}

	// not_flag is a flag used by the gates who needs a final inversion with the NOT GATE (NAND, NOR and NXOR operations)
	if (not_flag)
	{
		result = !result;
	}
	comp->out_status.out = result;
	return comp->out_status;
}