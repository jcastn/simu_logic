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

The repository contain a `/templates` folder, it contains a few cirucits you can import and simulate in a model in the `main.c` file. 


A Model is structure that contains multiple circuits and a circuit contains multiple linked components. 

The circuits files can be imported using the `import_file()` function in the `main.c` file. 

With this function, you can specify : 
- a NULL value : `import_file(NULL, my_model);`. A popup will appear and let you choose a file to import from the File Explorer. 
- a relative path (from simu_logic folder) : `import_file("templates/circuit_XXX.txt", my_model);`. The specified file will be directly open. 

