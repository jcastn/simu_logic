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

- Build the project with cmake :
`cmake -B build`

- Compile the project and run it :
`cmake --build build`

- Run it : 
`./build/simu-logic-app`

