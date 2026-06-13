//functions.c
#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "functions.h"
#include "structures.h"

// Function to create a component with his ID and his Type (source, diode, AND/OR/NOT gates)
Component* create_component(int id, TypeComponent type)
{
	Component* c = malloc(sizeof(Component));
    if (c == NULL)
	{
        return NULL;
	}
	c->id = id;
	c->type = type;
	c->out = 0;
	c->nb_in = (type == GATE_NOT) ? 1 : (type == SOURCE) ? 0 : 2;
	c->in[0] = NULL;
	c->in[1] = NULL;

	return c;
}

// Function to connect components by a Link 
// Parameters : source component, destination component, input index of the destination
Link* connect_components(Component* src, Component* dest, int port_number)
{
	if (port_number < 0 || port_number >= dest->nb_in)
		return NULL;

	Link* link = malloc(sizeof(Link));
	if (link == NULL)
		return NULL;
	
	link->src = src;
	link->dest = dest;

	dest->in[port_number] = link;

	return link;
}