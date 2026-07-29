#  [simu_logic](https://github.com/jcastn/simu_logic)

> A lightweight app to create, manage and simulate logic circuits from the terminal. Fully written in C, from scratch.

![License](https://img.shields.io/badge/license-GPL--3.0-blue)
![Version](https://img.shields.io/badge/version-v0.16.4-green)
![Platform](https://img.shields.io/badge/platform-macOS%20%7C%20Linux%20%7C%20Windows-lightgrey)

---

## Table of Contents

- [Quick Start](#quick-start)
- [I. Major Features](#i-major-features)
	- [1. Components](#1-components)
	- [2. Circuit Engine & Simulation](#2-circuit-engine--real-time-simulation)
	- [3. Topological Sorting](#3-topological-sorting--circuit-structure)
	- [4. Interactive CLI](#4-interactive-cli--user-experience)
- [II. Commands](#ii-app-commands)
- [III. Compilation](#iii-compilation)
	- [1. Prerequisites](#1-prerequisites)
	- [2. Compilation steps](#2-compilation-steps)
	- [3. Debug mode](#3-debug-mode)
	- [4. Helper script](#4-helper-script-simu_logicsh)
- [IV. Templates](#iv-templates--file-management)
	- [1. Circuits templates](#1-circuits-templates)
	- [2. Scripts templates](#2-scripts-templates)
- [V. Dependencies](#v-dependencies)
- [VI. License](#vi-license)

---

## Quick Start

```
git clone --recursive https://github.com/jcastn/simu_logic
cd simu_logic
cmake -B build && cmake --build build
./build/simu-logic-app
```

---

# I. Major Features

## 1. Components

### A. Sources & Constants

| Component     | Description                                         |
|---------------|-----------------------------------------------------|
| `SOURCE`      | Outputs a binary signal (ON or OFF).                |
| `CONST_ON`    | Outputs an always ON binary signal.                 |
| `CONST_OFF`   | Outputs an always OFF binary signal.                |

### B. Diodes & Displays

| Component      | Inputs | Description                                                                                          |
|----------------|--------|------------------------------------------------------------------------------------------------------|
| `DIODE`        | 1      | Indicator that displays the received binary state (ON or OFF).                                       |
| `DIODE_RGB`    | 3      | Indicator that displays a color based on the binary combination of its 3 inputs.                     |
| `DISPLAY_DEC`  | 8      | Displays a decimal number based on 8 inbound binary signals. (max: 127)                              |
| `DISPLAY_HEX`  | 8      | Displays a hexadecimal number based on 8 inbound binary signals. (max: FF)                           |
| `DISPLAY_CHAR` | 8      | Displays an ASCII character based on 8 inbound binary signals. (values between 32 and 126 only)      |

### C. Single-Input Gates (1 input)

| Component  | Description                           |
|------------|---------------------------------------|
| `GATE_NOT` | Inverts the input binary signal.      |
| `BUFFER`   | Replicates the input binary signal.   |

### D. Multi-Input Gates (2 to 8 inputs)

| Component    | Description                                                      |
|--------------|------------------------------------------------------------------|
| `GATE_AND`   | Outputs ON only if **all** inputs are ON.                        |
| `GATE_OR`    | Outputs ON if **at least one** input is ON.                      |
| `GATE_XOR`   | Outputs ON if the number of ON inputs is **odd**.                |
| `GATE_NAND`  | Inverted AND — Outputs ON if **at least one** input is OFF.      |
| `GATE_NOR`   | Inverted OR — Outputs ON only if **all** inputs are OFF.         |
| `GATE_NXOR`  | Inverted XOR — Outputs ON if the number of ON inputs is **even**.|

### E. Implication Gates (2 inputs)

| Component     | Description                         |
|---------------|-------------------------------------|
| `GATE_IMPLY`  | Logical Implication gate.           |
| `GATE_NIMPLY` | Logical Non-Implication gate.       |

### F. Buses (1 to 8 inputs and outputs)

| Component    | Description                                                                   |
|--------------|-------------------------------------------------------------------------------|
| `BUS_NOT`    | Bus of NOT gates — each inbound port is inverted to its linked outbound port. |
| `BUS_BUFFER` | Bus of buffers — each inbound port is replicated to its linked outbound port. |


## 2. Circuit Engine & Real-Time Simulation

- **Multiple Circuits Support:**
	- Load and manipulate multiple circuits at the same time.
	- Create, delete, clone or rename specific circuits without affecting the others.
	- Select an active circuit to work on it independently.

- **Event-Driven Signal Propagation:**
	- When a component state changes or a link is updated, the evaluation is automatically propagated to all affected components and stops once the signal stabilizes.


## 3. Topological Sorting & Circuit Structure

- **Automatic Topological Rearrangement:**
	- The topological sorting algorithm analyzes inbound and outbound dependencies to compute the level of each component in the circuit (treating it as a directed acyclic graph).

- **Dynamic Memory:**
	- Circuit management (which can involve thousands of components and links at the same time) relies on a meticulous system to manage the memory allocations in real-time.

- **Connection Safety & Validation:**
	- Strict validation rules prevent port double-booking, invalid port bounds and orphan connections.


## 4. Interactive CLI & User Experience

- **Rich Feedback & In-App Help:**
	- Embedded formatted help system — type `<command> help` for detailed usage of any command.

- **CLI Navigation ([linenoise](https://github.com/antirez/linenoise) integration):**
	- Auto-completion with <kbd>Tab</kbd>
	- Command history navigation with <kbd>↑</kbd> / <kbd>↓</kbd>
	- Inline line editing with <kbd>←</kbd> / <kbd>→</kbd>
	- Short command aliases (`circ`, `comp`, etc.)

- **Native File Dialogs ([TFD](https://sourceforge.net/projects/tinyfiledialogs) integration):**
	- Use the `IDK` keyword in place of a file path in any `import`, `export` or `run` command to open a file picker dialog (macOS, Windows, Linux).
	- Example: `circuit import all IDK` opens the OS file explorer and the user can select a circuit file to import.

- **Circuits and Scripts Templates :**
	- Import and export circuits as plain text files.
	- Automatically run multiple commands from plain text scripts files. 

---

# II. App commands:

The application contains a lot of commands, making it fully usable from the terminal.

For detailed syntax, subcommands, and available arguments, type `help commands` and `<command> help` directly inside the CLI.

- `circuit`: use it to manage the loaded circuits (alias: `circ`).
	Available options: 
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

- `component`: Interact with the components inside the active circuit (alias: `comp`).
	Available options: 
	- `create` (add a component (logic gates/sources/diodes) to the circuit)
	- `delete` (to delete a component)
	- `rename` (to rename a component)
	- `move` (to update the x,y coordinates of a component)
	- `show` (to show the details of a component and its inbound/outbound links)
	- `toggle` (to toggle the status of a SOURCE component)
	- `set` (to set the status of a SOURCE component to ON or OFF)

- `link`: Create and manage connections between the components of a circuit.
	Available actions: 
	- `create` (create a link between two components)
	- `delete` (delete a link from two components, or all the links of a component)

- `list`: List all the loaded circuits or the content of a circuit.
	Available options:  
	- `circuit` (display a list of all loaded circuits)
	- `components` (display all the components of a circuit)
	- `links` (display all the links of a circuit)

- `help`: Open the user guide.
	Available options: 
	- `commands` to list all available commands types, 
	- `aliases` to see all available shorthand commands
	- `components` to see all components types.

- `quit`: Properly exit the application.

- `reset`: Reset the app to its loading state.

- `run`: Reset the app to its loading state.

- `hello`: Display a "Hello World" message.

---

# III. Compilation

## 1. Prerequisites

### Supported platforms


- MacOS (Sequoia/Tahoe)
- Windows (10/11)
- Linux (Arch, Debian)


### Required tools

- **Git**
- **GCC** (or any C compiler)
- **CMake** (≥ 3.16)

### Platform-specific toolchains

- **Windows:** [w64devkit](https://github.com/skeeto/w64devkit/releases) or MinGW-w64 with CMake.
- **macOS:** Command Line Tools (`xcode-select --install`) or Xcode from the App Store.


## 2. Compilation steps

1. **Clone** the project with its dependencies:
	```bash
	git clone --recursive https://github.com/jcastn/simu_logic
	```

2. **Configure** the build:
	```bash
	cd simu_logic
	cmake -B build                             # macOS / Linux
	cmake -B build -G "MinGW Makefiles"        # Windows
	```

3. **Compile** and **run**:
	```bash
	cmake --build build
	./build/simu-logic-app                     # macOS / Linux
	.\build\simu-logic-app.exe                 # Windows
	```

> **Tip:** Once built, you can recompile and run in one line:
> ```bash
> cmake --build build && ./build/simu-logic-app
> ```


## 3. Debug mode

Build and run with the debugger:
```bash
cmake -B build -DDEBUG_MODE=ON && cmake --build build
lldb ./build/simu-logic-app
```

To switch back to normal mode, clean the build folder first:
```bash
rm -rf build && cmake -B build && cmake --build build
```
 
## 4. Helper script (simu_logic.sh)

A `simu_logic.sh` script is available to automate the build process. Add it to your `PATH` (`~/.bashrc` or `~/.zshrc`) to use these commands from anywhere on your computer:

Commands : 
- `simu_logic debug` : to build, compile and run it in **debug mode**.
- `simu_logic` : to build, compile and run it in **default mode**. 
- `simu_logic clear` : to clear the CMake `build` folder (needed when switching between from `debug` to `default` mode).


---

# IV. Templates (circuits and scripts)

## 1. Circuits templates 
The repository includes a `templates/circuits/` folder with example circuits ready to import:
- `circuit-xor.txt`     : XOR gate built with AND, OR and NOT gates.
- `circuit-rgb.txt`     : RGB diode demo circuit. 
- `circuit-counter.txt` : 8bits binary counter circuit.

You can import circuits using:
```
circuit import "templates/circuits/circuit-xor.txt"
```

Or use `circuit import IDK` to browse for a file using the file picker.


## 2. Scripts templates 
The repository includes a `templates/scripts/` folder with example scripts ready to run : 
- `script-bonjour.txt` : A little demo of how the scripts works. 

You can import scripts using : 
```
run "templates/scripts/script-bonjour.txt"
```

Or use `run` to browse for a file using the file picker. 

---

# V. Dependencies

**simu_logic** uses **2 external libraries**, both included as **git submodules**:

| Library | Description | Integration | License |
|---------------------------------------------------|-------------|-------------|---------|
| [linenoise](https://github.com/antirez/linenoise) | CLI line editing, history, and auto-completion | Single `.c` / `.h` file compiled directly | BSD-2-Clause |
| [Tiny File Dialog](https://sourceforge.net/projects/tinyfiledialogs) | Cross-platform file picker dialogs | Single `.c` / `.h` file compiled directly | No license |

Everything else relies on the **C standard library**.

---

# VI. License

This project is licensed under the **GNU General Public License v3.0** — see the [LICENSE](LICENSE) file for details.