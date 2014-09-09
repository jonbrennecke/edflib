/**
 *
 * EDFLIB is a cross platform EDF reading writing library in C++
 *
 *
 * by @jonbrennecke - github.com/jonbrennecke
 *
 */  

#include <fstream>
#include <map>
#include <vector>
#include <iostream>

namespace edf {

	/**   
	 *
	 * Basic data structure representing an EDF
	 *
 	 * - The primary header contains general information about the file
	 * - The second header contains information about each signal
	 * - The data records is 2-byte short ints that are scaled into doubles as 
	 *   part of the reading process.
	 */
	struct EDF {	
		std::map<std::string,std::string> general_header;
		std::map< std::string, std::vector<std::string> > signal_header;
		std::vector< std::vector<double> > records;
	};


	/**
	 *
	 * Read an EDF file
	 *
	 * :param filename - file name as a string
	 * :return - an EDF file struct that is filled if the file designated by 'filename' exists
	 *		otherwise the returned struct will be empty
	 */
	EDF* read ( std::string filename )
	{
		auto edf = new EDF();

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
				"phys_max", "dig_min", "dig_max", "prefiltering", "num_samples", "signal_reserved" };

			// read the first 256 bits of header data, containing general
			// information about the file
			for( auto iter=h1_sizes.begin(); iter!=h1_sizes.end(); ++iter )
			{
				char *buffer = new char [*iter];
				is.read(buffer,*iter);
				edf->general_header[ general_keys[iter - h1_sizes.begin()] ] = std::string( buffer, (size_t) *iter );
			}

			// cast num_signals to an int
			const int ns = atoi( edf->general_header["num_signals"].c_str() );

			// read the second part of the header
			for( auto iter=h2_sizes.begin(); iter!=h2_sizes.end(); ++iter )
			{
				// read each signal into a vector
				std::vector<std::string> tmp;
				for ( int i = 0; i < ns; ++i )
				{
					char *buffer = new char[*iter];
					is.read(buffer,*iter);
					tmp.push_back( std::string( buffer, (size_t) *iter ) );
				}
				edf->signal_header[ signal_keys[iter - h2_sizes.begin()] ] = tmp;
			}

			// the data record begins at 256 + ( num_signals * 256 ) bytes and is stored as n records of 2 * num_samples bytes
			// values are stored as 2 byte ascii in 2's complement
			for ( int i = 0; i < ns; ++i )
			{
				// retrieve constants from the header
				const int samples = atoi(edf->signal_header["num_samples"][i].c_str());
				const int phys_min = atoi(edf->signal_header["phys_min"][i].c_str());
				const int phys_max = atoi(edf->signal_header["phys_max"][i].c_str());
				const int dig_min = atoi(edf->signal_header["dig_min"][i].c_str());
				const int dig_max = atoi(edf->signal_header["dig_max"][i].c_str());
				const double scale = ((double)(phys_max - phys_min))/((double)(dig_max - dig_min));
				const double dc = phys_max - (scale * dig_max);
				
				// allocate a vector of doubles for each sample
				std::vector<double> rec(samples);

				// convert 2 bytes (little endian) into one int16_t
				for( auto it=rec.begin(); it!=rec.end(); ++it )
				{
					int16_t n = 0;
					n |= (unsigned char) is.get();
					n |= is.get() << 8;
					*it = (double)n * scale + dc;
				}

				edf->records.push_back(rec);
			}
			is.close();
		}
		else {
			std::cerr << "The file '" << filename << "' could not be located for reading." << std::endl;
		}
		return edf;
	}
}