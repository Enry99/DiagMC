#include <iostream>
#include <diagmc/setup.h>



int main(int argc, char** argv)
{

	std::cout<<"Diagrammatic Monte Carlo code for a two level spin sistem in a magnetic field.\n\n";



	launch_calculations(argc == 2 ? argv[1] : "settings.json");


	#ifdef _WIN32
	system("pause");
	#endif

	return 0;
}
