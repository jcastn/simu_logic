cmake_minimum_required(VERSION 3.16)

project(simu-logic C)

add_subdirectory(third_party/nfd)

add_executable(simu-logic-app
	src/main.c 
	src/functions-models.c
	src/functions-circuits.c 
	src/functions-components.c 
	src/functions-links.c 
	src/functions-eval.c 
	src/functions-helper.c 
	src/functions-in-out.c 
	src/functions-rearrange-circuit.c
	src/functions-run-loop.c
	src/functions-commands.c
)

target_compile_options(simu-logic-app PRIVATE -Wall -Wextra -Werror)

target_link_libraries(simu-logic-app PRIVATE nfd)