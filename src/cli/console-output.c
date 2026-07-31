// src/cli/console-output.c
#include "../../include/prototypes-cli.h"

static void		compute_color(ColorStatus color, const char** state_color, const char** state_text)
{
	if (color.r && color.g && color.b)
	{
		*state_color = TERMINAL_WHITE;
		*state_text = "ON (white)"; 
	}
	else if (color.r && color.g)
	{
		*state_color = TERMINAL_YELLOW;
		*state_text = "ON (yellow)";
	}
	else if (color.r && color.b)
	{
		*state_color = TERMINAL_MAGENTA;
		*state_text = "ON (magenta)";
	}
	else if (color.g && color.b)
	{
		*state_color = TERMINAL_CYAN;
		*state_text = "ON (cyan)";
	}
	else if (color.r)
	{
		*state_color = TERMINAL_RED;
		*state_text = "ON (red)";
	}
	else if (color.g)
	{
		*state_color = TERMINAL_GREEN;
		*state_text = "ON (green)";
	}
	else if (color.b)
	{
		*state_color = TERMINAL_BLUE;
		*state_text = "ON (blue)";
	}
	else
	{
		*state_color = TERMINAL_RED;
		*state_text = "OFF";
	}	
}

static void		compute_number(Component* comp, const char** state_color, const char** state_text, char* state_text_buffer)
{
	*state_color = COMPONENT_MAP[comp->type].color;
	if (comp->type == DISPLAY_DEC)
	{
		snprintf(state_text_buffer, STATE_SIZE_NUM, "Value : %d", comp->status.number);
	}
	else if (comp->type == DISPLAY_HEX)
	{
		snprintf(state_text_buffer, STATE_SIZE_NUM, "Value : %X", comp->status.number);
	}
	else if (comp->type == DISPLAY_CHAR)
	{
		if (comp->status.character >= 32 && comp->status.character <= 126)
		{
			snprintf(state_text_buffer, STATE_SIZE_NUM, "Char : '%c'", comp->status.character);
		}
		else
		{
			*state_color = TERMINAL_RED;
			snprintf(state_text_buffer, STATE_SIZE_NUM, "Char : UNDEF");
		}	
	}
	else 
	{
		*state_color = TERMINAL_RED;
		snprintf(state_text_buffer, STATE_SIZE_NUM, "Value : ERROR");
	}
	*state_text = state_text_buffer;
}

static void		compute_binary(CompStatus status, const char** state_color, const char** state_text)
{
	if (status.binary == true)
	{
		*state_color = TERMINAL_GREEN;
		*state_text = "ON";
	}
	else 
	{
		*state_color = TERMINAL_RED;
		*state_text = "OFF";		
	}
}

static void		get_component_out_status(Component* comp, const char** state_color, const char** state_text)
{
	static char state_text_buffer[STATE_SIZE_NUM];

	if ((comp->nb_out_ports == 0) || (!comp->out_ports))
	{
		if (comp->type == DIODE_RGB)
		{
			compute_color(comp->status.rgb, state_color, state_text);
		}
		else if ((comp->type == DISPLAY_DEC) || (comp->type == DISPLAY_HEX) || (comp->type == DISPLAY_CHAR))
		{
			compute_number(comp, state_color, state_text, state_text_buffer);
		}
		else if(comp->type == DIODE)
		{
			compute_binary(comp->status, state_color, state_text);
		}
		else
		{
			*state_color = TERMINAL_GRAY;
			*state_text = "Unknown state";
		}
	}
	else
	{
		if (comp->out_ports[0] != NULL)
		{
			compute_binary(comp->out_ports[0]->status, state_color, state_text);
		}
		else
		{
			compute_binary(comp->status, state_color, state_text);
		}
	}

	return;
}


void	show_components_from_circuit(Circuit* circ)
{
	int counter;
	printf(	"\nCircuit %d ("TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT"):\n%d Components and %d Links on %d Levels, \n", circ->id, circ->label, circ->component_count, circ->link_count, circ->max_level);
	printf(	"•----------------------•----------------------•----------------•--------•-------•-------•--------•--------•------------------•\n"
			"| Component Label      | Component Type       | State          | ID     | Level | Align | x      | y      | Ports            |\n"
			"•----------------------•----------------------•----------------•--------•-------•-------•--------•--------•------------------•\n");

	if (circ->component_count == 0)
	{
		printf("| "TERMINAL_GRAY"(empty)"TERMINAL_DEFAULT"              |                      |                |        |       |       |        |        |                  |\n");
	}

	counter = 0;
	while(counter < circ->component_count)
	{
		const char* component_color;
		Component* comp = circ->components[counter];
		component_color = COMPONENT_MAP[comp->type].color;

		// Out-Status State
		const char* state_color;
		const char* state_text;

		get_component_out_status(comp, &state_color, &state_text);
		
		printf("| %s%-"LABEL_SIZE"s" TERMINAL_DEFAULT " | %s%-"LABEL_SIZE"s" TERMINAL_DEFAULT " | %s%-"STATE_SIZE"s" TERMINAL_DEFAULT " | %-6d | %-5d | %-5d | %-6d | %-6d | In:%-4d Out:%-4d |\n", 
			component_color, comp->label,
			component_color, COMPONENT_MAP[comp->type].name,
			state_color, state_text,
			comp->id, 
			comp->coordinates.level, 
			comp->coordinates.alignment, 
			comp->coordinates.x, 
			comp->coordinates.y,
			comp->nb_in_ports,
			comp->nb_out_ports);
		counter++;
	}
	printf("•----------------------•----------------------•----------------•--------•-------•-------•--------•--------•------------------•\n");
}


