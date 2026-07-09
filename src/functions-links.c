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
		free(link);
		return NULL;
	}

	link->src = src;
	link->dest = dest;
	link->port_number = port_number;

	dest->in_links[port_number] = link;

	src->out_links = realloc(src->out_links, sizeof(Link*) * (src->nb_out + 1));
	src->out_links[src->nb_out] = link;
	src->nb_out++;

	// Dynamic enlargement of the array of link pointers
	Link** tmp = realloc(circ->links, sizeof(Link*) * (circ->link_count + 1));
	if (tmp == NULL)
	{
		return NULL;
	}
	circ->links = tmp;
	circ->links[circ->link_count] = link;
	circ->link_count++;

	printf("(▷) Link created : '%s' -> '%s' (port %d)\n", src->label, dest->label, port_number);

	return	link;
}


void	delete_link(Circuit* circ, Link* link)
{
	int	i;
	int	index;

	if (!circ || !link)
	{
		printf(MESS_ERROR"No circuit or link found when trying to delete a link.");
		return;
	}

	// Find link index
	index = -1;
	i = 0;
	while (i < circ->link_count) 
	{
		if (circ->links[i] == link)
		{
			index = i; 
			break;
		}
		i+=1;
	}
	if (index == -1)
	{
		return;
	}

	printf("(▷) Link deleted : '%s' -> '%s' (port %d)\n", link->src->label, link->dest->label, link->port_number);


	// Loop to remove inbound links 
	i = 0;
	while (i < link->dest->nb_in) 
	{
		if (link->dest->in_links[i] == link) 
		{
			link->dest->in_links[i] = NULL;
		}
		i+=1;
	}

	// Loop to remove outbound links 
	i = 0;
	while (i < link->src->nb_out) 
	{
		if (link->src->out_links[i] == link)
		{
			shift_pointer_array((void**)link->src->out_links, i, link->src->nb_out);
			link->src->nb_out--;
			break;
		}
		i+=1;
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
	int i;
	if (!circ || !src || !dest || port_number < 0 || port_number >= dest->nb_in)
	{
		printf(MESS_ERROR"No link found using get_link() function\n");
		return NULL;
	}

	i = 0;
	while(i < src->nb_out)
	{
		if (src->out_links[i]->dest == dest)
		{
			if (src->out_links[i]->port_number == port_number)
			{
				return src->out_links[i];
			}
			else 
			{
				printf(MESS_ERROR"Invalid port number !\n");
				return NULL;
			}
		}
		i++;
	}
	printf(MESS_ERROR"No link found using get_link()\n");
	return NULL;
}