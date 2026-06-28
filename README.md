# simu_logic
An app to create and simulate logic circuits. Fully written in C, from scratch.
Feel free to contribute ! 

# I. Supported components

### 1. **Components** : 
- SOURCE
- DIODE
- DIODE_RGB (three inputs diode that show the color corresponding to the binary input) 
- LINK (used connect two components)

### 2. **Single input gates** : 
- GATE_NOT (used to invert the binary signal)
- BUFFER (used to replicate the same binary signal)

### 3. **Multi inputs gates** :
- GATE_AND (The gate results True only if all the inputs are True)
- GATE_OR (The gate results True only if at least one of the inputs is True)
- GATE_NAND (Inversion of a GATE_AND, the gate results TRUE only if at least one of the inputs is FALSE) 
- GATE_NOR (Inversion of a GATE_OR, the gate results TRUE only if all the inputs is FALSE) 
- GATE_XOR (The gate results TRUE only if the number of TRUE inputs is odd)
- GATE_NXOR (Inversion of GATE_XOR, the gate results TRUE only if the number of TRUE inputs is even)

### 4. **Other gates** (2 input gates) : 
- GATE_IMPLY
- GATE_NIMPLY

Other components will be added in future updates ! 

---

# II. Commands : 

- Build the project with cmake :
`cmake -B build`

- Compile the project it :
`cmake --build build`

- Run it : 
`./build/simu-logic-app`

---

# III. Templates & File management

The repository contain a `/templates` folder, it contains a few cirucits you can import and simulate with the app. 

The circuits files can be imported using the `file_process()` function in the `main.c` file. 

With this function, you can specify the path with : 
- a NULL value : `import_file(NULL, my_model);`. A popup will appear and let you choose a file to import from the File Explorer. 
- a relative path (from simu_logic folder) : `import_file("templates/circuit_XXX.txt", my_model);`. The specified file will be directly open.
You can also spectify if you want to IMPORT a circuit from a file or EXPORT a cricuit to a file. 

A step by step detailled user guide will be added in a future update ! 

---

# IV. Dependencies 

- Native File Dialog Extended : https://github.com/btzy/nativefiledialog-extended.git
Used to show a popup from the File Explorer (from any OS) when importing or exporting a file to the app.