void	show_links_from_circuit(Circuit* circ)
{
	(void)circ;	
	int counter_comps;
	int counter_ports;
	int counter_links;
	const char* component_color;
	
	if (circ->link_count == 0)
	{
		printf(MESS_INFO"The circuit '%s' contains no links !\n", circ->label);
		return;
	}

	printf(	"\nCircuit %d ("TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT"):\n%d Links and %d Components on %d Levels, \n", circ->id, circ->label, circ->link_count, circ->component_count, circ->max_level);

	counter_comps = 0;
	while (counter_comps < circ->component_count)
	{
		counter_ports = 0;
		while (counter_ports < circ->components[counter_comps]->nb_out_ports)
		{
			Component* comp = circ->components[counter_comps];
			component_color = COMPONENT_MAP[comp->type].color;
			if (comp->out_ports[counter_ports]->nb_out_links != 0)
			{
				counter_links = 0;
				printf("\n'%s%s"TERMINAL_DEFAULT"' is linked to :\n", component_color, comp->label);
				while(counter_links < comp->out_ports[counter_ports]->nb_out_links)
				{
					component_color = COMPONENT_MAP[comp->out_ports[counter_ports]->out_links[counter_links]->dest->type].color;
					printf(" ⤷ '%s%s"TERMINAL_DEFAULT"' on port %d\n", component_color, comp->out_ports[counter_ports]->out_links[counter_links]->dest->label, PORT_DISPLAY(comp->out_ports[counter_ports]->out_links[counter_links]->dest_port_number));
					counter_links++;
				}
			}
			counter_ports++;
		}
		counter_comps++;
	}


}

void	show_components_from_model(Model *model)
{
	if (model->circuits_count == 0)
	{
		printf(MESS_INFO"There's no circuit in this model !\n");
	}
	int counter = 0;

	while (counter<model->circuits_count)
	{
		show_components_from_circuit(model->circuits[counter]);
		counter++;
	}
}

