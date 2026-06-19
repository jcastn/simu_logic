cmake_minimum_required(VERSION 3.16)

project(simu-logic C)

add_subdirectory(third_party/nfd)

add_executable(simu-logic-app
	src/main.c 
	src/functions.c 
	src/functions-eval.c 
	src/functions-helper.c 
	src/functions-in-out.c 
	src/functions-rearrange-circuit.c
)

target_compile_options(simu-logic-app PRIVATE -Wall -Wextra -Werror)

target_link_libraries(simu-logic-app PRIVATE nfd)