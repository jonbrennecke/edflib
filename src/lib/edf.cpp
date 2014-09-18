#include "edf.h"
#include <fstream>
#include <iostream>
#include <algorithm>

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

std::istream& Edf::__deserialize ( std::istream& is, Edf& edf )
{
	if (is.good())
	{
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// deserialize the basic header
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		edf.basicHeader_ = (Edf::edfBasicHeader_t*)malloc(sizeof(Edf::edfBasicHeader_t));
		is.read(reinterpret_cast<char*>(edf.basicHeader_),sizeof(Edf::edfBasicHeader_t));

		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// deserialize the extended header
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		// number of samples
		const size_t ns = (const size_t)atoi(edf.basicHeader_->ns);

		// TODO if I try to realloc here, then for some reason GCC complains about realloc'ing a pointer that's never been malloc'ed
		// even if comparing to NULL should catch that
		edf.extendedHeader_ = (Edf::edfExtendedHeader_t**)malloc(ns * sizeof(Edf::edfExtendedHeader_t*));

		for (size_t i = 0; i < ns; ++i)
		{
			edf.extendedHeader_[i] = (Edf::edfExtendedHeader_t*)malloc(sizeof(Edf::edfExtendedHeader_t));
			is.read(reinterpret_cast<char*>(edf.extendedHeader_[i]),sizeof(Edf::edfExtendedHeader_t));
		}

		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// deserialize the extended header
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		edf.data_ = (double**)malloc(ns * sizeof(double*));

		size_t samples, i;
		int physMin, physMax, digMin, digMax;
		double scale, dc;
		for (i = 0; i < ns; ++i)
		{
			samples = (size_t)atoi(edf.extendedHeader_[i]->samples);
			physMin = (int)atoi(edf.extendedHeader_[i]->physMin);
			physMax = (int)atoi(edf.extendedHeader_[i]->physMax);
			digMin = (int)atoi(edf.extendedHeader_[i]->digMin);
			digMax = (int)atoi(edf.extendedHeader_[i]->digMax);
			scale = ((double)(physMax - physMin))/((double)(digMax - digMin));
			dc = physMax - (scale * digMax);

			edf.data_[i] = (double*)malloc(samples * sizeof(double));

			for (size_t j = 0; j < samples; ++j)
			{
				int16_t n = 0;
				n |= (unsigned char) is.get();
				n |= is.get() << 8;
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
