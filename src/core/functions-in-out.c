//functions-in-out.c
#include <nfd.h>
#include "../../include/prototypes.h"


#ifdef __APPLE__
// Temporary fix for a known issue:
// nfd_file() don't really work on MacOS devices if the application is a binary file in terminal.
// To temporary fix that, we can use apple scripts to directly interact with the OS, without using an external module.  
static char* macos_file(FileMode mode)
{
	char path[256] = {0};
	char command[128];

	// Apple scripts for IMPORT files and EXPORT files popups
	if (mode == IMPORT)
	{
		strcpy(command, "osascript -e 'POSIX path of (choose file with prompt \"Select a circuit file:\" of type {\"txt\", \"json\"})'");
	}
	else
	{
		strcpy(command, "osascript -e 'POSIX path of (choose file name with prompt \"Export circuit as:\" default name \"modele_simu_logic.txt\")'");
	}

	// Exec apple script
	FILE* fp = popen(command, "r");
	if (fp == NULL) {
		printf(MESS_ERROR"Failed to run apple script\n");
		return NULL;
	}

	// Remove the end of the received path
	if (fgets(path, sizeof(path), fp) != NULL) {
		
		path[strcspn(path, "\n")] = '\0';
		path[strcspn(path, "\r")] = '\0';
	}

	int status = pclose(fp);

	// If user pressed Cancel
	if (status != 0 || strlen(path) == 0) {
		printf(MESS_INFO"User pressed cancel\n");
		return NULL;
	}

	printf(MESS_INFO"File found: %s\n", path);
	return strdup(path);
}
#endif



static char* nfd_file(FileMode mode)
{
	char* path = NULL;

	#ifdef __APPLE__
	path = macos_file(mode);
	return path;
	#endif


	if (NFD_Init() != NFD_OKAY) {
		printf(MESS_ERROR"Error with nfd_file(): %s\n", NFD_GetError());
		return NULL;
	}
	
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
		printf(MESS_INFO"File found with nfd_file(): %s\n", outPath);
		path = strdup(outPath);
		NFD_FreePathU8(outPath);
	}
	else if (result == NFD_CANCEL)
	{
		printf(MESS_INFO"User pressed cancel from file explorer popup.");
	}
	else 
	{
		printf(MESS_ERROR"Error: %s\n", NFD_GetError());
	}

	NFD_Quit();
	fflush(stdout);
	fflush(stdin);

	return path;
}


static void	read_file_content(char* file_path, Model* model)
{
	FILE *file = fopen(file_path, "r");
	if (!file) 
	{
		printf(MESS_ERROR"ERROR: Impossible to open the file\n");
		return;
	}

	char line[128];
	char type_str[LABEL_SIZE_NUM+1];
	char comp_label[LABEL_SIZE_NUM+1];
	char comp_label2[LABEL_SIZE_NUM+1];
	int src_port = 0;
	int dest_port = 0;
	int nb_in_links = 0;
	int nb_out_links = 0;
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

		char circ_label[LABEL_SIZE_NUM+1];

		// New circuit detection
		if (sscanf(line, "$Circuit$ \"%"LABEL_SIZE"[^\"]\"", circ_label))
		{
			current_circ = create_circuit(model, circ_label);
			//rename_circuit(model, current_circ, );

			current_state = STATE_NONE;
			continue;
		}

		// Current state detection
		if (strstr(line, "$Components$"))
		{
			current_state = STATE_COMPONENTS;
			printf("\nSTEP 1: STATE_COMPONENTS\n");
			continue;
		}
		else if (strstr(line, "$Inversions$"))
		{
			current_state = STATE_INVERSIONS;
			printf("\nSTEP 2: STATE_INVERSIONS\n");
			continue;
		}
		else if (strstr(line, "$Links$"))
		{
			current_state = STATE_LINKS;
			printf("\n\nSTEP 3: STATE_LINKS\n");
			continue;
		}
		else if (strstr(line, "$Commands$"))
		{
			current_state = STATE_COMMANDS;
			printf("\n\nSTEP 4: STATE_COMMANDS\n");
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
				if (sscanf(line, " type:%"LABEL_SIZE"[^,], label:\"%"LABEL_SIZE"[^\"]\", (in:%d, out:%d) (x:%d, y:%d)", type_str, comp_label, &nb_in_links, &nb_out_links, &x, &y) >= 3)
				{
					type_found = false;
					comp_type = string_to_typecomponent(type_str, &type_found);

					if (type_found)
					{
						Component* comp = create_component(current_circ, comp_type, comp_label, nb_in_links, nb_out_links);
						if (comp != NULL)
						{
							update_coordinates(comp, x, y);
						}						
					}
					else
					{
						printf(MESS_ERROR"ERROR: Unknown component type '%s'. Component not created, please review the line in the import file. \n", type_str);
					}
				}
				break;
			}
			case STATE_INVERSIONS:
			{
				if (sscanf(line, " label:\"%"LABEL_SIZE"[^\"]\"", comp_label) == 1)
				{
					Component* comp = get_component_by_label(comp_label, current_circ);
					if (comp)
					{
						invert_source_state(comp);
					}
					else 
					{
						printf(MESS_ERROR"ERROR: Unknown component name '%s', no component inverted, please review the line in the import file. \n", comp_label);
					}
				}
				break;
			}
			case STATE_LINKS:
			{
				if (sscanf(line, " src:\"%"LABEL_SIZE"[^\"]\", src_port:%d, dest:\"%"LABEL_SIZE"[^\"]\", dest_port:%d", comp_label, &src_port, comp_label2, &dest_port) == 4)
				{
					Component* src = get_component_by_label(comp_label, current_circ);
					Component* dest = get_component_by_label(comp_label2, current_circ);

					if ((src) && (dest) && (src_port >= 0) && (dest_port >= 0))
					{
						create_link(current_circ, src, PORT_INPUT(src_port), dest, PORT_INPUT(dest_port));
					}
					else 
					{
						printf(MESS_ERROR"ERROR: Unknown component name source '%s', or dest '%s', or src port '%d' or dest port '%d': No link created, please review the line in the import file. \n", comp_label, comp_label2, src_port, dest_port);
					}
				}
				break;
			}
			case STATE_COMMANDS:
			{
				printf(MESS_INFO"Not yet implemented !");
				break;
			}

			default:
			{
				break;
			}
		}		
	}
	rearrange_circuit(current_circ, false);
	printf(MESS_INFO"File '%s' fully imported !\n\n", file_path);
	fclose(file);
}

