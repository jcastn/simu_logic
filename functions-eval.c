#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions-prototypes.h"
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

static bool	read_parent_status(Component* comp, int port_number)
{
    if (comp->in_links[port_number] != NULL)
    {
        return comp->in_links[port_number]->src->out_status;
    }
    return false;
}

bool		generic_eval(Component* comp)
{
	int		i;
	bool	val;
	bool	result;
	bool	final_not;
	bool	(*operation)(bool, bool) = NULL;

	if (!comp || comp->type == SOURCE)
	{
        return false;
	}

	final_not = false;
	switch (comp->type)
	{
	case GATE_NOT :
		comp->out_status = !read_parent_status(comp, 0);
		return comp->out_status;
	case DIODE :
		comp->out_status = read_parent_status(comp, 0);
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
		final_not = true;
		break;
	case GATE_NOR :
		result = false;
		operation = op_or;
		final_not = true;
		break;
	case GATE_XOR :
		result = false;
		operation = op_xor;
		break;
	case GATE_NXOR :
		result = false;
		operation = op_xor;
		final_not = true;
		break;
	default:
		return false;
		break;
	}

	i = 0;
	while (i < comp->nb_in)
	{
		// The logic evaluation only happen if the port is linked to a component (the status can be true or false, but not NULL)
		if (comp->in_links[i] != NULL)
        {
			val = read_parent_status(comp, i);
			result = operation(result, val);
		}
		i++;
	}
	// Flag for the gates that needs a final inversion with the NOT GATE (NAND, NOR, NXOR)
	if (final_not)
	{
		result = !result;
	}
	comp->out_status = result;
	return comp->out_status;
}
