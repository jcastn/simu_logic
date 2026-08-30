// src/core/in-out.c
#include "../../include/prototypes-core.h"
#include "../../third_party/tinyfiledialogs/tinyfiledialogs.h"

static char* tfd_file(FileMode mode, void (*logger)(const char*)) {
	char const *file_path = NULL;
	char const *filters[1] = {"*.txt"};
	char message[MESSAGE_SIZE_NUM];
	
	if (mode == IMPORT)
	{
	  	file_path = tinyfd_openFileDialog("Select a circuit file", "", 1, filters, "Circuit files (.txt)", 0);
	}
	else if (mode == EXPORT)
	{
	  	file_path = tinyfd_saveFileDialog("Export circuit as", "modele_simu_logic.txt", 1, filters, "Circuit files (.txt)");
	}
	else if (mode == COMMANDS)
	{
	  	file_path = tinyfd_openFileDialog("Select a commands file", "", 1, filters, "Commands files (.txt)", 0);
	}


	if (!file_path) {
	  snprintf(message, sizeof(message), MESS_INFO "User pressed cancel\n");
	  logger(message);
	  return NULL;
	}

	return strdup(file_path);
}

static void	import_file_content(char* file_path, Model* model, void (*logger)(const char*))
{
	char message[MESSAGE_SIZE_NUM];
	FILE *file = fopen(file_path, "r");
	if (!file) 
	{
		snprintf(message, sizeof(message), MESS_ERROR"ERROR: Impossible to open the file\n");
		logger(message);
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
			current_state = STATE_NONE;
			continue;
		}

		// Current state detection
		if (strstr(line, "$Components$"))
		{
			current_state = STATE_COMPONENTS;
			snprintf(message, sizeof(message), "\n\n"TERMINAL_PURPLE"(↓) Step 1 :"TERMINAL_DEFAULT" Creating components");
			logger(message);
			continue;
		}
		else if (strstr(line, "$Inversions$"))
		{
			current_state = STATE_INVERSIONS;
			snprintf(message, sizeof(message), "\n\n"TERMINAL_PURPLE"(↓) Step 2 :"TERMINAL_DEFAULT" Inverting sources :");
			logger(message);
			continue;
		}
		else if (strstr(line, "$Links$"))
		{
			current_state = STATE_LINKS;
			snprintf(message, sizeof(message), "\n\n"TERMINAL_PURPLE"(↓) Step 3 :"TERMINAL_DEFAULT" Creating links :");
			logger(message);
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
						if (!comp)
						{
							snprintf(message, sizeof(message), MESS_ERROR"Component '%s' not created.", comp_label);
							logger(message);
							return;
						}
						else 
						{
							update_coordinates(comp, x, y);
							snprintf(message, sizeof(message), 	MESS_COMP"Component '%s%s"TERMINAL_DEFAULT"' created as '%s%s"TERMINAL_DEFAULT"', with %d inbound ports and %d outbound ports, on coordinates (x:%d;y:%d).", 
									COMPONENT_MAP[comp->type].color, COMPONENT_MAP[comp->type].name, 
									COMPONENT_MAP[comp->type].color, comp->label, 
									comp->nb_in_ports, comp->nb_out_ports,
									comp->coordinates.x, comp->coordinates.y);
							logger(message);
						}
					}
					else
					{
						snprintf(message, sizeof(message), MESS_ERROR"Component '%s' not created because its type '%s' is missing or is invalid.", comp_label, type_str);
						logger(message);
					}
				}
				break;
			}
			case STATE_INVERSIONS:
			{
				if (sscanf(line, " label:\"%"LABEL_SIZE"[^\"]\"", comp_label) == 1)
				{
					Component* comp = get_component_by_label(comp_label, current_circ);
					if (!comp)
					{
						snprintf(message, sizeof(message), MESS_ERROR"Unknown component name '%s', no component inverted.", comp_label);
						logger(message);	
					}
					else 
					{
						invert_source_state(comp);
						snprintf(message, sizeof(message), MESS_COMP"Status of the component '"TERMINAL_CYAN"%s"TERMINAL_DEFAULT"' is inverted.", comp->label);
						logger(message);
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
						snprintf(message, sizeof(message), MESS_LINK"Link created : '%s%s (out port %d)%s' → '%s%s (in port %d)%s'", COMPONENT_MAP[src->type].color, src->label, src_port, TERMINAL_DEFAULT, COMPONENT_MAP[dest->type].color, dest->label, dest_port, TERMINAL_DEFAULT);
						logger(message);
					}
					else 
					{
						snprintf(message, sizeof(message), MESS_ERROR"ERROR: Unknown component name source '%s', or dest '%s', or src port '%d' or dest port '%d': No link created, please review the line in the import file.", comp_label, comp_label2, src_port, dest_port);
						logger(message);
					}
				}
				break;
			}
			default:
			{
				break;
			}
		}		
	}
	rearrange_circuit(current_circ, false);
	snprintf(message, sizeof(message), "\n"MESS_INFO"File '%s' fully imported !", file_path);
	logger(message);
	fclose(file);
}

