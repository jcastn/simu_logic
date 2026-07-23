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
	src/commands/commands-list.c
)

target_compile_options(simu-logic-app PRIVATE 
	-Wall
	-Wextra
	-Werror
	-finput-charset=UTF-8
	-fexec-charset=UTF-8
)

if(WIN32)
	target_compile_definitions(simu-logic-app PRIVATE _WIN32_WINNT=0x0600 WINVER=0x0600)
	target_link_libraries(simu-logic-app PRIVATE ole32 uuid shell32)
endif()

target_link_libraries(simu-logic-app PRIVATE nfd)