# simu_logic
An app to create and simulate logic circuits. Fully written in C, feel free to contribute ! 



### I. Supported components

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


Other components will be added in the next updates ! 

---

### II. Commands : 

- Build the project with cmake :
`cmake -B build`

- Compile the project it :
`cmake --build build`

- Run it : 
`./build/simu-logic-app`

---

### III. Templates 

The repository contain a `templates` folder, it contains a few cirucits models you can simulate in the `main.c` file. 
