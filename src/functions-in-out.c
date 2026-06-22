#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nfd.h>

#include "prototypes.h"
#include "structures.h"

#define TERMINAL_BLACK		"\033[30m"
#define TERMINAL_RED		"\033[31m"
#define TERMINAL_GREEN		"\033[32m"
#define	TERMINAL_BLUE		"\033[34m"
#define TERMINAL_MAGENTA	"\033[35m"
#define TERMINAL_CYAN		"\033[36m"
#define TERMINAL_DEFAULT   	"\033[0m"

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
	printf("\nCircuit %d (\"%s\"):\n%d Components and %d Links on %d Levels, \n", circ->id, circ->label, circ->component_count, circ->link_count, circ->max_level);
	printf("•------------------•------------------•-------•-----•-------•-------•--------•--------•------------------•\n");
	printf("| Component Label  | Component Type   | State | ID  | Level | Align | x      | y      | Links            |\n");
	printf("•------------------•------------------•-------•-----•-------•-------•--------•--------•------------------•\n");


	for (int i = 0; i<circ->component_count; i++)
	{
		const char* current_color;
		Component* comp = circ->components[i];

		if (comp->type <= 1){
			current_color = TERMINAL_CYAN;
		}
		else 
		{
			current_color = TERMINAL_MAGENTA;
		}

		printf("| %s%-16s" TERMINAL_DEFAULT " | %s%-16s" TERMINAL_DEFAULT " | %s%-5s" TERMINAL_DEFAULT " | %-3d | %-5d | %-5d | %-6d | %-6d | In:%-4d Out:%-4d |\n", 
			current_color,
			comp->label,
			current_color,
			ComponentNames[comp->type],
			comp->out_status ? TERMINAL_GREEN : TERMINAL_RED, 
			comp->out_status ? "ON" : "OFF",
			comp->id, 
			comp->coordinates->level, 
			comp->coordinates->alignment, 
			comp->coordinates->x, 
			comp->coordinates->y,
			comp->nb_in,
			comp->nb_out);

	}
	printf("•------------------•------------------•-------•-----•-------•-------•--------•--------•------------------•\n");
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

static char* nfd_import_file_popup()
{
	if (NFD_Init() != NFD_OKAY) {
        printf("Error: %s\n", NFD_GetError());
        return NULL;
    }

	char* returned_path = NULL;


    nfdu8char_t *outPath;
    nfdu8filteritem_t filters[1] = { {"Text file", "txt,json"} };
    nfdopendialogu8args_t args = {0};
    args.filterList = filters;
    args.filterCount = 1;

    nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
	
    if (result == NFD_OKAY)
    {
        printf("Success! File found : %s\n", outPath);
		returned_path = strdup(outPath);
        NFD_FreePathU8(outPath);
    }
    else if (result == NFD_CANCEL)
    {
        printf("User pressed cancel");
    }
    else 
    {
        printf("Error: %s\n", NFD_GetError());
    }

    NFD_Quit();
	return returned_path;
}

static void read_file_content(char* file_path, Model* model)
{
	printf("The choosen file is : %s\n", file_path);

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
			printf("STEP 1 : STATE_COMPONENTS\n");
			continue;
		}
		else if (strstr(line, "$Inversions$"))
		{
			current_state = STATE_INVERSIONS;
			printf("STEP 2 : STATE_INVERSIONS\n");
			continue;
		}
        else if (strstr(line, "$Links$"))
		{
			current_state = STATE_LINKS;
			printf("STEP 3 : STATE_LINKS\n");
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
				if (sscanf(line, " %[^,], \"%[^\"]\", %d, %d, %d", type_str, comp_label, &nb_in, &x, &y) >= 3)
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
				if (sscanf(line, " \"%[^\"]\"", comp_label) == 1)
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




/*
void		save_circuit(Circuit *circ)
{
	//Export a circuit to a .txt or .json file 
}
*/

// Function to import a file : 
// 1. Use `import_file(NULL);` to import a file choosen by the user from a File Explorer popup (with NFD module)
// 2. Use `import_file("path/to/file.txt");` to import a file from a relative path 
int		import_file(char* file_path, Model* model)
{
	bool needs_free = false;
	// If the function import_file is called with a NULL value, the nfd_import_file() function is used to allow the user to choose a file from the File Explorer
	if (file_path == NULL)
	{
		file_path = nfd_import_file_popup();

		if (file_path == NULL)
		{
			printf("Error ! No file find ! \n");
			return -1;
		}
		needs_free = true;
	}
    

    if (file_path != NULL) {
        read_file_content(file_path, model);

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


