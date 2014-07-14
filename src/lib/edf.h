/**
 *
 * EDFLIB is a cross platform EDF reading writing library in C++
 *
 *
 * by @jonbrennecke - github.com/jonbrennecke
 *
 */  

// TODO read edf file into hdf5

#include <fstream>
#include <map>
#include <vector>

namespace edf {

	/**   
	 *
	 * basic data structure for containing an EDF
	 *
	 */
	struct EDF {
		
		// the primary header contains general information about the file
		std::map<std::string,std::string> general_header;

		// the second header contains information about each signal
		std::map< std::string, std::vector<std::string> > signal_header;

		// data records is 2-byte short ints
		std::vector< std::vector<int16_t> > records;

	};


	/**
	 *
	 * Read an EDF file
	 *
	 * :param filename - file name
	 * :return EDF file struct - filled if the file designated by 'filename' exists
	 *		otherwise returns a blank EDF struct
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
				const int samples = atoi( edf->signal_header["num_samples"][i].c_str() );
				
				std::vector<int16_t> rec(samples);

				// convert 2 bytes (little endian) into one int16_t
				for( auto it=rec.begin(); it!=rec.end(); ++it )
				{
					int16_t n = 0;
					n |= (unsigned char) is.get();
					n |= is.get() << 8;
					*it = n;
				}

				edf->records.push_back(rec);
			}
			
			is.close();
		}
		return edf;
	}
}