static void	write_file_content(char* file_path, Model *model, int circuit_index)
{
	if(model->circuits_count == 0){
		printf(MESS_ERROR"Error ! The selected model contains no circuits, so there's nothing to export.");
		return;
	}
	int circ;
	int comp;

	circ = 0;
	FILE *file = fopen(file_path, "w");
	

	if (file == NULL) {
		printf(MESS_ERROR"Error ! File is NULL (function write_file_content()) ! \n");
		return;
	}

	if (circuit_index != -1)
	{
		circ = circuit_index;
	}

	while(circ < model->circuits_count)
	{
		fprintf(file, "$Circuit$ \"%s\"\n", model->circuits[circ]->label);

		fprintf(file, "\t$Components$\n");
		comp = 0;
		while(comp < model->circuits[circ]->component_count)
		{
			fprintf(file, "\t\ttype:%s, label:\"%s\", (in:%d, out:%d) (x:%d, y:%d)\n", 
				COMPONENT_MAP[model->circuits[circ]->components[comp]->type].name,
				model->circuits[circ]->components[comp]->label,
				model->circuits[circ]->components[comp]->nb_in_ports,
				model->circuits[circ]->components[comp]->nb_out_ports,
				model->circuits[circ]->components[comp]->coordinates->x,
				model->circuits[circ]->components[comp]->coordinates->y);
			comp++;
		}

		comp = 0;
		fprintf(file, "\n\t$Inversions$\n");
		while(comp < model->circuits[circ]->component_count)
		{
			if((model->circuits[circ]->components[comp]->type == SOURCE) && (model->circuits[circ]->components[comp]->status.binary == 1))
			{
				fprintf(file, "\t\tlabel:\"%s\"\n",model->circuits[circ]->components[comp]->label);
			}
			
			comp++;
		}

		comp = 0;
		fprintf(file, "\n\t$Links$\n");
		while(comp < model->circuits[circ]->link_count)
		{
			fprintf(file, "\t\tsrc:\"%s\", src_port:%d, dest:\"%s\", dest_port:%d\n", 
				model->circuits[circ]->links[comp]->src->label,
				PORT_DISPLAY(model->circuits[circ]->links[comp]->src_port_number),
				model->circuits[circ]->links[comp]->dest->label,
				PORT_DISPLAY(model->circuits[circ]->links[comp]->dest_port_number));
			comp++;
		}
		fprintf(file, "\n");

		if (circuit_index != -1){
			break;
		}
		circ++;

	}


	fclose(file);
	
	if (circuit_index == -1)
	{
		printf(MESS_INFO"File '%s' is generated with success. It contains %d circuits.\n", file_path, circ);
		return;
	}
	else
	{
		printf(MESS_INFO"File '%s' is generated with success. It contains 1 circuit.\n", file_path);
		return;
	}
}


// Function to import/export a file with 3 arguments: 
// - Argument 1: File_path 
// -> If it's a string, the function will use this string as a file_path
// -> If it's NULL, the user will select the file from a NFD Popup. 
// - Argument 2: FileMode (IMPORT or EXPORT)
// - Argument 3: Model
// - Argument 4: Number of the circuit to process (only works with EXPORT), use -1 to select all circuits
void		file_process(char* file_path, FileMode file_mode, Model* model, int circuit_index)
{
	bool needs_free = false;
	// If the function file_process is called with a NULL value, the nfd_file_process() function is used to allow the user to choose a file from the File Explorer
	if (file_path == NULL)
	{
		file_path = nfd_file(file_mode);

		if (file_path == NULL)
		{
			printf(MESS_ERROR"No file find ! \n");
			return;
		}
		needs_free = true;
	}


	if (file_path != NULL) {
		if (file_mode == IMPORT)
		{
			if (!check_path(file_path)){
				return; 
			}
			printf("\n(⬇︎) File open: %s\n", file_path);
			read_file_content(file_path, model);
		}
		else	//file_mode == EXPORT
		{
			printf("\n(+) File created: %s\n", file_path);
			write_file_content(file_path, model, circuit_index);
		}

		if (needs_free)
		{
			free(file_path);
		}
	}
	else {
		printf(MESS_ERROR"No file were imported.\n");
	}

	return;
}