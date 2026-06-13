//functions.c
#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "functions.h"
#include "structures.h"

// Function to create a component with his ID, his Type (source, diode, NOT/AND/OR/NAND/NOR/XOR/NXOR gates) and his number of incoming links
Component* create_component(int id, TypeComponent type, int in_nbr)
{
	int i;

	Component* c = malloc(sizeof(Component));
    if (c == NULL)
	{
        return NULL;
	}
	c->id = id;
	c->type = type;
	c->out_status = 0;
	c->nb_out = 0;
	c->out_links = NULL;

	if (type == SOURCE)
		c->nb_in = 0;
	else if (type == GATE_NOT)
		c->nb_in = 1;
	else
		c->nb_in = in_nbr;

		
	//Allocation of the table of link pointers 
	if (c->nb_in > 0)
	{
		c->in_links = malloc(sizeof(Link*) * c->nb_in);
		if (c->in_links == NULL)
		{
			free(c);
			return NULL;
		}
		i = 0;
		while (i < c->nb_in)
		{
			c->in_links[i] = NULL;
			i++;
		}
	}
	else 
	{
		c->in_links = NULL;
	}
	return c;
}

// Function to connect components by a Link 
// Parameters : source component, destination component, port number of the destination
Link* connect_components(Component* src, Component* dest, int port_number)
{
	if (port_number < 0 || port_number >= dest->nb_in)
		return NULL;

	Link* link = malloc(sizeof(Link));
	if (link == NULL)
		return NULL;
	
	link->src = src;
	link->dest = dest;

	dest->in_links[port_number] = link;

	src->out_links = realloc(src->out_links, sizeof(Link*) * (src->nb_out + 1));
    src->out_links[src->nb_out] = link;
    src->nb_out++;

	return link;
}

bool op_and(bool a, bool b) { return a && b; }
bool op_or(bool a, bool b) { return a || b; }
bool op_xor(bool a, bool b) {return a ^ b;}

bool generic_eval(Component* comp)
{
	int		i;
	bool	val;
	bool	res;
	bool	final_not;
	bool	(*operation)(bool, bool) = NULL;

	if (!comp || comp->type == SOURCE)
	{
        return 0;
	}

	final_not = 0;
	switch (comp->type)
	{
	case GATE_NOT :
		comp->out_status = !((comp->in_links[0] != NULL) ? comp->in_links[0]->src->out_status : false);
		return comp->out_status;
	case DIODE :
		comp->out_status = (comp->in_links[0] != NULL) ? comp->in_links[0]->src->out_status : false;
		printf("The diode (id : %d) is set on %d\n", comp->id, comp->out_status);
		return comp->out_status;
	case GATE_AND :
		res = 1;
		operation = op_and;
		break;
	case GATE_OR :
		res = 0;
		operation = op_or;
		break;
	case GATE_NAND :
		res = 1;
		operation = op_and;
		final_not = 1;
		break;
	case GATE_NOR :
		res = 0;
		operation = op_or;
		final_not = 1;
		break;
	case GATE_XOR :
		res = 0;
		operation = op_xor;
		break;
	case GATE_NXOR :
		res = 0;
		operation = op_xor;
		final_not = 1;
		break;
	default:
		return 0;
		break;
	}

	i = 0;
	while (i < comp->nb_in)
	{
		val = (comp->in_links[i] != NULL) ? comp->in_links[i]->src->out_status : false;
		res = operation(res, val);
		i++;
	}
	if (final_not == 1)
	{
		res = !res;
	}
	comp->out_status = res;
	return comp->out_status;
}
