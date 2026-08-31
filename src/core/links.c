// src/core/links.c
#include "prototypes-core.h"

// Function to connect components by a Link 
// Parameters : source component, destination component, port number of the destination
Link*	create_link(Circuit* circ, Component* src, int src_port_number, Component* dest, int dest_port_number)
{
	if (!circ || !src || !src->out_ports || !dest)
	{
		return NULL;
	}

	if ((src_port_number < 0) && (src_port_number >= src->nb_out_ports) && 
		(dest_port_number < 0) && (dest_port_number >= dest->nb_in_ports) &&
		(dest->in_links[dest_port_number] != NULL))
	{
		return NULL;
	}

	Link* link = malloc(sizeof(Link));
	if (!link)
	{
		return NULL;
	}

	link->src = src;
	link->src_port_number = src_port_number;
	link->dest = dest;
	link->dest_port_number = dest_port_number;

	// Enlargement of the link pointers array
	Link** tmp_out = realloc(src->out_ports[src_port_number]->out_links, sizeof(Link*) * (src->out_ports[src_port_number]->nb_out_links + 1));
	if (!tmp_out)
	{
		free(link);
		return NULL;
	}

	src->out_ports[src_port_number]->out_links = tmp_out;
	src->out_ports[src_port_number]->out_links[src->out_ports[src_port_number]->nb_out_links] = link;
	src->out_ports[src_port_number]->nb_out_links++;

	Link** tmp_circ = realloc(circ->links, sizeof(Link*) * (circ->link_count + 1));
	if (!tmp_circ)
	{
		free(link);
		return NULL;
	}

	circ->links = tmp_circ;
	circ->links[circ->link_count] = link;
	circ->link_count++;

	dest->in_links[dest_port_number] = link;

	// Propagate eval to linked component
	component_eval(dest);
	propagate_eval_from_component(dest);

	return link;
}


void	delete_link(Circuit* circ, Link* link)
{
	int	counter;
	int	index;

	if (!circ || !link)
	{
		return;
	}

	// Find link index
	index = -1;
	counter = 0;
	while (counter < circ->link_count) 
	{
		if (circ->links[counter] == link)
		{
			index = counter; 
			break;
		}
		counter+=1;
	}
	if (index == -1)
	{
		return;
	}

	Component *dest = link->dest;

	// Remove inbound link
	if ((dest) && (link->dest_port_number >= 0) && (link->dest_port_number < dest->nb_in_ports))
	{
		link->dest->in_links[link->dest_port_number] = NULL;
	}

	// Loop to remove outbound links 
	counter = 0;
	if (link->src)
	{
		while (counter < link->src->out_ports[link->src_port_number]->nb_out_links) 
		{
			if (link->src->out_ports[link->src_port_number]->out_links[counter] == link)
			{
				shift_pointer_array((void**)link->src->out_ports[link->src_port_number]->out_links, counter, link->src->out_ports[link->src_port_number]->nb_out_links);
				link->src->out_ports[link->src_port_number]->nb_out_links--;
				break;
			}
			counter+=1;
		}
	}

	free(link);
	shift_pointer_array((void**)circ->links, index, circ->link_count);
	
	circ->link_count--;
	if (circ->link_count == 0)
	{
		free(circ->links); 
		circ->links = NULL;
	}

	if (dest)
	{
		component_eval(dest);
		propagate_eval_from_component(dest);
	}
	return;
}

Link*	get_link(Circuit* circ, Component* src, int src_port_number, Component* dest, int dest_port_number)
{
	int counter;

	if (!circ || !src || !dest)
	{
		return NULL;
	}
	
	if ((src_port_number < 0) && (src_port_number >= src->nb_out_ports) && 
		(dest_port_number < 0) && (dest_port_number >= dest->nb_in_ports) &&
		(dest->in_links[dest_port_number] != NULL))
	{
		return NULL;
	}

	counter = 0;
	while(counter < src->out_ports[src_port_number]->nb_out_links)
	{
		if ((src->out_ports[src_port_number]->out_links[counter]->dest == dest) &&
			(src->out_ports[src_port_number]->out_links[counter]->dest_port_number == dest_port_number))
		{
			return src->out_ports[src_port_number]->out_links[counter];
		}
		counter++;
	}
	return NULL;
}
