//functions-eval.c
#include "../../include/prototypes.h"

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

static CompStatus	eval_gate(Component* comp, bool (*operation)(bool, bool), bool accumulator, bool invert_flag)
{
	int counter;
	counter = 0;
	while (counter < comp->nb_in_links)
	{
		// The logic evaluation only happen if the port is linked to a component (the status can be true or false, but not NULL)
		if (comp->in_links[counter] != NULL)
		{
			accumulator = operation(accumulator, read_parent_status(comp, counter));
		}
		counter++;
	}

	// invert_flag is a flag used by the gates who needs a final inversion with the NOT GATE (NAND, NOR and NXOR operations)
	if (invert_flag)
	{
		accumulator = !(accumulator);
	}
	comp->out_status.out = accumulator;
	return comp->out_status;
}

static CompStatus	eval_imply_gate(Component* comp, bool final_not)
{
	comp->out_status.out = !read_parent_status(comp, 0) || read_parent_status(comp, 1);

	if (final_not)
	{
		comp->out_status.out = !comp->out_status.out;
	}

	return comp->out_status;
}

static CompStatus	eval_rgb(Component* comp)
{
	comp->out_status.rgb.r = read_parent_status(comp, 0);
	comp->out_status.rgb.g = read_parent_status(comp, 1);
	comp->out_status.rgb.b = read_parent_status(comp, 2);
	return comp->out_status;
}

static CompStatus	eval_display(Component *comp)
{
	int counter = 0;
	int value = 0;
	comp->out_status.number = 0;
	
	while (counter < comp->nb_in_links)
	{
		if ((comp->in_links[counter] != NULL) && (read_parent_status(comp, counter)))
		{
			value += (1 << counter);
		}
		counter++;
	}

	if (comp->type == DISPLAY_CHAR)
	{
		comp->out_status.character = value;
	}
	else
	{
		comp->out_status.number = value;
	}
	return comp->out_status;
}

CompStatus	component_eval(Component* comp)
{
	if (!comp)
	{
		return (CompStatus){0};
	}

	switch (comp->type)
	{
		case SOURCE : case CONST_OFF : case CONST_ON : 
			return comp->out_status;

		case DIODE : case BUFFER :
			comp->out_status.out = read_parent_status(comp, 0);
			return comp->out_status;

		case DIODE_RGB :
			return eval_rgb(comp);

		case GATE_NOT :
			comp->out_status.out = !read_parent_status(comp, 0);
			return comp->out_status;

		case GATE_AND :
			return eval_gate(comp, op_and, true, false);

		case GATE_NAND :
			return eval_gate(comp, op_and, true, true);

		case GATE_OR :
			return eval_gate(comp, op_or, false, false);

		case GATE_NOR :
			return eval_gate(comp, op_or, false, true);

		case GATE_XOR :
			return eval_gate(comp, op_xor, false, false);

		case GATE_NXOR :
			return eval_gate(comp, op_xor, false, true);

		case GATE_IMPLY :
			return eval_imply_gate(comp, false);

		case GATE_NIMPLY :
			return eval_imply_gate(comp, true);

		case DISPLAY_DEC : case DISPLAY_HEX : case DISPLAY_CHAR :
			return eval_display(comp);

		default:
			comp->out_status.out = false;
			return comp->out_status;
	}
}

// Recursive function to propagate the modification of a binary status (WIP - It will be implemented in a future update)
void	propagate_eval_from_component(Component* comp)
{
	int counter;

	if (!comp)
	{
		return;
	}
	// The evaluation is propagated to the next components
	counter = 0;
	while  (counter < comp->nb_out_links)
	{
		Link* link = comp->out_links[counter];
		if (link && link->dest)
		{
			Component* child_comp = link->dest;
			CompStatus child_old_status = child_comp->out_status;

			component_eval(child_comp);

			// If the state of the child changed, the propagation will continue
			if (child_old_status.raw_value != child_comp->out_status.raw_value)
			{
				propagate_eval_from_component(link->dest);
				//printf(MESS_INFO"Evaluation propagated to %s\n", link->dest->label);

			}
		}
		counter++;
	}
}

