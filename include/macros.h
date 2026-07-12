//macros.h
#pragma once

#define STRINGIFY(x)				#x
#define TOSTRING(x)					STRINGIFY(x)

#define TERMINAL_BLACK				"\e[0;30m"
#define TERMINAL_GRAY				"\e[38;5;243m"
#define TERMINAL_WHITE				"\e[0;37m"
#define TERMINAL_RED				"\e[0;31m"
#define	TERMINAL_ORANGE				"\e[38;5;173m"
#define TERMINAL_YELLOW				"\e[0;33m"
#define TERMINAL_GREEN				"\e[0;32m"
#define TERMINAL_CYAN				"\e[0;36m"
#define	TERMINAL_BLUE				"\e[0;34m"
#define TERMINAL_PINK				"\e[38;5;218m"
#define TERMINAL_MAGENTA			"\e[38;5;219m"
#define TERMINAL_PURPLE				"\e[0;35m"
#define TERMINAL_VIOLET				"\e[38;5;141m"

#define TERMINAL_DEFAULT			"\e[0;0m"

#define COMPONENTS_COUNT			13
#define MAX_COMMAND_ARGS			5
#define COMPONENTS_IN_PORTS			10
#define LABEL_SIZE_NUM				20

#define LABEL_SIZE					TOSTRING(LABEL_SIZE_NUM)
#define COMPONENTS_IN_PORTS_STR		TOSTRING(COMPONENTS_IN_PORTS)

#define APP_NAME					"simu-logic"
#define APP_VERSION					"v0.14"

#define MESS_ERROR					TERMINAL_RED		"\n/!\\ ERROR : "			TERMINAL_DEFAULT
#define MESS_SYNTAX					TERMINAL_RED		"\n/!\\ WRONG SYNTAX : "	TERMINAL_DEFAULT
#define MESS_INFO					TERMINAL_BLUE		"\n(i) INFO : "				TERMINAL_DEFAULT
#define MESS_TIP					TERMINAL_MAGENTA	"\n(⟐) TIP : "				TERMINAL_DEFAULT
#define	MESS_COMP					TERMINAL_ORANGE		"(▷) "						TERMINAL_DEFAULT
#define	MESS_LINK					TERMINAL_ORANGE		"(→) "						TERMINAL_DEFAULT
#define	MESS_CIRC					TERMINAL_ORANGE		"(◌) "						TERMINAL_DEFAULT


#define COM_OPEN					TERMINAL_YELLOW		"'"
#define COM_CLOSE					TERMINAL_YELLOW		"'"					TERMINAL_DEFAULT
#define STR_OPEN					TERMINAL_ORANGE		" \""
#define STR_CLOSE					"\""				TERMINAL_DEFAULT
#define STR_PATH					TERMINAL_ORANGE		" \"path/to/file\""	TERMINAL_DEFAULT
#define KEYWORD_ALL					TERMINAL_BLUE		" all"				TERMINAL_YELLOW
#define KEYWORD_ACTIVE				TERMINAL_GREEN		" active"			TERMINAL_YELLOW
#define OPTION(option)				TERMINAL_YELLOW		#option 			TERMINAL_DEFAULT
#define OPTION_COM(option)			TERMINAL_YELLOW		" '"					#option 			"'"					TERMINAL_DEFAULT
#define OPTION_INT(option)			TERMINAL_MAGENTA	" " 				#option 			TERMINAL_DEFAULT
#define OPTION_STR(option)			TERMINAL_ORANGE		" \""				#option				"\""				TERMINAL_DEFAULT
