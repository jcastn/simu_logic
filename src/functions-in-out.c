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
	printf("The circuit %d contains : %d Components and %d Links on %d Levels, \n\n", circ->id, circ->component_count, circ->link_count, circ->max_level);
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

static void read_file_content(char* file_path)
{
	printf("The choosen file is : %s\n", file_path);

    FILE *file = fopen(file_path, "r");
    if (file) 
	{
		char file_content[100]; 
		printf("\nContenu du fichier :\n\n");

		while(fgets(file_content, 100, file))
		{
  			printf("%s", file_content);
		}
		fclose(file);
    } 
	else 
	{
        printf("ERROR : Impossible to open the file");
    }
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
int		import_file(char* file_path)
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
        read_file_content(file_path);

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


