cmake_minimum_required(VERSION 3.16)

project(simu-logic C)

add_subdirectory(third_party/nfd)

add_executable(simu-logic-app
	src/core/main.c
	src/core/functions-models.c
	src/core/functions-circuits.c 
	src/core/functions-components.c 
	src/core/functions-console-output.c 
	src/core/functions-links.c 
	src/core/functions-eval.c 
	src/core/functions-helper.c 
	src/core/functions-in-out.c 
	src/core/functions-rearrange-circuit.c
	src/core/functions-run-loop.c
	src/commands/commands.c
	src/commands/commands-circuit.c
	src/commands/commands-component.c
	src/commands/commands-link.c
)

target_compile_options(simu-logic-app PRIVATE -Wall -Wextra -Werror)

target_link_libraries(simu-logic-app PRIVATE nfd)