static void	write_file_content(char* file_path, Model *model, int circuit_index, void (*logger)(const char*))
{
	int circ;
	int comp;
	char message[MESSAGE_SIZE_NUM];

	if(model->circuits_count == 0){
		snprintf(message, sizeof(message), MESS_ERROR"Error ! The selected model contains no circuits, so there's nothing to export.");
		logger(message);
		return;
	}


	circ = 0;
	FILE *file = fopen(file_path, "w");
	

	if (!file) {
		snprintf(message, sizeof(message), MESS_ERROR"Error ! File is NULL (function write_file_content()) ! \n");
		logger(message);
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
				model->circuits[circ]->components[comp]->coordinates.x,
				model->circuits[circ]->components[comp]->coordinates.y);
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
		snprintf(message, sizeof(message), MESS_INFO"File '%s' is generated with success. It contains %d circuits.\n", file_path, circ);
		logger(message);
		return;
	}
	else
	{
		snprintf(message, sizeof(message), MESS_INFO"File '%s' is generated with success. It contains 1 circuit.\n", file_path);
		logger(message);
		return;
	}
}

static void run_file_content(char* file_path, Model* model, void (*logger)(const char*), void (*process_line)(Model*, char*))
{
	const char* file_name = strrchr(file_path, '/');
	file_name++;
	char message[MESSAGE_SIZE_NUM];

	FILE *file = fopen(file_path, "r");
	if (!file) 
	{
		snprintf(message, sizeof(message), MESS_ERROR"ERROR: Impossible to open the file\n");
		return;
	}

	char 	line[200];

	// First pass: count total commands
	int total = 0;
	while(fgets(line, sizeof(line), file))
	{
		if (line[0] != '\n' && line[0] != '\r' && strstr(line, "//") != line)
		{
			total++;
		}
	}
	rewind(file);

	// Second pass: execute commands
	int counter = 1;
	while(fgets(line, sizeof(line), file))
	{
		if (line[0] == '\n' || line[0] == '\r' || strstr(line, "//") == line) 
		{
			continue;
		}

		// Remove trailing newline/carriage return
		line[strcspn(line, "\r\n")] = '\0';

		if ((model->active_circuit != NULL) && (strlen(model->active_circuit->label) > 0))
		{
			snprintf(message, sizeof(message), "\n"TERMINAL_MAGENTA"[running \"%s\" (%d/%d)] " TERMINAL_GREEN "\"%s\"" TERMINAL_MAGENTA" > "TERMINAL_DEFAULT"%s\n", file_name, counter, total, model->active_circuit->label, line);
			logger(message);
		}
		else
		{
			snprintf(message, sizeof(message), "\n"TERMINAL_MAGENTA"[running \"%s\" (%d/%d)] > "TERMINAL_DEFAULT"%s\n", file_name, counter, total, line);
			logger(message);
		}

		process_line(model, line);
		//scan_user_entry(model, line);
		getchar();
		counter++;
	}
	snprintf(message, sizeof(message), MESS_INFO"File '%s' fully runned !\n", file_path);
	logger(message);
	fclose(file);
	return;
}


// Function to import/export a file with 3 arguments: 
// - Argument 1: File_path 
// -> If it's a string, the function will use this string as a file_path
// -> If it's NULL, the user will select the file from a TFD Popup. 
// - Argument 2: FileMode (IMPORT or EXPORT)
// - Argument 3: Model
// - Argument 4: Number of the circuit to process (only works with EXPORT), use -1 to select all circuits
void		file_process(char* file_path, FileMode file_mode, Model* model, int circuit_index, void (*logger)(const char*),	void (*process_line)(Model*, char*))
{
	char message[MESSAGE_SIZE_NUM];
	bool needs_free = false;
	// If the function file_process is called with a NULL value, the tfd_file() function is used to allow the user to choose a file from the File Explorer
	if (!file_path)
	{
		file_path = tfd_file(file_mode, logger);

		if (!file_path)
		{
			snprintf(message, sizeof(message), MESS_ERROR"No file find ! \n");
			logger(message);
			return;
		}
		needs_free = true;
	}

	if (!file_path)
	{
		snprintf(message, sizeof(message), MESS_ERROR"There's no file path ! No file were imported.\n");
		logger(message);
	}
	else 
	{
		if (file_mode == IMPORT)
		{
			if (!check_path(file_path)){
				snprintf(message, sizeof(message), MESS_ERROR"The file path "TERMINAL_ORANGE"\"%s'"TERMINAL_DEFAULT" is invalid !", file_path);
				logger(message);
				return; 
			}

			snprintf(message, sizeof(message), "\n"TERMINAL_PURPLE"(↓) File open :"TERMINAL_DEFAULT" \"%s\"", file_path);
			logger(message);
			import_file_content(file_path, model, logger);
		}
		else if (file_mode == EXPORT)
		{
			snprintf(message, sizeof(message), "\n"TERMINAL_PURPLE"(+) File created:"TERMINAL_DEFAULT" \"%s\"\n", file_path);
			logger(message);
			write_file_content(file_path, model, circuit_index, logger);
		}
		else if (file_mode == COMMANDS)
		{
			if (!check_path(file_path)){
				snprintf(message, sizeof(message), MESS_ERROR"The file path "TERMINAL_ORANGE"\"%s'"TERMINAL_DEFAULT" is invalid !", file_path);
				logger(message);
				return; 
			}

			snprintf(message, sizeof(message), "\n"TERMINAL_PURPLE"(>) File runned :"TERMINAL_DEFAULT" \"%s\"\n", file_path);
			logger(message);
			run_file_content(file_path, model, logger, process_line);
		}

		if (needs_free)
		{
			free(file_path);
		}
	}
	return;
}
