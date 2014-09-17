#ifndef EDF_H
#define EDF_H

#include <fstream>

class Edf
{
public:
	Edf( const char* );
	~Edf();

	// stream operators need to be able to access the protected 
	// static serialization/deserialization methods
	friend std::ostream& operator<<( std::ostream&, const Edf& );
	friend std::istream& operator>>( std::istream&, Edf& );

protected:
	static std::istream& __deserialize ( std::istream& is, Edf& );
	static std::ostream& __serialize ( std::ostream& os, const Edf& edf );

private:

	struct edfBasicHeader
	{
		char version[8];
		char patientId[80];
		char recordId[80];
		char start[8];
		char end[8];
		char bytes[8];
		char reserved[44];
		char nr[8];
		char duration[8];
		char ns[4];
	};

	struct edfExtendedHeader
	{
		char labels[16];
		char transducer[80];
		char dimension[8];
		char physMin[8];
		char physMax[8];
		char digMin[8];
		char digMax[8];
		char prefiltering[80];
		char samples[8];
		char reserved[32];
	};

	double** data_; // array of data arrays
	const char* filename_;
	edfBasicHeader* basicHeader_;
	edfExtendedHeader** extendedHeader_;
};

// stream operators
std::istream& operator>> ( std::istream& is, Edf& );
std::ostream& operator<< ( std::ostream& os, const Edf& edf );

#endif