void	show_component(Component* comp)
{
	int			counter;
	int			counter_out_ports;
	int			counter_out_links;
	char		inbound[64];
	char		outbound[64];
	const char*	component_color;
	const char*	state_color;
	const char*	state_text;

	component_color = COMPONENT_MAP[comp->type].color;
	get_component_out_status(comp, &state_color, &state_text);

	printf(	"\nDetails about component %s%s"TERMINAL_DEFAULT" :"
			"\n▻ Type   : %s%s"TERMINAL_DEFAULT
			"\n▻ ID     : %d"
			"\n▻ x      : %d"
			"\n▻ y      : %d"
			"\n▻ Level  : %d"
			"\n▻ Align  : %d"
			"\n▻ State  : %s%s"TERMINAL_DEFAULT"\n\n",
			component_color, comp->label, 
			component_color, COMPONENT_MAP[comp->type].name, 
			comp->id,
			comp->coordinates.x, comp->coordinates.y,
			comp->coordinates.level, comp->coordinates.alignment,
			state_color, state_text);

	printf(	"•-------------------------------•-------------------------------•\n"
			"| Inbound links                 | Outbound links                |\n"
			"•-------------------------------•-------------------------------•\n");

	counter = 0;
	counter_out_ports = 0;
	counter_out_links = -1;
	while ((counter == 0) || (counter < comp->nb_in_ports) || ((comp->out_ports != NULL) && (counter_out_ports < comp->nb_out_ports)))
	{
		// Left column (inbound links)
		// If there's an inbound link (or space for an inbound link), we display it
		if ((counter < comp->nb_in_ports) && (comp->in_links != NULL))
		{
			if (comp->in_links[counter] != NULL)
			{
				component_color = COMPONENT_MAP[comp->in_links[counter]->src->type].color;
				snprintf(inbound, sizeof(inbound), "Port %d : %s%-"LABEL_SIZE"s"TERMINAL_DEFAULT, PORT_DISPLAY(counter), component_color, comp->in_links[counter]->src->label);
			}
			else
			{
				snprintf(inbound, sizeof(inbound), "Port %d : "TERMINAL_GRAY"(empty)"TERMINAL_DEFAULT"             ", PORT_DISPLAY(counter));
			}
		}
		// If there's no inbound link, we display it 
		else
		{
			if ((counter == 0) && (comp->nb_in_ports == 0))
			{
				snprintf(inbound, sizeof(inbound), TERMINAL_GRAY"(empty - no inbound ports)   "TERMINAL_DEFAULT);
			}
			else 
			{
				snprintf(inbound, sizeof(inbound), "                             ");
			}
		}

		// Right column (Outbound)

		// If there's no outbound ports
		if (!comp->out_ports || counter_out_ports >= comp->nb_out_ports)
		{
			if (counter == 0 && !comp->out_ports)
			{
				snprintf(outbound, sizeof(outbound), TERMINAL_GRAY"(empty - no outbound ports)  "TERMINAL_DEFAULT);
			}
			else
			{
				snprintf(outbound, sizeof(outbound), "                             ");
			}
		}
		// If there's an outbound ports, we display the outbound port number
		else if (counter_out_links == -1)
		{
			snprintf(outbound, sizeof(outbound), "Port %d :                     ", PORT_DISPLAY(counter_out_ports));
			
			// If this port as no outbound link, at the next tun it will show the no outbound link message
			if (comp->out_ports[counter_out_ports]->nb_out_links == 0)
			{
				counter_out_links = -2; 
			}
			else
			{
				counter_out_links = 0; 
			}
		}
		// If there's no link at the outbound port 
		else if (counter_out_links == -2)
        {
            snprintf(outbound, sizeof(outbound), TERMINAL_GRAY"↳ (empty - no outbound link) "TERMINAL_DEFAULT);
            counter_out_links = -1;
            counter_out_ports++;
        }
		//If there's a link at the outbound port
		else if (counter_out_links < comp->out_ports[counter_out_ports]->nb_out_links)
		{
			Link* current_link = comp->out_ports[counter_out_ports]->out_links[counter_out_links];
			if (current_link != NULL && current_link->dest != NULL)
			{
				component_color = COMPONENT_MAP[current_link->dest->type].color;
				snprintf(outbound, sizeof(outbound), "↳ (%d) %s%-"LABEL_SIZE"s"TERMINAL_DEFAULT"   ", PORT_DISPLAY(counter_out_links), component_color, current_link->dest->label);
			}
			else
			{
				snprintf(outbound, sizeof(outbound), TERMINAL_RED"(unknown)"TERMINAL_DEFAULT"                   ");
			}

			counter_out_links++;

			if (counter_out_links >= comp->out_ports[counter_out_ports]->nb_out_links)
			{
				counter_out_links = -1;
				counter_out_ports++;
			}
		}

		counter++;
		printf("| %29s | %29s |\n", inbound, outbound);
	}

	printf("•-------------------------------•-------------------------------•\n");
}

void list_loaded_circuits(Model *model)
{
	int counter;
	int active_circuit_id = -1 ;
	
	if (model->circuits_count == 0)
	{
		printf(MESS_INFO"No circuits are currently loaded\n");
		return;
	}

	if (model->active_circuit != NULL)
	{
		active_circuit_id = model->active_circuit->id;
	}

	counter = 0;
	while (counter < model->circuits_count){
		printf(	"\nLoaded circuits :\n\n"
				"•----------------•----------------------•------------•------------•\n"
				"| Circuit ID     | Circuit Label        | Components | Links      |\n"
				"•----------------•----------------------•------------•------------•\n");
		while(counter < model->circuits_count)
		{
			if (active_circuit_id == model->circuits[counter]->id)
			{
				printf("| "TERMINAL_GREEN"Circuit %-6d"TERMINAL_DEFAULT" | "TERMINAL_GREEN"%-"LABEL_SIZE"s"TERMINAL_DEFAULT" | "TERMINAL_GREEN"%-10d"TERMINAL_DEFAULT" | "TERMINAL_GREEN"%-10d"TERMINAL_DEFAULT" |\n", model->circuits[counter]->id, model->circuits[counter]->label, model->circuits[counter]->component_count, model->circuits[counter]->link_count);
			}
			else
			{
				printf("| Circuit %-6d | %-"LABEL_SIZE"s | %-10d | %-10d |\n", model->circuits[counter]->id, model->circuits[counter]->label, model->circuits[counter]->component_count, model->circuits[counter]->link_count);
			}
			counter++;
		}
	}
	printf("•----------------•----------------------•------------•------------•\n");
}