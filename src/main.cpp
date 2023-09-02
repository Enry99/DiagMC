#include <iostream>
#include <diagmc/simulation.h>
#include <diagmc/diagram.h>



int main(int argc, char** argv)
{

	std::cout<<"Esecuzione del programma principale:\n";


	SingleRunResults res = run_simulation(1,1,1,1,50'000'000, 1'000'000);

	res.print_results();


	#ifdef _WIN32
	system("pause");
	#endif

	return 0;
}
