//functions-links.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prototypes.h"
#include "structures.h"


// Function to connect components by a Link 
// Parameters : source component, destination component, port number of the destination
Link*	create_link(Component* src, Component* dest, int port_number, Circuit* circ)
{
	if (!circ || !src || !dest || port_number < 0 || port_number >= dest->nb_in)
	{
		return NULL;
	}

	// If the user try to connect a link to a port where one already exist, an error is generated 
	if (dest->in_links[port_number] != NULL)
	{
		printf(MESS_ERROR"A link is already connected on the port '%d' of the component '%s'. Link not created !\n", port_number, dest->label);
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
	Link** tmp_out = realloc(src->out_links, sizeof(Link*) * (src->nb_out + 1));
	if (tmp_out == NULL)
	{
		printf(MESS_ERROR"Realloc of out_links failed (function create_link)");
		free(link);
		return NULL;
	}
	src->out_links = tmp_out;
	src->out_links[src->nb_out] = link;
	src->nb_out++;
	
	dest->in_links[port_number] = link;

	Link** tmp_circ = realloc(circ->links, sizeof(Link*) * (circ->link_count + 1));
	if (tmp_circ == NULL)
	{
		dest->in_links[port_number] = NULL;
		src->nb_out--;
		free(link);
		return NULL;
	}
	circ->links = tmp_circ;
	circ->links[circ->link_count] = link;
	circ->link_count++;

	printf(MESS_LINK"Link created : '%s' → '%s' (port %d)\n", src->label, dest->label, port_number);

	return	link;
}


void	delete_link(Circuit* circ, Link* link)
{
	int	counter;
	int	index;

	if (!circ || !link)
	{
		printf(MESS_ERROR"No circuit or link found when trying to delete a link.");
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

	// Loop to remove inbound links 
	counter = 0;
	while (counter < link->dest->nb_in) 
	{
		if (link->dest->in_links[counter] == link) 
		{
			link->dest->in_links[counter] = NULL;
		}
		counter+=1;
	}

	// Loop to remove outbound links 
	counter = 0;
	while (counter < link->src->nb_out) 
	{
		if (link->src->out_links[counter] == link)
		{
			shift_pointer_array((void**)link->src->out_links, counter, link->src->nb_out);
			link->src->nb_out--;
			break;
		}
		counter+=1;
	}

	free(link);
	shift_pointer_array((void**)circ->links, index, circ->link_count);
	
	circ->link_count--;
	if (circ->link_count == 0)
	{
		free(circ->links); 
		circ->links = NULL;
	}
	return;
}

Link*	get_link(Circuit* circ, Component* src, Component* dest, int port_number)
{
	int counter;
	if (!circ || !src || !dest || port_number < 0 || port_number >= dest->nb_in)
	{
		printf(MESS_ERROR"No link found using get_link() function\n");
		return NULL;
	}

	counter = 0;
	while(counter < src->nb_out)
	{
		if (src->out_links[counter]->dest == dest)
		{
			if (src->out_links[counter]->port_number == port_number)
			{
				return src->out_links[counter];
			}
			else 
			{
				printf(MESS_ERROR"Invalid port number !\n");
				return NULL;
			}
		}
		counter++;
	}
	printf(MESS_ERROR"No link found using get_link()\n");
	return NULL;
}