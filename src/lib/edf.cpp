// *************************************************************************************
// 
// 
// 
// EDFLIB - some classes for reading and writing Edf files
// 
// 
// by Jon Brennecke
// 
// 
// *************************************************************************************




#include "edf.h"
#include <fstream>
#include <iostream>
#include <algorithm>





// *************************************************************************************
// 
// Edf class constructor
// 
// :param filename - name of the Edf file as a c-string
// 
// *************************************************************************************

Edf::Edf( const char* filename ) : filename_(filename) 
{
	// open a binary file stream
	std::ifstream is(this->filename_, std::ios::in | std::ios::binary);
	if ( is )
	{
		is >> *this; // deserialize
		is.close();
	}
	else 
	{
		std::cerr << "The file '" << filename << "' could not be located for reading." << std::endl;
	}
}

Edf::~Edf() {}




// *************************************************************************************
// 
// Serialization and Deserialization methods
// 
// *************************************************************************************

std::istream& Edf::edfBasicHeader_t::__deserialize ( std::istream& is, edfBasicHeader_t& header )
{
	if (is.good()) 
	{
		is.get(header.version,sizeof(header.version));
		is.get(header.patientId,sizeof(header.patientId));
		is.get(header.recordId,sizeof(header.recordId));
		is.get(header.start,sizeof(header.start));
		is.get(header.end,sizeof(header.end));
		is.get(header.bytes,sizeof(header.bytes));
		is.get(header.reserved,sizeof(header.reserved));
		is.get(header.nr,sizeof(header.nr));
		is.get(header.duration,sizeof(header.duration));
		is.get(header.ns,sizeof(header.ns));
	}

	return is;
}

std::ostream& Edf::edfBasicHeader_t::__serialize ( std::ostream& os, const edfBasicHeader_t& header )
{
	if (os.good()) 
	{
		// TODO
	}
	return os;
}

std::istream& Edf::edfExtendedHeader_t::__deserialize ( std::istream& is, edfExtendedHeader_t& header )
{

	if (is.good()) 
	{
		is.get(header.labels,sizeof(header.labels));
		is.get(header.transducer,sizeof(header.transducer));
		is.get(header.dimension,sizeof(header.dimension));
		is.get(header.physMin,sizeof(header.physMin));
		is.get(header.physMax,sizeof(header.physMax));
		is.get(header.digMin,sizeof(header.digMin));
		is.get(header.digMax,sizeof(header.digMax));
		is.get(header.prefiltering,sizeof(header.prefiltering));
		is.get(header.samples,sizeof(header.samples));
		is.get(header.reserved,sizeof(header.reserved));
	}

	return is;
}

std::ostream& Edf::edfExtendedHeader_t::__serialize ( std::ostream& os, const edfExtendedHeader_t& header )
{
	if (os.good()) 
	{
		// TODO
	}
	return os;
}


