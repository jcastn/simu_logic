# simu_logic
A lightweight app to create, manage and simulate logic circuits from the terminal. Fully written in C, from scratch.


# I. Major Features

## 1. Circuit Engine & Real-Time Simulation
- **Multiple Components and Circuits Support:**
	- You have access to multiple types of components : 
		- simple components (SOURCE and DIODE, to product or display binary signals states)
		- single-input gates (NOT, BUFFER).
		- multi-input gates (AND, OR, XOR, NAND, NOR, NXOR, up to 8 inputs).
		- implication gates (IMPLY, NIMPLY, with 2 inputs).
		- number display (DISPLAY_DEC, DISPLAY_HEX, with 8 inputs).
		- rgb led color display (DIODE_RGB, 3 inputs).
	- You can load multiple logic circuits at same time. Also, you're able to load, create, manage, delete or clone specific circuits without affecting the others.

- **Event-Driven Signal Propagation:**
	- When a source state changes or a link is updated, the evaluation of the component state is propagated to the affected components and stops automatically once the signal stabilize.


## 2. Topological Sorting & Circuit Structure
- **Automatic Topological Rearrangement:**
	- The topological sorting algorithm can analyze the inbound and outbound dependencies of components to calculate their level into the circuit (like an oriented graph).

- **Dynamic Memory:**
	- Circuit management (which can involve thousands of components and links at the same time) relies on a meticulous system to manage the memory allocations in real-time.

- **Connection Safety & Validation:** 
	- Strict validation rules preventing port double-booking, invalid port bounds, or orphan connections.



## 3. Interactive CLI & User Experience

- **Rich Feedback & In-App Help:**
	- Embedded formatted help system (`<command> help`) for each command of the app. 

- **CLI Navigation (LineNoise integration):** 
	- Commands auto-completion (with TAB key), commands history navigation (with up/down arrows keys), inline line editing (with left/right arrows keys), and short command aliases (circ, comp, etc.).

- **Native File Dialogs (NFD Integration):**
	- Uses file explorer popups (macOS, Windows, Linux) to import and export external circuit files.

- **Script-Based File Format & Templates:**
	- Import and export circuits and templates as external files. 


## 4. List of supported components 

### A. Sources & Diodes:
- **SOURCE**      : Outputs a binary signal (TRUE or FALSE).
- **DIODE**       : Single-input indicator showing the received binary state (ON = TRUE / OFF = FALSE).
- **DIODE_RGB**   : Three-input indicator displaying a color, based on the binary combination.
- **DISPLAY_DEC** : Display a decimal number depending on the inbound binary signals.
- **DISPLAY_HEX** : Display a hex number depending on the inbound binary signals.
					
### B. Single-Input Gates: (1 input)
- **GATE_NOT**    : Inverts the input binary signal.
- **BUFFER**      : Replicates the input binary signal.

### C. Multi-Input Gates: (2 to 8 inputs)
- **GATE_AND**    : Outputs TRUE only if ALL inputs are TRUE.
- **GATE_OR**     : Outputs TRUE if AT LEAST ONE input is TRUE.
- **GATE_XOR**    : Outputs TRUE if the number of TRUE inputs is ODD.

### D. Inverted Multi-Input Gates: (2 to 8 inputs)
- **GATE_NAND**   : Inverted GATE_AND. Outputs TRUE if AT LEAST ONE input is FALSE.
- **GATE_NOR**    : Inverted GATE_OR. Outputs TRUE only if ALL inputs are FALSE.
- **GATE_NXOR**   : Inverted GATE_XOR. Outputs TRUE if the number of TRUE inputs is EVEN.

### E. Implication Gates (2 inputs):
- **GATE_IMPLY**  : Logical Implication gate.
- **GATE_NIMPLY** : Logical Non-Implication gate.

---

# II. App commands :

The application contains a lot of commands, making it fully usable from the terminal.

For detailed syntax, subcommands, and available arguments, type `help commands` and `<command> help` directly inside the CLI.

- `circuit` : use it to manage the loaded circuits (alias: `circ`)
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

- `quit` : Properly exit the application.

- `reset` : Reset the app to its loading state.

- `hello` : Display a "Hello World" message.

---

# III. Compilation : 

## 1. Needed tools 

Supported Operating Systems (the project should compile/run well if you've installed the right tools) : 
- MacOS (26) 
- Windows (10/11)
- Linux (Arch, Debian)

To compile the project, you need to install :
- Git 
- GCC (C compiler)
- CMake (C build tool)

Toolchains : 
- **Windows devices** : [w64devkit](https://github.com/skeeto/w64devkit/releases) or MinGW-w64 with CMake.
- **Mac devices** : Command Line Tools or XCode App from the Mac App Store (with C package).


## 2. Compilation steps : 

1. Clone the project and the dependencies : 
	- `git clone --recursive https://github.com/jcastn/simu_logic`

2. Move to the simu_logic folder :
	- `cd simu_logic`

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

The repository have a `/templates` folder, which contains a few circuits you can import and simulate with the app.

The circuits files can be imported and exported using `circuit import all` and `circuit export all` commands from the terminal. 

---

# V. Dependencies 

1. Native File Dialog Extended (https://github.com/btzy/nativefiledialog-extended.git) :
	- Used to show a popup from the File Explorer (from any OS) when importing or exporting a file.

2. Linenoise (https://github.com/antirez/linenoise.git) :
	- It adds more functionalities to the CLI :
		- an history of the last typed commands (with up/down arrows keys)
		- moving the cursor when typing a command (with left/right arrows keys)
		- command auto-completion (with tab key).