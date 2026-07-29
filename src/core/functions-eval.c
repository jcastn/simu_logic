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

static void	set_out_port_status(Component* comp, int port_number, CompStatus status)
{
	if ((comp) && (comp->out_ports) && (port_number < comp->nb_out_ports) && (comp->out_ports[port_number]))
	{
		comp->out_ports[port_number]->status = status;
	}
	return;
}

static void	eval_gate(Component* comp, bool (*operation)(bool, bool), bool accumulator, bool invert_flag)
{
	int counter;
	counter = 0;
	while (counter < comp->nb_in_ports)
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
	CompStatus result = (CompStatus){.binary = accumulator};
	set_out_port_status(comp, 0, result);
	return;
}

static void	eval_imply_gate(Component* comp, bool final_not)
{
	bool val = !read_parent_status(comp, 0) || read_parent_status(comp, 1);

	if (final_not)
	{
		val = !val;
	}

	CompStatus result = (CompStatus){ .binary = val};
	set_out_port_status(comp, 0, result);
	return;
}

static void	eval_rgb(Component* comp)
{
	comp->status.rgb.r = read_parent_status(comp, 0);
	comp->status.rgb.g = read_parent_status(comp, 1);
	comp->status.rgb.b = read_parent_status(comp, 2);
}

static void	eval_display(Component *comp)
{
	int counter = 0;
	int value = 0;
	comp->status.number = 0;
	
	while (counter < comp->nb_in_ports)
	{
		if ((comp->in_links[counter] != NULL) && (read_parent_status(comp, counter)))
		{
			value += (1 << counter);
		}
		counter++;
	}

	if (comp->type == DISPLAY_CHAR)
	{
		comp->status.character = value;
	}
	else
	{
		comp->status.number = value;
	}
}

static void eval_bus(Component* comp, bool final_not)
{
	CompStatus value;
	int counter = 0;

	while ((counter < comp->nb_in_ports) && (counter < comp->nb_out_ports))
	{
		if (comp->in_links != NULL && comp->in_links[counter] != NULL)
		{
			value = (CompStatus){ .binary = read_parent_status(comp, counter)};
		}
		else
		{
			value = (CompStatus){ .binary = false};
		}

		if (final_not)
		{
			value.binary = !value.binary;
		}

		set_out_port_status(comp, counter, value);
		counter++;
	}
}

void	component_eval(Component* comp)
{
	if (!comp)
	{
		return;
	}

	switch (comp->type)
	{
		case SOURCE : case CONST_OFF : case CONST_ON : 
			set_out_port_status(comp, 0, comp->status);
			break;

		case DIODE :
			comp->status = (CompStatus){.binary = read_parent_status(comp, 0)};
			break;

		case DIODE_RGB :
			eval_rgb(comp);
			break;

		case BUFFER : case BUS_BUFFER :
			eval_bus(comp, false);
			break;

		case GATE_NOT : case BUS_NOT :
			eval_bus(comp, true);
			break;

		case GATE_AND :
			eval_gate(comp, op_and, true, false);
			break;

		case GATE_NAND :
			eval_gate(comp, op_and, true, true);
			break;

		case GATE_OR :
			eval_gate(comp, op_or, false, false);
			break;

		case GATE_NOR :
			eval_gate(comp, op_or, false, true);
			break;

		case GATE_XOR :
			eval_gate(comp, op_xor, false, false);
			break;

		case GATE_NXOR :
			eval_gate(comp, op_xor, false, true);
			break;

		case GATE_IMPLY :
			eval_imply_gate(comp, false);
			break;

		case GATE_NIMPLY :
			eval_imply_gate(comp, true);
			break;

		case DISPLAY_DEC : case DISPLAY_HEX : case DISPLAY_CHAR :
			eval_display(comp);
			break;

		default:
			comp->status = (CompStatus){.binary = false};
			break;
	}
}


static bool has_comp_status_changed(Component* comp, CompStatus old_status, CompStatus* old_ports_status)
{
	if (!comp)
	{
		return false;
	}

	if ((comp->out_ports == NULL) || (comp->nb_out_ports <= 0))
	{
		return old_status.raw_value != comp->status.raw_value;
	}
	else 
	{
		int counter = 0;
		while (counter < comp->nb_out_ports)
		{
			if (comp->out_ports[counter] != NULL)
			{
				if (old_ports_status[counter].raw_value != comp->out_ports[counter]->status.raw_value)
				{
					return true;
				}
			}
			counter++;
		}
		return false;
	}	
}


// Recursive function to propagate the modification of the binary status to the next components
void	propagate_eval_from_component(Component* comp)
{
	int counter_ports;
	int counter_links;
	int counter_child_ports;

	// The evaluation is propagated to the next components only if the component have out_ports
	if (!comp || !comp->out_ports)
	{
		return;
	}
	
	counter_ports = 0;
	while  (counter_ports < comp->nb_out_ports)
	{
		counter_links = 0;
		while  (counter_links < comp->out_ports[counter_ports]->nb_out_links)
		{
			Link* link = comp->out_ports[counter_ports]->out_links[counter_links];
			if (link && link->dest)
			{
				Component* child_comp = link->dest;
				
				CompStatus child_old_status = child_comp->status;
				CompStatus child_old_ports_status[child_comp->nb_out_ports > 0 ? child_comp->nb_out_ports : 1];

				if (child_comp->out_ports != NULL)
				{
					counter_child_ports = 0;

					while(counter_child_ports < child_comp->nb_out_ports)
					{
						if (child_comp->out_ports[counter_child_ports] != NULL)
						{
							child_old_ports_status[counter_child_ports] = child_comp->out_ports[counter_child_ports]->status;
						}
						counter_child_ports++;
					}
				}

				component_eval(child_comp);

				// If the state of the child changed, the propagation will continue
				if (has_comp_status_changed(child_comp, child_old_status, child_old_ports_status))
				{
					propagate_eval_from_component(child_comp);
					//printf(MESS_INFO"Evaluation propagated to %s\n", link->dest->label);

				}
			}
			counter_links++;
		}
		counter_ports++;
	}
	
}

