#include "edf.h"
#include <iostream>

int main (void){
	edf::EDF* es = edf::read("../../../test2.edf");

	// std::cout << "============================== GENERAL HEADER ============================== " << std::endl;

	// for ( auto it = es->general_header.begin(); it != es->general_header.end(); ++it)
	// {
	// 	std::cout << it->first << "\t" << it->second << std::endl;
	// }

	// std::cout << "============================== SIGNAL HEADER ============================== " << std::endl;
	
	// for ( auto it = es->signal_header.begin(); it != es->signal_header.end(); ++it)
	// {
	// 	std::cout << it->first << "\t" << it->second[0] << std::endl;
	// }

	for (int i = 0; i < 20; ++i)
	{
		std::cout << es->records[2][i] << std::endl;
	}

	// std::cout << "ns" << file->general_header["num_signals"] << std::endl;
	// std::cout << "" << file->general_header["num_signals"] << std::endl;
	// std::cout << file->general_header["num_signals"] << std::endl;

	return 0;
}