std::istream& Edf::__deserialize ( std::istream& is, Edf& edf )
{
	if (is.good())
	{
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// deserialize the basic header
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		edf.basicHeader_ = Edf::edfBasicHeader_t();
		is >> edf.basicHeader_;

		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// deserialize the extended header
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		// number of samples
		const size_t ns = (const size_t)atoi(edf.basicHeader_.ns);

		edf.extendedHeader_ = (Edf::edfExtendedHeader_t*)malloc(ns * sizeof(Edf::edfExtendedHeader_t));

		size_t i = 0;

		// create 4 extended headers
		for (i = 0; i < ns; ++i)
			edf.extendedHeader_[i] = Edf::edfExtendedHeader_t();

		// blahfurghakdal kill me now
		for (i = 0; i < ns; ++i)
			is.get(edf.extendedHeader_[i].labels,sizeof(edf.extendedHeader_[i].labels));
		for (i = 0; i < ns; ++i)
			is.get(edf.extendedHeader_[i].transducer,sizeof(edf.extendedHeader_[i].transducer));
		for (i = 0; i < ns; ++i)
			is.get(edf.extendedHeader_[i].dimension,sizeof(edf.extendedHeader_[i].dimension));
		for (i = 0; i < ns; ++i)
			is.get(edf.extendedHeader_[i].physMin,sizeof(edf.extendedHeader_[i].physMin));
		for (i = 0; i < ns; ++i)
			is.get(edf.extendedHeader_[i].physMax,sizeof(edf.extendedHeader_[i].physMax));
		for (i = 0; i < ns; ++i)
			is.get(edf.extendedHeader_[i].digMin,sizeof(edf.extendedHeader_[i].digMin));
		for (i = 0; i < ns; ++i)
			is.get(edf.extendedHeader_[i].digMax,sizeof(edf.extendedHeader_[i].digMax));
		for (i = 0; i < ns; ++i)
			is.get(edf.extendedHeader_[i].prefiltering,sizeof(edf.extendedHeader_[i].prefiltering));
		for (i = 0; i < ns; ++i)
			is.get(edf.extendedHeader_[i].samples,sizeof(edf.extendedHeader_[i].samples));
		for (i = 0; i < ns; ++i)
			is.get(edf.extendedHeader_[i].reserved,sizeof(edf.extendedHeader_[i].reserved));

		// std::cout << (is.fail()?"true":"false") << std::endl;


		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// deserialize the data
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		edf.data_ = (double**)malloc(ns * sizeof(double*));

		size_t samples;
		int physMin, physMax, digMin, digMax;
		double scale, dc;
		for (i = 0; i < ns; ++i)
		{
			samples = (size_t)atoi(edf.extendedHeader_[i].samples);
			physMin = (int)atoi(edf.extendedHeader_[i].physMin);
			physMax = (int)atoi(edf.extendedHeader_[i].physMax);
			digMin = (int)atoi(edf.extendedHeader_[i].digMin);
			digMax = (int)atoi(edf.extendedHeader_[i].digMax);
			scale = ((double)(physMax - physMin))/((double)(digMax - digMin));
			dc = (double)physMax - (scale * (double)digMax);

			edf.data_[i] = (double*)malloc(samples * sizeof(double));

			// TODO this can obviously be sped up by buffering the file into chunks
			// this version is not very memory intensive, but you'll end up waiting for the file every
			// iteration of the loop
			// A much faster way would be to buffer the file into chunks, and read from each chunk in the loop
			for (size_t j = 0; j < samples; ++j)
			{
				char c;
				int16_t n = 0;
				is.get(c);
				n |= (unsigned char)c;
				is.get(c); 
				n |= ((unsigned char)c) << 8;
				edf.data_[i][j] = (double)n * scale + dc;
			}
		}
	}

	return is;
}

std::ostream& Edf::__serialize ( std::ostream& os, const Edf& edf )
{
	if (os.good()) 
	{
		// TODO
	}
	return os;
}




// *************************************************************************************
// 
// stream operators for class edfExtendedHeader_t
// 
// *************************************************************************************

// serialize
std::ostream& operator<< ( std::ostream& os, const Edf::edfExtendedHeader_t& header )
{
	return header.__serialize(os,header);
}

// deserialize
std::istream& operator>> ( std::istream& is, Edf::edfExtendedHeader_t& header )
{
	return header.__deserialize(is,header);
}



// *************************************************************************************
// 
// stream operators for class edfBasicHeader_t
// 
// *************************************************************************************

// serialize
std::ostream& operator<< ( std::ostream& os, const Edf::edfBasicHeader_t& header )
{
	return header.__serialize(os,header);
}

// deserialize
std::istream& operator>> ( std::istream& is, Edf::edfBasicHeader_t& header )
{
	return header.__deserialize(is,header);
}



// *************************************************************************************
// 
// stream operators for class Edf
// 
// *************************************************************************************

// serialize
std::ostream& operator<< ( std::ostream& os, const Edf& edf )
{
	return edf.__serialize(os,edf);
}

// deserialize
std::istream& operator>> ( std::istream& is, Edf& edf )
{
	return edf.__deserialize(is,edf);
}
