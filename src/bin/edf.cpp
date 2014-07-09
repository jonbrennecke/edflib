// read edf file into hdf5
// compile with --std=c++11

#include <fstream>
#include <map>
#include <iostream>
#include <vector>


// struct EDF {

// };


int main (void) {

	const char *filename = "../BA1216 05_07_2012.edf";

	// open a binary file stream
	std::ifstream is(filename, std::ios::in | std::ios::binary);
	if ( is )
	{
		// constant header sizes and keys
		const std::vector<int> h1_sizes = { 8,80,80,8,8,8,44,8,8,4 };
		const std::vector<int> h2_sizes = { 16,80,8,8,8,8,8,80,8,32 };
		const std::vector<std::string> general_keys = {
			"version", "patient_id", "rec_id", "start_date",
			"end_time", "header_bytes", "reserved", "num_items",
			"duration", "num_signals" }; 
		const std::vector<std::string> signal_keys = { 
			"labels", "transducer", "dimension", "phys_min",
			"phys_max", "dig_min", "dig_max", "prefiltering", "num_samples" };

		// read the first 256 bits of header data, containing general
		// information about the file
		std::map<std::string,char*> general_header;
		for( auto iter=h1_sizes.begin(); iter!=h1_sizes.end(); ++iter )
		{
			char *buffer = new char [*iter];
			is.read(buffer,*iter);
			general_header[ general_keys[iter - h1_sizes.begin()] ] = buffer;
		}

		// the second header contains information about each signal
		std::map< std::string, std::vector<char*> > signal_header;

		// cast num_signals to an int
		const int ns = atoi( general_header["num_signals"] );

		// read the second part of the header
		for( auto iter=h2_sizes.begin(); iter!=h2_sizes.end(); ++iter )
		{
			// read each signal into a vector
			std::vector<char*> tmp;
			for (int i = 0; i < ns; ++i)
			{
				char *buffer = new char [*iter];
				is.read(buffer,*iter);
				tmp.push_back( buffer );
			}
			signal_header[ signal_keys[iter - h2_sizes.begin()] ] = tmp;
		}


		is.close();
	}

	return 0;
}
