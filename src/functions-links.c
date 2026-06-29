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
	Link* link = malloc(sizeof(Link));
	if (!link || !circ || !dest || port_number < 0 || port_number >= dest->nb_in)
	{
		return NULL;
		free(link);
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
		return false;
	}
	circ->links = tmp;
	circ->links[circ->link_count] = link;
	circ->link_count++;

	printf("▷ Link created : '%s' -> '%s' (%d)\n", src->label, dest->label, port_number);

	return	link;
}




void	delete_link(Circuit* circ, Link* link)
{
	int	i;
	int	index;

	if (!circ || !link)
	{
		printf("/!\\ ERROR : No circuit or link find when trying to delete a link.");
		return;
	}
	// 1. Trouver l'index du lien
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