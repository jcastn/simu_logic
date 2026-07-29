# Add it to your path (~/.zshrc or ~/.bashrc) and run : 
# 'simu_logic', 'simu_logic debug' or 'simu_logic clear'
# See README.md file for more details. 

function simu_logic(){
	cd ~/Dev/simu_logic || return 1
	echo -e "(i) INFO : Moved to simu_logic folder\n"

	if [ "$1" = "clear" ]; then
		rm -rf build
		echo -e "(i) INFO : Deleted build folder\n"
	elif [ "$1" = "debug" ]; then
		cmake -B build -DDEBUG_MODE=ON
		echo -e "(i) INFO : Project built in debug mode\n"
		cmake --build build
		echo -e "(i) INFO : Project compiled\n"
		lldb ./build/simu-logic-app
	else
		cmake -B build
		echo -e "(i) INFO : Project built\n"
		cmake --build build
		echo -e "(i) INFO : Project compiled\n"
		./build/simu-logic-app
	fi
}
