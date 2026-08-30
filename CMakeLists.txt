cmake_minimum_required(VERSION 3.16)

project(simu-logic C)

add_executable(simu-logic-app
	third_party/linenoise/linenoise.c
	third_party/tinyfiledialogs/tinyfiledialogs.c

	src/main.c

	src/core/models.c
	src/core/circuits.c 
	src/core/components.c 
	src/core/links.c 
	src/core/eval.c 
	src/core/helper.c 
	src/core/in-out.c 
	src/core/rearrange-circuit.c
	src/core/init-platform.c

	src/cli/console-output.c 
	src/cli/run-loop.c
	src/cli/user-entry.c
	src/cli/commands.c
	src/cli/commands-map.c
	src/cli/commands-help.c
	src/cli/commands-circuit.c
	src/cli/commands-component.c
	src/cli/commands-link.c
	src/cli/commands-list.c
)

option(DEBUG_MODE "Enable debug mode" OFF)

target_compile_options(simu-logic-app PRIVATE 
	-Wall
	-Wextra
	-Werror
	#-g
	-finput-charset=UTF-8
	-fexec-charset=UTF-8
)

set_source_files_properties(
	third_party/linenoise/linenoise.c
	third_party/tinyfiledialogs/tinyfiledialogs.c
	PROPERTIES COMPILE_FLAGS "-w"
)

if(DEBUG_MODE)
	target_compile_definitions(simu-logic-app PRIVATE DEBUG_MODE)
	target_compile_options(simu-logic-app PRIVATE -g)
endif()

if(WIN32)
	target_compile_definitions(simu-logic-app PRIVATE _WIN32_WINNT=0x0600 WINVER=0x0600)
	target_link_libraries(simu-logic-app PRIVATE ole32 uuid shell32)
endif()
