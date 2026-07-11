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

For detailed syntax, subcommands, and available arguments, type `help commands` or `<command> help` directly inside the CLI.

- `circuit` : use it to mangage the loaded circuits (alias: `circ` )
	Available options : 
	- `create` (to create a new circuit)
	- `delete` (to delete a circuit)
	- `rename` (to edit the name of a circuit)
	- `list` (to list all the loaded circuits)
	- `select` (to set an active circuit)
	- `unselect` (to unset an active circuit)
	- `show` (print compoennts of a circuit)
	- `simulate` (to simulate the loaded circuits)
	- `import` (to export circuits to an external file)
	- `export` (to import circuits from an external file)

- `component` : Interact with the components inside the active circuit (alias: `comp`)
	Available actions: 
	- `create` (add a component (logic gates/sources/diodes) to the circuit)
	- `delete` (to delete a component)
	- `rename` (to rename a component)
	- `move` (to update the x,y coordinates of a component)

- `link` : Create and manage connections between your circuit components.
	Available actions: 
	- `create` (wire two components through specific input ports)
	- `delete` (delete a link from two components, or all links of a components)
	- `show` (view the inbound and outbound links of a component)

- `help` : Open the user guide.
	Use : 
	- `help commands` to list all availables commands types, 
	- `help aliases` to see all available shorthand commands
	- `help components` to see all components types.

- `quit` : Proprely exit the application .

- `hello` : Display a "Hello World" message.

Tip: Don't hesitate to use the built-in `help` features (e.g., `circuit help`, `link delete help`, `component create help`) to see exact argument lengths and input formats at any time!

---

# III. Compilation Commands : 

- Build the project with cmake :
`cmake -B build`

- Compile the project :
`cmake --build build`

- Run it : 
`./build/simu-logic-app`

---

# IV. Templates & File management

The repository contains a `/templates` folder, it contains a few cirucits you can import and simulate with the app.

The circuits files can be imported using `circuit import` and `circuit export` commands from the terminal. 

---

# V. Dependencies 

- Native File Dialog Extended : https://github.com/btzy/nativefiledialog-extended.git
Used to show a popup from the File Explorer (from any OS) when importing or exporting a file to the app.

