//functions.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions-prototypes.h"
#include "structures.h"

// Function to create a component with his ID, his Type (source, diode, NOT/AND/OR/NAND/NOR/XOR/NXOR gates) and his number of incoming links
Component*	create_component(int id, TypeComponent type, int in_nbr)
{
	int	i;

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
	{
		c->nb_in = 0;
	}
	else if (type == GATE_NOT)
	{
		c->nb_in = 1;
	}
	else
	{
		c->nb_in = in_nbr;
	}

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
Link*	create_link(Component* src, Component* dest, int port_number)
{
	if (port_number < 0 || port_number >= dest->nb_in)
	{
		return NULL;
	}

	Link* link = malloc(sizeof(Link));
	if (link == NULL)
	{
		return NULL;
	}

	link->src = src;
	link->dest = dest;

	dest->in_links[port_number] = link;

	src->out_links = realloc(src->out_links, sizeof(Link*) * (src->nb_out + 1));
	src->out_links[src->nb_out] = link;
	src->nb_out++;

	return	link;
}


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
	// 2. Déconnexion matérielle des composants
	// (Retrait du lien chez le dest)
	i = 0;
	while (i < link->dest->nb_in) 
	{
		if (link->dest->in_links[i] == link) 
		{
			link->dest->in_links[i] = NULL;
		}
		i+=1;
	}
	// (Retrait du lien chez la source avec notre outil)
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

	// 3. Destruction physique et retrait du circuit
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

	// Destruction du composant et retrait du circuit
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

    // 1. Libérer tous les liens (delete_link s'occupe de free chaque link)
    // On consomme le tableau par le début car la taille diminue à chaque appel
    while (circ->link_count > 0)
    {
        delete_link(circ, circ->links[0]);
    }

    // 2. Libérer tous les composants (delete_component s'occupe de free chaque component)
    while (circ->component_count > 0)
    {
        delete_component(circ, circ->components[0]);
    }

    // 3. Libérer le conteneur principal
    free(circ);
}