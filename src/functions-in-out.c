//functions-in-out.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nfd.h>

#include "prototypes.h"
#include "structures.h"

#define TERMINAL_BLACK		"\e[0;30m"
#define TERMINAL_RED		"\e[0;31m"
#define TERMINAL_GREEN		"\e[0;32m"
#define TERMINAL_YELLOW		"\e[0;33m"
#define	TERMINAL_BLUE		"\e[0;34m"
#define TERMINAL_MAGENTA	"\e[0;35m"
#define TERMINAL_CYAN		"\e[0;36m"
#define TERMINAL_WHITE		"\e[0;37m"
#define TERMINAL_DEFAULT	"\e[0;0m"

void	print_circuit_diodes(Circuit* circ)
{
	if (!circ)
	{
		return;
	}

	printf("Diodes status (Circuit ID:%d) :\n", circ->id);
	int i = 0;
	while (i < circ->component_count)
	{
		Component* comp = circ->components[i];
		if (comp->type == DIODE)
		{
			printf("• Diode (ID: %d) at coordinates (%d, %d) is: %s\n", comp->id, comp->coordinates->x, comp->coordinates->y, comp->out_status ? "ON (true)" : "OFF (false)");
		}
		i++;
	}
}

void	print_circuit_components(Circuit* circ)
{
	int i;
	printf("\nCircuit %d (\"%s\"):\n%d Components and %d Links on %d Levels, \n", circ->id, circ->label, circ->component_count, circ->link_count, circ->max_level);
	printf("•------------------•------------------•-------------•-----•-------•-------•--------•--------•------------------•\n");
	printf("| Component Label  | Component Type   | State       | ID  | Level | Align | x      | y      | Links            |\n");
	printf("•------------------•------------------•-------------•-----•-------•-------•--------•--------•------------------•\n");


	i = 0;
	while(i<circ->component_count)
	{
		const char* component_color;
		Component* comp = circ->components[i];

		// Component Label and Component Type color 
		if (comp->type <= 1){
			component_color = TERMINAL_CYAN;
		}
		else 
		{
			component_color = TERMINAL_MAGENTA;
		}

		// Out-Status State
		const char* state_color = comp->out_status ? TERMINAL_GREEN : TERMINAL_RED;
		const char* state_text = comp->out_status ? "ON" : "OFF";

		if (comp->type == DIODE_RGB)
		{
			if (read_parent_status(comp, 0) && read_parent_status(comp, 1) && read_parent_status(comp, 2))
			{
				state_color = TERMINAL_WHITE;
				state_text = "ON (white)"; 
			}
			else if (read_parent_status(comp, 0) && read_parent_status(comp, 1))
			{
				state_color = TERMINAL_YELLOW;
				state_text = "ON (yellow)";
			}
			else if (read_parent_status(comp, 0) && read_parent_status(comp, 2))
			{
				state_color = TERMINAL_MAGENTA;
				state_text = "ON (magenta)";
			}
			else if (read_parent_status(comp, 1) && read_parent_status(comp, 2))
			{
				state_color = TERMINAL_CYAN;
				state_text = "ON (cyan)";
			}
			else if (read_parent_status(comp, 0))     
			{
				state_color = TERMINAL_RED;
				state_text = "ON (red)";
			}
			else if (read_parent_status(comp, 1))     
			{
				state_color = TERMINAL_GREEN;
				state_text = "ON (green)";
			}
			else if (read_parent_status(comp, 2))     
			{
				state_color = TERMINAL_BLUE;
				state_text = "ON (blue)";
			}
		}
		
		printf("| %s%-16s" TERMINAL_DEFAULT " | %s%-16s" TERMINAL_DEFAULT " | %s%-12s" TERMINAL_DEFAULT "| %-3d | %-5d | %-5d | %-6d | %-6d | In:%-4d Out:%-4d |\n", 
			component_color,
			comp->label,
			component_color,
			ComponentNames[comp->type],
			state_color,
			state_text,
			comp->id, 
			comp->coordinates->level, 
			comp->coordinates->alignment, 
			comp->coordinates->x, 
			comp->coordinates->y,
			comp->nb_in,
			comp->nb_out);
		
		i++;
	}
	printf("•------------------•------------------•-------------•-----•-------•-------•--------•--------•------------------•\n");
}

