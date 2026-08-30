//src/init-platform.c
#ifdef _WIN32
#include <windows.h>
#endif

void    init_platform()
{
#ifdef _WIN32
	// Force UTF-8 output for Windows
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif
    
}

