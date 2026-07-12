// functions-console-output.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "prototypes.h"
#include "structures.h"

static const char* ComponentColors[] = {
	[SOURCE]      = TERMINAL_CYAN,
	[DIODE]       = TERMINAL_BLUE,
	[DIODE_RGB]   = TERMINAL_BLUE,
	[BUFFER]      = TERMINAL_PINK,
	[GATE_NOT]    = TERMINAL_PINK,
	[GATE_AND]    = TERMINAL_MAGENTA,
	[GATE_OR]     = TERMINAL_MAGENTA,
	[GATE_XOR]    = TERMINAL_MAGENTA,
	[GATE_NAND]   = TERMINAL_PURPLE,
	[GATE_NOR]    = TERMINAL_PURPLE,
	[GATE_NXOR]   = TERMINAL_PURPLE,
	[GATE_IMPLY]  = TERMINAL_VIOLET,
	[GATE_NIMPLY] = TERMINAL_VIOLET,
};

static void		get_component_out_status(Component* comp, const char** state_color, const char** state_text)
{
	if (comp->type == DIODE_RGB)
	{
		if (comp->out_status.rgb.r && comp->out_status.rgb.g && comp->out_status.rgb.b)
		{
			*state_color = TERMINAL_WHITE;
			*state_text = "ON (white)"; 
		}
		else if (comp->out_status.rgb.r && comp->out_status.rgb.g)
		{
			*state_color = TERMINAL_YELLOW;
			*state_text = "ON (yellow)";
		}
		else if (comp->out_status.rgb.r && comp->out_status.rgb.b)
		{
			*state_color = TERMINAL_MAGENTA;
			*state_text = "ON (magenta)";
		}
		else if (comp->out_status.rgb.g && comp->out_status.rgb.b)
		{
			*state_color = TERMINAL_CYAN;
			*state_text = "ON (cyan)";
		}
		else if (comp->out_status.rgb.r)     
		{
			*state_color = TERMINAL_RED;
			*state_text = "ON (red)";
		}
		else if (comp->out_status.rgb.g)     
		{
			*state_color = TERMINAL_GREEN;
			*state_text = "ON (green)";
		}
		else if (comp->out_status.rgb.b)     
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
	else
	{
		// Out-Status State
		*state_color = comp->out_status.out ? TERMINAL_GREEN : TERMINAL_RED;
		*state_text = comp->out_status.out ? "ON" : "OFF";
	}
	return;
}


void	show_components_from_circuit(Circuit* circ)
{
	int counter;
	printf("\nCircuit %d (\"%s\"):\n%d Components and %d Links on %d Levels, \n", circ->id, circ->label, circ->component_count, circ->link_count, circ->max_level);
	printf("•----------------------•----------------------•--------------•--------•-------•-------•--------•--------•------------------•\n");
	printf("| Component Label      | Component Type       | State        | ID     | Level | Align | x      | y      | Links            |\n");
	printf("•----------------------•----------------------•--------------•--------•-------•-------•--------•--------•------------------•\n");


	counter = 0;
	while(counter<circ->component_count)
	{
		const char* component_color;
		Component* comp = circ->components[counter];
		component_color = ComponentColors[comp->type];

		// Out-Status State
		const char* state_color;
		const char* state_text;

		get_component_out_status(comp, &state_color, &state_text);
		
		printf("| %s%-"LABEL_SIZE"s" TERMINAL_DEFAULT " | %s%-"LABEL_SIZE"s" TERMINAL_DEFAULT " | %s%-12s" TERMINAL_DEFAULT " | %-6d | %-5d | %-5d | %-6d | %-6d | In:%-4d Out:%-4d |\n", 
			component_color, comp->label,
			component_color, ComponentNames[comp->type],
			state_color, state_text,
			comp->id, 
			comp->coordinates->level, 
			comp->coordinates->alignment, 
			comp->coordinates->x, 
			comp->coordinates->y,
			comp->nb_in,
			comp->nb_out);
		
		counter++;
	}
	printf("•----------------------•----------------------•--------------•--------•-------•-------•--------•--------•------------------•\n");
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

void	show_component_links(Component* comp)
{
	int count;
	char inbound[64];
	char outbound[64];
	const char* component_color;

	component_color = ComponentColors[comp->type];
	printf("\nComponent '%s%s"TERMINAL_DEFAULT"' - (Type : '%s%s"TERMINAL_DEFAULT"' - ID : %d)\n", component_color, comp->label, component_color, ComponentNames[comp->type], comp->id);
	printf("•-------------------------------•-------------------------------•\n");
	printf("| Inbound links                 | Outbound links                |\n");
	printf("•-------------------------------•-------------------------------•\n");

	count = 0;
	while ((count == 0) || (count < comp->nb_in) || (count < comp->nb_out))
	{
		if ((count < comp->nb_in) && (comp->in_links != NULL))
		{
			if (comp->in_links[count] != NULL)
			{
				component_color = ComponentColors[comp->in_links[count]->src->type];

				snprintf(inbound, sizeof(inbound), "Port %d : %s%-"LABEL_SIZE"s"TERMINAL_DEFAULT, count, component_color, comp->in_links[count]->src->label);
			}
			else
			{
				snprintf(inbound, sizeof(inbound), "Port %d : "TERMINAL_GRAY"(empty)"TERMINAL_DEFAULT"             ", count);
			}
		}
		else
		{
			if ((count == 0) && (comp->nb_in == 0))
			{
				snprintf(inbound, sizeof(inbound), TERMINAL_GRAY"(empty - no inbound ports)   "TERMINAL_DEFAULT);
			}
			else 
			{
				snprintf(inbound, sizeof(inbound), "                             ");
			}
		}

		if ((count < comp->nb_out) && (comp->out_links != NULL))
		{
			if ((comp->out_links[count] != NULL) && (comp->out_links[count]->dest != NULL))
			{
				component_color = ComponentColors[comp->out_links[count]->dest->type];
				snprintf(outbound, sizeof(outbound), "%s%-"LABEL_SIZE"s"TERMINAL_DEFAULT"         ", component_color, comp->out_links[count]->dest->label);
			}
			else
			{
				snprintf(outbound, sizeof(outbound), " "TERMINAL_RED"(unknown)"TERMINAL_DEFAULT"                   ");
			}
		}
		else 
		{
			if (count == 0 && comp->nb_out == 0)
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
		printf("\nLoaded circuits :\n");
		printf(	"•----------------•----------------------•--------•------------•------------•\n"
				"| Circuit ID     | Circuit Label        | Active | Components | Links      |\n"
				"•----------------•----------------------•--------•------------•------------•\n");
		while(counter < model->circuits_count)
		{
			if (active_circuit_id == model->circuits[counter]->id)
			{
				printf("| "TERMINAL_GREEN"Circuit %-6d"TERMINAL_DEFAULT" | "TERMINAL_GREEN"%-"LABEL_SIZE"s"TERMINAL_DEFAULT" | "TERMINAL_GREEN"Yes"TERMINAL_DEFAULT"    | "TERMINAL_GREEN"%-10d"TERMINAL_DEFAULT" | "TERMINAL_GREEN"%-10d"TERMINAL_DEFAULT" |\n", model->circuits[counter]->id, model->circuits[counter]->label, model->circuits[counter]->component_count, model->circuits[counter]->link_count);
			}
			else
			{
				printf("| Circuit %-6d | %-"LABEL_SIZE"s | No     | %-10d | %-10d |\n", model->circuits[counter]->id, model->circuits[counter]->label, model->circuits[counter]->component_count, model->circuits[counter]->link_count);
			}
			counter++;
		}
	}
	printf("•----------------•----------------------•--------•------------•------------•\n");

}