void	print_model_components(Model *model)
{
	int i = 0;

	while (i<model->circuits_count)
	{
		print_circuit_components(model->circuits[i]);
		i++;
	}
}

static char* nfd_file(FileMode mode)
{
	if (NFD_Init() != NFD_OKAY) {
		printf("/!\\ Error with nfd_file() : %s\n", NFD_GetError());
		return NULL;
	}

	char* path = NULL;
	nfdu8char_t *outPath = NULL;
	nfdu8filteritem_t filters[1] = { {"Text file", "txt,json"} };
	nfdresult_t result;

	if (mode == IMPORT)
	{
		nfdopendialogu8args_t open_args = {0};
		open_args.filterList = filters;
		open_args.filterCount = 1;
		result = NFD_OpenDialogU8_With(&outPath, &open_args);
	}
	else
	{
		nfdsavedialogu8args_t save_args = {0};
		save_args.filterList = filters;
		save_args.filterCount = 1;
		save_args.defaultName = "modele_simu_logic.txt";
		result = NFD_SaveDialogU8_With(&outPath, &save_args);
	}
	
	if (result == NFD_OKAY)
	{
		printf("(i) INFO : File found with nfd_file() : %s\n", outPath);
		path = strdup(outPath);
		NFD_FreePathU8(outPath);
	}
	else if (result == NFD_CANCEL)
	{
		printf("(i) INFO : nfd_file() - User pressed cancel");
	}
	else 
	{
		printf("/!\\ Error: %s\n", NFD_GetError());
	}

	NFD_Quit();
	return path;
}

static void	read_file_content(char* file_path, Model* model)
{
	printf("⬇︎ File open : %s\n", file_path);

	FILE *file = fopen(file_path, "r");
	if (!file) 
	{
		printf("/!\\ ERROR : Impossible to open the file");
		return;
	}

	char line[100];
	char type_str[16];
	char comp_label[16];
	char comp_label2[16];
	int port = 0;

	int nb_in = 0;
	int x = 0;
	int y = 0;
	TypeComponent comp_type = SOURCE;
	bool type_found = false;
	ParseState current_state = STATE_NONE;
	Circuit* current_circ = NULL;

	while(fgets(line, sizeof(line), file))
	{
		if (line[0] == '\n' || line[0] == '\r' || strstr(line, "//") == line) 
		{
			continue;
		}

		char circ_name[16];

		// New circuit detection
		if (sscanf(line, "$Circuit$ \"%[^\"]\"", circ_name))
		{
			current_circ = create_circuit(model);
			rename_circuit(current_circ, circ_name);

			current_state = STATE_NONE;
			continue;
		}

		// Current state detection
		if (strstr(line, "$Components$"))
		{
			current_state = STATE_COMPONENTS;
			printf("\nSTEP 1 : STATE_COMPONENTS\n");
			continue;
		}
		else if (strstr(line, "$Inversions$"))
		{
			current_state = STATE_INVERSIONS;
			printf("\nSTEP 2 : STATE_INVERSIONS\n");
			continue;
		}
		else if (strstr(line, "$Links$"))
		{
			current_state = STATE_LINKS;
			printf("\nSTEP 3 : STATE_LINKS\n");
			continue;
		}

		if (!current_circ) 
		{
			continue;
		}

		switch (current_state)
		{
			case STATE_COMPONENTS:
			{
				if (sscanf(line, " %15[^,], \"%15[^\"]\", %d, %d, %d", type_str, comp_label, &nb_in, &x, &y) >= 3)
				{
					type_found = false;
					comp_type = string_to_typecomponent(type_str, &type_found);

					if (type_found)
					{
						Component* comp = create_component(comp_type, nb_in, current_circ);
						rename_component(comp, comp_label);
						update_coordinates(comp, x, y);
					}
					else
					{
						printf("/!\\ ERROR : Unknown component type '%s'. Component not created, please review the line in the import file. \n", type_str);
					}
				}
				break;
			}
			case STATE_INVERSIONS:
			{
				if (sscanf(line, " \"%15[^\"]\"", comp_label) == 1)
				{
					Component* comp = get_component_by_label(comp_label, current_circ);
					if (comp)
					{
						invert_source_state(comp);
					}
					else 
					{
						printf("/!\\ ERROR : Unknown component name '%s', no component inverted, please review the line in the import file. \n", comp->label);
					}
				}
				break;
			}
			case STATE_LINKS:
			{
				if (sscanf(line, " \"%[^\"]\", \"%[^\"]\", %d", comp_label, comp_label2, &port) == 3)
				{
					Component* src = get_component_by_label(comp_label, current_circ);
					Component* dest = get_component_by_label(comp_label2, current_circ);
					if (src && dest && port >= 0)
					{
						create_link(src, dest, port, current_circ);
					}
					else 
					{
						printf("/!\\ ERROR : Unknown component name source '%s', or dest '%s', or port '%d' : No link created, please review the line in the import file. \n", src->label, dest->label, port);
					}
				}
				break;
			}
			default:
			{
				break;
			}
		}


		//printf("%s", file_content);
		//create_circuit(word, model);
		//rename_circuit(model->circuits[0], word);
		
	}
	fclose(file);
}

