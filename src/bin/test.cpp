#include "edf.hpp"
#include <iostream>

int main (void){
	edf::EDF* file = edf::read("../../BA1216 05_07_2012.edf");

	std::cout << file->general_header["num_signals"] << std::endl;

	return 0;
}