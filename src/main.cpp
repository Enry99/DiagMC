/**
 * @file main.cpp
 * @brief Main function of the executable for the Diagrammatic Monte Carlo code for a 2-level spin system in a magnetic field.
 * It reads the settings from 'settings.json' file by default. A different filename can be provided as a command-line argument upon execution.
 * @author Enrico Pedretti
 * @date 2023-09-03
 */

#include <iostream>
#include <diagmc/setup.h>



int main(int argc, char** argv)
{

	std::cout<<"Diagrammatic Monte Carlo code for a two level spin sistem in a magnetic field.\n\n";


	//launch the calculations, optionally specifying which settings file to use by passing it as a command-line argument
	launch_calculations(argc == 2 ? argv[1] : "settings.json");


	//option to avoid the terminal to close after the execution if run by double-click on the .exe file on Windows
	#ifdef _WIN32
	system("pause");
	#endif

	return 0;
}
