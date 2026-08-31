// src/cli/run-loop.c
#include "linenoise.h"
#include "prototypes-cli.h"
#include "prototypes-core.h"
#include <errno.h>

#ifdef DEBUG_MODE
#include <unistd.h>
#endif

#ifndef DEBUG_MODE
void			run_loop()
{
	char	prompt[256];

	char*	user_entry;
	linenoiseSetCompletionCallback(auto_complete_entry);
	linenoiseHistorySetMaxLen(100);

	Model* model = create_model();

	model->run_loop = true;

	while(model->run_loop)
	{
		printf("\n\n");

		if ((model->active_circuit != NULL) && (strlen(model->active_circuit->label) > 0))
		{
			//#ifdef __APPLE__
			//snprintf(prompt, sizeof(prompt), TERMINAL_CYAN "[" APP_NAME " " APP_VERSION"] "TERMINAL_GREEN "\"%s\"" TERMINAL_CYAN" > "TERMINAL_DEFAULT, model->active_circuit->label);
			//#else
			snprintf(prompt, sizeof(prompt), "[" APP_NAME " " APP_VERSION"] \"%s\"  > ", model->active_circuit->label);
			//#endif
		}
		else
		{
			//#ifdef __APPLE__
			//snprintf(prompt, sizeof(prompt), TERMINAL_CYAN "[" APP_NAME " " APP_VERSION"] > "TERMINAL_DEFAULT);
			//#else
			snprintf(prompt, sizeof(prompt), "[" APP_NAME " " APP_VERSION"] > ");
			//#endif
		}

		user_entry = linenoise(prompt);
		if (!user_entry) 
		{
            if (errno == EAGAIN)
            {
                cli_logger(MESS_INFO "CTRL+C disabled, please use 'quit' command to leave the app.");
                continue;
            }
			break;
		}

		if (user_entry[0] != '\0')
		{
			linenoiseHistoryAdd(user_entry);
			scan_user_entry(model, user_entry);
		}
		free(user_entry);
	}

	delete_model(model, true);

	return;
}
#endif

#ifdef DEBUG_MODE
void			run_loop()
{
	char	prompt[256];

	Model* model = create_model();

	model->run_loop = true;

	while(model->run_loop)
	{
		printf("\n\n");
		usleep(10000);

		if ((model->active_circuit != NULL) && (strlen(model->active_circuit->label) > 0))		
		{
			printf("\n" TERMINAL_CYAN "[" APP_DEBUG " " APP_VERSION"] "TERMINAL_GREEN "\"%s\"" TERMINAL_CYAN" > "TERMINAL_DEFAULT, model->active_circuit->label);
		}
		else {
			printf("\n" TERMINAL_CYAN "[" APP_DEBUG " " APP_VERSION"] > "TERMINAL_DEFAULT);
		}

		if (fgets(prompt, sizeof(prompt), stdin) != NULL)
		{
			scan_user_entry(model, prompt);
		}
	}

	delete_model(model, true);

	return;
}
#endif
