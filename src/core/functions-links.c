//functions-links.c
#include "../../include/prototypes.h"

// Function to connect components by a Link 
// Parameters : source component, destination component, port number of the destination
Link*	create_link(Component* src, Component* dest, int port_number, Circuit* circ)
{
	if (!circ || !src || !dest || port_number < 0 || port_number >= dest->nb_in_links)
	{
		return NULL;
	}

	// If the user try to connect a link to a port where one already exist, an error is generated 
	if (dest->in_links[port_number] != NULL)
	{
		printf(MESS_ERROR"A link is already connected on the port '%d' of the component '%s'. Link not created !\n", PORT_DISPLAY(port_number), dest->label);
		return NULL;
	}

	Link* link = malloc(sizeof(Link));
	if (!link)
	{
		return NULL;
	}

	link->src = src;
	link->dest = dest;
	link->port_number = port_number;

	// Enlargement of the link pointers array
	Link** tmp_out = realloc(src->out_links, sizeof(Link*) * (src->nb_out_links + 1));
	if (tmp_out == NULL)
	{
		printf(MESS_ERROR"Realloc of out_links failed (function create_link)");
		free(link);
		return NULL;
	}

	Link** tmp_circ = realloc(circ->links, sizeof(Link*) * (circ->link_count + 1));
	if (tmp_circ == NULL)
	{
		printf(MESS_ERROR"Realloc of circ->links failed (function create_link)");
		free(link);
		return NULL;
	}

	src->out_links = tmp_out;
	src->out_links[src->nb_out_links] = link;
	src->nb_out_links++;
	
	circ->links = tmp_circ;
	circ->links[circ->link_count] = link;
	circ->link_count++;

	dest->in_links[port_number] = link;


	printf(MESS_LINK"Link created : '%s' → '%s' (port %d)\n", src->label, dest->label, PORT_DISPLAY(port_number));

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

	// Loop to remove inbound links 
	if ((dest) && (link->port_number >= 0) && (link->port_number < dest->nb_in_links))
	{
		link->dest->in_links[link->port_number] = NULL;
	}

	// Loop to remove outbound links 
	counter = 0;
	if (link->src)
	{
		while (counter < link->src->nb_out_links) 
		{
			if (link->src->out_links[counter] == link)
			{
				shift_pointer_array((void**)link->src->out_links, counter, link->src->nb_out_links);
				link->src->nb_out_links--;
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

Link*	get_link(Circuit* circ, Component* src, Component* dest, int port_number)
{
	int counter;
	if (!circ || !src || !dest || port_number < 0 || port_number >= dest->nb_in_links)
	{
		printf(MESS_ERROR"No link found using get_link() function\n");
		return NULL;
	}

	counter = 0;
	while(counter < src->nb_out_links)
	{
		if ((src->out_links[counter]->dest == dest) && (src->out_links[counter]->port_number == port_number))
		{
			return src->out_links[counter];
		}
		counter++;
	}
	printf(MESS_INFO"No link found ! (Maybe the port number is invalid ?) \n");
	return NULL;
}