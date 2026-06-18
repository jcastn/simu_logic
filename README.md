# simu_logic
An app to create and simulate logic circuits. Fully written in C, feel free to contribute ! 

List of supported components : 
- Components : 
	- SOURCE 
	- DIODE 
	- LINK 

- Single input gates : 
	- GATE_NOT
	- BUFFER

- Two inputs gates : 
	- GATE_IMPLY
	- GATE_NIMPLY

- Multi inputs gates : 
	- GATE_AND
	- GATE_OR
	- GATE_NAND
	- GATE_NOR
	- GATE_XOR
	- GATE_NXOR



Commands : 
- Complilation : 
`cd "PATH/TO/FOLDER/simu_logic/" && mkdir -p build &&  gcc -Wall -Wextra -Werror main.c functions.c functions-eval.c functions-helper.c functions-in-out.c functions-rearrange-circuit.c -o build/main && ./build/main`
