#include <iostream>
#include <diagmc/functions.h>



int main(int argc, char** argv)
{

	std::cout<<"Esecuzione del programma principale: 5+5="<<somma(5,5)<<'\n';


#ifdef _WIN32
	system("pause");
#endif

	return 0;
}
