//functions-links.c
#include "../../include/prototypes.h"

// Function to connect components by a Link 
// Parameters : source component, destination component, port number of the destination
Link*	create_link(Circuit* circ, Component* src, int src_port_number, Component* dest, int dest_port_number)
{
	if (!circ || !src || !src->out_ports || !dest)
	{
		printf(MESS_ERROR "Invalid paramters for the function 'create_link()'.\n");
		return NULL;
	}
	
	if (src_port_number < 0 || src_port_number >= src->nb_out_ports)
	{
		printf(MESS_ERROR "Invalid source port index '%d' for component '%s'.\n", PORT_DISPLAY(src_port_number), src->label);
		return NULL;
	}

	if (dest_port_number < 0 || dest_port_number >= dest->nb_in_ports)
	{
		printf(MESS_ERROR "Invalid destination port index '%d' for component '%s'.\n", PORT_DISPLAY(dest_port_number), dest->label);
		return NULL;
	}

	// If the user try to create or connect a link to a port where one link already exist, an error is generated 
	if (dest->in_links[dest_port_number] != NULL)
	{
		printf(MESS_ERROR"A link is already connected on the inbound port '%d' of the component '%s'. Link not created !\n", PORT_DISPLAY(dest_port_number), dest->label);
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
	if (tmp_out == NULL)
	{
		printf(MESS_ERROR"Realloc of out_links failed (function create_link)");
		free(link);
		return NULL;
	}

	src->out_ports[src_port_number]->out_links = tmp_out;
	src->out_ports[src_port_number]->out_links[src->out_ports[src_port_number]->nb_out_links] = link;
	src->out_ports[src_port_number]->nb_out_links++;

	Link** tmp_circ = realloc(circ->links, sizeof(Link*) * (circ->link_count + 1));
	if (tmp_circ == NULL)
	{
		printf(MESS_ERROR"Realloc of circ->links failed (function create_link)");
		free(link);
		return NULL;
	}

	circ->links = tmp_circ;
	circ->links[circ->link_count] = link;
	circ->link_count++;

	dest->in_links[dest_port_number] = link;


	printf(MESS_LINK"Link created : '%s%s (port %d)%s'  → '%s%s (port %d)%s' \n", COMPONENT_MAP[src->type].color, src->label, PORT_DISPLAY(src_port_number), TERMINAL_DEFAULT, COMPONENT_MAP[dest->type].color, dest->label, PORT_DISPLAY(dest_port_number), TERMINAL_DEFAULT);

	// Propagate eval to linked component
	component_eval(dest);
	propagate_eval_from_component(dest);

	return	link;
}


void	delete_link(Circuit* circ, Link* link)
{
	int	counter;
	int	index;

	if (!circ || !link)
	{
		printf(MESS_ERROR"No circuit or link found when trying to delete a link.\n");
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
	
	if (src_port_number < 0 || src_port_number >= src->nb_out_ports)
	{
		printf(MESS_ERROR "Invalid source port index '%d' for component '%s'.\n", PORT_DISPLAY(src_port_number), src->label);
		return NULL;
	}

	if (dest_port_number < 0 || dest_port_number >= dest->nb_in_ports)
	{
		printf(MESS_ERROR "Invalid destination port index '%d' for component '%s'.\n", PORT_DISPLAY(dest_port_number), dest->label);
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
	printf(MESS_INFO"No link found ! (Maybe the port number is invalid ?) \n");
	return NULL;
}