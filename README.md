# simu_logic
An app to create and simulate logic circuits from the terminal. Fully written in C, from scratch.
Feel free to contribute ! 

---

# I. Supported components

### 1. Sources & Diodes:
- **SOURCE**      : Outputs a binary signal (TRUE or FALSE).
- **DIODE**       : Single-input indicator showing the received binary state (ON = TRUE / OFF = FALSE).
- **DIODE_RGB**   : Three-input indicator displaying a color, based on the binary combination.

### 2. Single-Input Gates: (1 input)
- **GATE_NOT**    : Inverts the input binary signal.
- **BUFFER**      : Replicates the input binary signal.

### 3. Multi-Input Gates: (2 to 10 inputs)
- **GATE_AND**    : Outputs TRUE only if ALL inputs are TRUE.
- **GATE_OR**     : Outputs TRUE if AT LEAST ONE input is TRUE.
- **GATE_XOR**    : Outputs TRUE if the number of TRUE inputs is ODD.

### 4. Inverted Multi-Input Gates: (2 to 10 inputs)
- **GATE_NAND**   : Inverted GATE_AND. Outputs TRUE if AT LEAST ONE input is FALSE.
- **GATE_NOR**    : Inverted GATE_OR. Outputs TRUE only if ALL inputs are FALSE.
- **GATE_NXOR**   : Inverted GATE_XOR. Outputs TRUE if the number of TRUE inputs is EVEN.

### 5. Implication Gates (2 inputs):
- **GATE_IMPLY**  : Logical Implication gate.
- **GATE_NIMPLY** : Logical Non-Implication gate.


---

# II. App commands :

The application contains a lot of commands, making it fully usable from the terminal.

For detailed syntax, subcommands, and available arguments, type `help commands` and `<command> help` directly inside the CLI.

- `circuit` : use it to manage the loaded circuits (alias: `circ` )
	Available options : 
	- `create` (to create a new circuit)
	- `delete` (to delete a circuit)
	- `rename` (to edit the name of a circuit)
	- `duplicate` (to duplicate a circuit)
	- `clear` (to clear a circuit)
	- `select` (to set an active circuit)
	- `unselect` (to unset an active circuit)
	- `simulate` (to simulate the loaded circuits)
	- `rearrange` (to rearrange the components of a circuit by using a topological sort algorithm)
	- `import` (to import circuits from an external file)
	- `export` (to export circuits to an external file)

- `component` : Interact with the components inside the active circuit (alias: `comp`)
	Available options: 
	- `create` (add a component (logic gates/sources/diodes) to the circuit)
	- `delete` (to delete a component)
	- `rename` (to rename a component)
	- `move` (to update the x,y coordinates of a component)
	- `show` (to show the details of a component and its inbound/outbound links)
	- `toggle` (to toggle the status of a SOURCE component)
	- `set` (to set the status of a SOURCE component to ON or OFF)

- `link` : Create and manage connections between the components of a circuit.
	Available actions: 
	- `create` (create a link between two components)
	- `delete` (delete a link from two components, or all the links of a component)

- `list` : List all the loaded circuits or the content of a circuit.
	Available options:  
	- `circuit` (display a list of all loaded circuits)
	- `components` (display all the components of a circuit)
	- `links` (display all the links of a circuit)


- `help` : Open the user guide.
	Available options: 
	- `commands` to list all available commands types, 
	- `aliases` to see all available shorthand commands
	- `components` to see all components types.

- `quit` : Properly exit the application .

- `hello` : Display a "Hello World" message.

---

# III. Compilation : 

## 1. Needed tools 

To compile the project, you need to install :
- Git 
- GCC (C compiler)
- CMake (C build tool)

Toolchains : 
- **Windows devices** : [w64devkit](https://github.com/skeeto/w64devkit/releases) or MinGW-w64 with CMake.
- **Mac devices** : Command Line Tools or XCode from the Mac App Store (with C package).


## 2. Compilation steps : 

1. Clone the project and the dependencies : 
	- `git clone --recursive https://github.com/jcastn/simu_logic`

2. Move to the simu_logic folder :
	- `cd ...../simu_logic`

3. Build the project with cmake :
	- *Mac/Linux : `cmake -B build`*
	- *Windows : `cmake -B build -G "MinGW Makefiles"`*

4. Compile the project :
	- `cmake --build build`

5. Run it :
	- *Mac/Linux : `./build/simu-logic-app`*
	- *Windows : `.\build\simu-logic-app.exe`*

*Note : once the project is built, you can easily compile it and run it with :*
- *Mac/Linux : `cmake --build build && ./build/simu-logic-app`*
- *Windows : `cmake --build build ; .\build\simu-logic-app.exe`*


---

# IV. Templates & File management

The repository contains a `/templates` folder, it contains a few circuits you can import and simulate with the app.

The circuits files can be imported and exported using `circuit import` and `circuit export` commands from the terminal. 

---

# V. Dependencies 

- Native File Dialog Extended (https://github.com/btzy/nativefiledialog-extended.git) :
	- Used to show a popup from the File Explorer (from any OS) when importing or exporting a file.