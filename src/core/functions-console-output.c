// functions-console-output.c
#include "../../include/prototypes.h"

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
		snprintf(state_text_buffer, STATE_SIZE_NUM, "Value : %d", comp->out_status.number);
	}
	else if (comp->type == DISPLAY_HEX)
	{
		snprintf(state_text_buffer, STATE_SIZE_NUM, "Value : %X", comp->out_status.number);
	}
	else if (comp->type == DISPLAY_CHAR)
	{
		if (comp->out_status.character >= 32 && comp->out_status.character <= 126)
		{
			snprintf(state_text_buffer, STATE_SIZE_NUM, "Char : '%c'", comp->out_status.character);
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

static void		get_component_out_status(Component* comp, const char** state_color, const char** state_text)
{
	static char state_text_buffer[STATE_SIZE_NUM];

	if (comp->type == DIODE_RGB)
	{
		compute_color(comp->out_status.rgb, state_color, state_text);
	}
	else if ((comp->type == DISPLAY_DEC) || (comp->type == DISPLAY_HEX) || (comp->type == DISPLAY_CHAR))
	{
		compute_number(comp, state_color, state_text, state_text_buffer);
	}
	else
	{
		if (comp->out_status.out)
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

	return;
}


void	show_components_from_circuit(Circuit* circ)
{
	int counter;
	printf(	"\nCircuit %d ("TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT"):\n%d Components and %d Links on %d Levels, \n", circ->id, circ->label, circ->component_count, circ->link_count, circ->max_level);
	printf(	"•----------------------•----------------------•----------------•--------•-------•-------•--------•--------•------------------•\n"
			"| Component Label      | Component Type       | State          | ID     | Level | Align | x      | y      | Links            |\n"
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
			comp->coordinates->level, 
			comp->coordinates->alignment, 
			comp->coordinates->x, 
			comp->coordinates->y,
			comp->nb_in_links,
			comp->nb_out_links);
		counter++;
	}
	printf("•----------------------•----------------------•----------------•--------•-------•-------•--------•--------•------------------•\n");
}


void	show_links_from_circuit(Circuit* circ)
{
	int counter;
	int counter_bis;
	const char* component_color;
	
	if (circ->link_count == 0)
	{
		printf(MESS_INFO"The circuit '%s' contains no links !\n", circ->label);
		return;
	}

	printf(	"\nCircuit %d ("TERMINAL_ORANGE"\"%s\""TERMINAL_DEFAULT"):\n%d Links and %d Components on %d Levels, \n", circ->id, circ->label, circ->link_count, circ->component_count, circ->max_level);

	counter = 0;
	while (counter < circ->component_count)
	{
		counter_bis = 0;
		Component* comp = circ->components[counter];
		component_color = COMPONENT_MAP[comp->type].color;
		if (comp->nb_out_links != 0)
		{
			printf("\n%s%s"TERMINAL_DEFAULT"\n", component_color, comp->label);
			while(counter_bis < comp->nb_out_links)
			{
				component_color = COMPONENT_MAP[comp->out_links[counter_bis]->dest->type].color;
				printf(" ⤷ %s%s"TERMINAL_DEFAULT" (Port : %d)\n", component_color, comp->out_links[counter_bis]->dest->label, PORT_DISPLAY(comp->out_links[counter_bis]->port_number));
				counter_bis++;
			}
		}
		counter++;
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
	int			count;
	char		inbound[64];
	char		outbound[64];
	const char*	component_color;
	const char*	state_color;
	const char*	state_text;

	component_color = COMPONENT_MAP[comp->type].color;
	get_component_out_status(comp, &state_color, &state_text);

	printf(	"\nDetails about component %s%s"TERMINAL_DEFAULT" :"
			"\n▻ Type   : '%s%s"TERMINAL_DEFAULT"'"
			"\n▻ ID     : '%d'"
			"\n▻ x      : '%d'"
			"\n▻ y      : '%d'"
			"\n▻ Level  : '%d'"
			"\n▻ Align  : '%d'"
			"\n▻ State : '%s%s"TERMINAL_DEFAULT"'\n\n",
			component_color, comp->label, 
			component_color, COMPONENT_MAP[comp->type].name, 
			comp->id,
			comp->coordinates->x, comp->coordinates->y,
			comp->coordinates->level, comp->coordinates->alignment,
			state_color, state_text);

	printf(	"•-------------------------------•-------------------------------•\n"
			"| Inbound links                 | Outbound links                |\n"
			"•-------------------------------•-------------------------------•\n");

	count = 0;
	while ((count == 0) || (count < comp->nb_in_links) || (count < comp->nb_out_links))
	{
		if ((count < comp->nb_in_links) && (comp->in_links != NULL))
		{
			if (comp->in_links[count] != NULL)
			{
				component_color = COMPONENT_MAP[comp->in_links[count]->src->type].color;

				snprintf(inbound, sizeof(inbound), "Port %d : %s%-"LABEL_SIZE"s"TERMINAL_DEFAULT, PORT_DISPLAY(count), component_color, comp->in_links[count]->src->label);
			}
			else
			{
				snprintf(inbound, sizeof(inbound), "Port %d : "TERMINAL_GRAY"(empty)"TERMINAL_DEFAULT"             ", PORT_DISPLAY(count));
			}
		}
		else
		{
			if ((count == 0) && (comp->nb_in_links == 0))
			{
				snprintf(inbound, sizeof(inbound), TERMINAL_GRAY"(empty - no inbound ports)   "TERMINAL_DEFAULT);
			}
			else 
			{
				snprintf(inbound, sizeof(inbound), "                             ");
			}
		}

		if ((count < comp->nb_out_links) && (comp->out_links != NULL))
		{
			if ((comp->out_links[count] != NULL) && (comp->out_links[count]->dest != NULL))
			{
				component_color = COMPONENT_MAP[comp->out_links[count]->dest->type].color;
				snprintf(outbound, sizeof(outbound), "%s%-"LABEL_SIZE"s"TERMINAL_DEFAULT"         ", component_color, comp->out_links[count]->dest->label);
			}
			else
			{
				snprintf(outbound, sizeof(outbound), " "TERMINAL_RED"(unknown)"TERMINAL_DEFAULT"                   ");
			}
		}
		else 
		{
			if (count == 0 && comp->nb_out_links == 0)
			{
				snprintf(outbound, sizeof(outbound), TERMINAL_GRAY"(empty - no outbound ports)  "TERMINAL_DEFAULT);
			}
			else
			{
				snprintf(outbound, sizeof(outbound), "                             ");
			}
		}

		printf("| %29s | %29s |\n", inbound, outbound);
		count++;

	}
	printf("•-------------------------------•-------------------------------•\n");
	return;
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