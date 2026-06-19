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
	case DIODE :
		comp->out_status = read_parent_status(comp, 0);
		return comp->out_status;
	case BUFFER :
		comp->out_status = read_parent_status(comp, 0);
		return comp->out_status;
	case GATE_NOT :
		comp->out_status = !read_parent_status(comp, 0);
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
	case GATE_IMPLY :
		comp->out_status = op_imply(read_parent_status(comp, 0), read_parent_status(comp, 1));
		return comp->out_status;
	case GATE_NIMPLY :
		comp->out_status = !op_imply(read_parent_status(comp, 0), read_parent_status(comp, 1));
		return comp->out_status;
	default:
		return false;
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
	// final_not is a flag used by the gates needing a final inversion with the NOT GATE (NAND, NOR, NXOR, NIMPLY operations)
	if (final_not)
	{
		result = !result;
	}
	comp->out_status = result;
	return comp->out_status;
}


// Recursive function to propagate the modification of a binary status (WIP - It will be implemented in a future update)
void	propagate_evaluation(Component* comp)
{
	int i;

	if (!comp)
		return;

	// Update of the component out_status state
	bool old_status = comp->out_status;
	generic_eval(comp);
	if (old_status == comp->out_status)
	{
		return;
	}

	// If the status changed, the evaluation is propagated to the nexts components
	i = 0;
	while  (i < comp->nb_out)
	{
		Link* link = comp->out_links[i];
		if (link && link->dest)
		{
			propagate_evaluation(link->dest);
		}
		i+=1;
	}
}

