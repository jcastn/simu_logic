//functions.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "prototypes.h"
#include "structures.h"

Circuit*	create_circuit(int id)
{
	Circuit* circ = malloc(sizeof(Circuit));
	if (circ == NULL)
	{
		return NULL;
	}

	circ->id = id;
	circ->components = NULL;
	circ->component_count = 0;
	circ->links = NULL ;
	circ->link_count = 0;
	return circ;
}

bool	add_component_to_circuit(Circuit* circ, Component* comp)
{
	if (!circ || !comp)
	{
		return false;
	}

	// Agrandissement dynamique du tableau de pointeurs de composants
	Component** tmp = realloc(circ->components, sizeof(Component*) * (circ->component_count + 1));
	if (tmp == NULL)
	{
		return false;
	}

	circ->components = tmp;
	circ->components[circ->component_count] = comp;
	circ->component_count++;
	return true;
}

bool	add_link_to_circuit(Circuit* circ, Link* link)
{
	if (!circ || !link)
	{
		return false;
	}
	// Agrandissement dynamique du tableau de pointeurs de liens
	Link** tmp = realloc(circ->links, sizeof(Link*) * (circ->link_count + 1));
	if (tmp == NULL)
	{
		return false;
	}
	circ->links = tmp;
	circ->links[circ->link_count] = link;
	circ->link_count++;
	return true;
}

// Function to create a component with : 
// • its Type (SOURCE, DIODE, NOT / AND / OR / NAND / NOR / XOR / NXOR gates),
// • its number of inbound links (0 if it's a SOURCE ; 1 if it's a NOT_GATE or a BUFFER)
// • the circuit where the component is included
Component*	create_component(TypeComponent type, int in_nbr, Circuit* circ)
{
	int	i;
	static int next_id = 1;

	Component* comp = malloc(sizeof(Component));
	comp->coordinates = malloc(sizeof(Coordinates));
	if (!circ || !comp || (comp->coordinates == NULL) )
	{
		return NULL;
	}

	comp->id = next_id++;
	comp->type = type;
	comp->out_status = false;
	comp->nb_out = 0;
	comp->out_links = NULL;

	if (type == SOURCE)
	{
		comp->nb_in = 0;
	}
	else if ((type == GATE_NOT) || (type == BUFFER ))
	{
		comp->nb_in = 1;
	}
	else if ((type == GATE_IMPLY) || (type == GATE_NIMPLY))
	{
		comp->nb_in = 2;
	}	
	else
	{
		comp->nb_in = in_nbr;
	}

	//Allocation of the table of link pointers 
	if (comp->nb_in > 0)
	{
		comp->in_links = malloc(sizeof(Link*) * comp->nb_in);
		if (comp->in_links == NULL)
		{
			free(comp);
			return NULL;
		}
		i = 0;
		while (i < comp->nb_in)
		{
			comp->in_links[i] = NULL;
			i++;
		}
	}
	else 
	{
		comp->in_links = NULL;
	}

	add_component_to_circuit(circ, comp);

	return comp;
}

// Function to connect components by a Link 
// Parameters : source component, destination component, port number of the destination
Link*	create_link(Component* src, Component* dest, int port_number, Circuit* circ)
{
	if (!circ || !dest || port_number < 0 || port_number >= dest->nb_in)
	{
		return NULL;
	}

	Link* link = malloc(sizeof(Link));
	if (!link)
	{
		return NULL;
		free(link);
	}

	link->src = src;
	link->dest = dest;

	dest->in_links[port_number] = link;

	src->out_links = realloc(src->out_links, sizeof(Link*) * (src->nb_out + 1));
	src->out_links[src->nb_out] = link;
	src->nb_out++;

	add_link_to_circuit(circ, link);
	return	link;
}




bool	delete_link(Circuit* circ, Link* link)
{
	int	i;
	int	index;

	if (!circ || !link)
	{
		return false;
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
		return false;
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
	return true;
}

bool	delete_component(Circuit* circ, Component* comp)
{
	int i;
	int	index;

	if (!circ || !comp)
	{
		return false;
	}

	index = -1;
	i = 0;
	while(i < circ->component_count) 
	{
		if (circ->components[i] == comp)
		{
			index = i;
			break;
		}
		i+=1;
	}
	if (index == -1)
	{
		return false;
	}	


	// Loops to delete links with delete_link()
	if (comp->in_links) 
	{
		i = 0;
		while(i < comp->nb_in) 
		{
			if (comp->in_links[i]) 
			{
				delete_link(circ, comp->in_links[i]);
			}
			i+=1;
		}
		free(comp->in_links);
	}
	if (comp->out_links) 
	{
		i = comp->nb_out - 1;
		while(i >= 0) 
		{
			if (comp->out_links[i])
			{
				delete_link(circ, comp->out_links[i]);
			}
			i-=1;
		}
		free(comp->out_links);
	}

	// Deletion of the component et retrait du circuit
	free(comp->coordinates);
	free(comp);
	// Décalage du tableau de pointeurs vers la gauche
	shift_pointer_array((void**)circ->components, index, circ->component_count);
	
	circ->component_count-=1;
	if (circ->component_count == 0)
	{
		free(circ->components);
		circ->components = NULL;			
	}

	return true;
}

void	delete_circuit(Circuit *circ)
{
    if (!circ)
    {
		return;
	}

	while (circ->link_count > 0)
    {
        delete_link(circ, circ->links[0]);
    }

    while (circ->component_count > 0)
    {
        delete_component(circ, circ->components[0]);
    }

    free(circ);
}

Component*	invert_source_state(Component* comp)
{
	if (comp->type == SOURCE)
	{
		comp->out_status = !comp->out_status;
	}
	return comp;
}


void	simulate_circuit(Circuit* circ)
{
	if (!circ)
		return;

	for (int i = 0; i < circ->component_count; i++)
	{
		generic_eval(circ->components[i]);
	}
}

