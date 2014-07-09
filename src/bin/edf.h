/**
 *
 * EDFLIB is a cross platform EDF reading writing library in C++
 *
 */

// TODO read edf file into hdf5
// compile with --std=c++11

#include <fstream>
#include <map>
#include <vector>

namespace edf {

	struct EDF {
		
		// the primary header contains general information about the file
		std::map<std::string,char*> general_header;

		// the second header contains information about each signal
		std::map< std::string, std::vector<char*> > signal_header;

		// data records is 2-byte short ints
		std::vector<int16_t*> records;

	};


	/**
	 *
	 * Read an EDF file
	 *
	 * :param filename - file name
	 * :return EDF file struct
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
				"phys_max", "dig_min", "dig_max", "prefiltering", "num_samples" };

			// read the first 256 bits of header data, containing general
			// information about the file
			for( auto iter=h1_sizes.begin(); iter!=h1_sizes.end(); ++iter )
			{
				char *buffer = new char [*iter];
				is.read(buffer,*iter);
				edf->general_header[ general_keys[iter - h1_sizes.begin()] ] = buffer;
			}

			// cast num_signals to an int
			const int ns = atoi( edf->general_header["num_signals"] );

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
				edf->signal_header[ signal_keys[iter - h2_sizes.begin()] ] = tmp;
			}

			// the data record begins at 256 + ( num_signals * 256 ) bytes and is stored as n records of 2 * num_samples bytes
			// values are stored as 2 byte ascii in 2's complement
			for (int i = 0; i < ns; ++i)
			{
				const int samples = atoi( edf->signal_header["num_samples"][i] );
				char *buffer = new char[ 2 * samples ];
				int16_t *rec = new int16_t[ samples ];
				is.read(buffer,i);

				for (int j = 0; j < samples; j+=2) {
					rec[j] = buffer[j+1] << 8 | buffer[j]; // little endian
					rec[j] = ~rec[j] + 1; // two's complement
				}

				edf->records.push_back(rec);
			}
			
			is.close();
		}

		return edf;
	}
}