static void	write_file_content(char* file_path, Model *model)
{
	if(model->circuits_count == 0){
		printf("/!\\ Error ! The selected model contains no circuits, so there's nothing to export.");
		return;
	}
	int circ;
	int comp;

	circ = 0;
	FILE *file = fopen(file_path, "w");
	

	if (file == NULL) {
		printf("/!\\ Error ! File is NULL (function write_file_content()) ! \n");
		return;
	}
	while(circ < model->circuits_count)
	{
		fprintf(file, "$Circuit$ \"%s\"\n", model->circuits[circ]->label);

		fprintf(file, "\t$Components$\n");
		comp = 0;
		while(comp < model->circuits[circ]->component_count)
		{
			fprintf(file, "\t\t%s, \"%s\", %d, %d, %d\n", 
				ComponentNames[model->circuits[circ]->components[comp]->type],
				model->circuits[circ]->components[comp]->label,
				model->circuits[circ]->components[comp]->nb_in,
				model->circuits[circ]->components[comp]->coordinates->x,
				model->circuits[circ]->components[comp]->coordinates->y);
			comp++;
		}

		comp = 0;
		fprintf(file, "\n\t$Inversions$\n");
		while(comp < model->circuits[circ]->component_count)
		{
			if((model->circuits[circ]->components[comp]->type == SOURCE) && (model->circuits[circ]->components[comp]->out_status == 1))
			{
				fprintf(file, "\t\t\"%s\"\n",model->circuits[circ]->components[comp]->label);
			}
			
			comp++;
		}

		comp = 0;
		fprintf(file, "\n\t$Links$\n");
		while(comp < model->circuits[circ]->link_count)
		{
			fprintf(file, "\t\t\"%s\", \"%s\", %d\n", 
				model->circuits[circ]->links[comp]->src->label,
				model->circuits[circ]->links[comp]->dest->label,
				model->circuits[circ]->links[comp]->port_number);
			comp++;
		}
		circ++;
	}


	fclose(file);
	
	printf("(i) INFO : File '%s' is generated with success. It contains %d circuits.\n", file_path, circ);

	return;
}


// Function to import/export a file with 3 arguments : 
// - Argument 1 : File_path 
// -> If it's a string, the function will use this string as a file_path
// -> If it's NULL, the user will select the file from a NFD Popup. 
// - Argument 2 : FileMode (IMPORT or EXPORT)
// - Argument 3 : Model
int		file_process(char* file_path, FileMode file_mode, Model* model)
{
	bool needs_free = false;
	// If the function file_process is called with a NULL value, the nfd_file_process() function is used to allow the user to choose a file from the File Explorer
	if (file_path == NULL)
	{
		file_path = nfd_file(file_mode);

		if (file_path == NULL)
		{
			printf("Error ! No file find ! \n");
			return -1;
		}
		needs_free = true;
	}


	if (file_path != NULL) {
		printf("⬇︎ File open : %s\n", file_path);

		if (file_mode == IMPORT)
		{
			read_file_content(file_path, model);
		}
		else
		{
			write_file_content(file_path, model);
		}

		if (needs_free)
		{
			free(file_path);
		}
	} 
	else {
		printf("ERROR : No file were imported.\n");
	}

	return 0;
}