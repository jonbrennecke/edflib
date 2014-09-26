#ifndef EDF_H
#define EDF_H

#include <fstream>

class Edf
{
private:

	struct edfBasicHeader_t
	{
		char version[9];
		char patientId[81];
		char recordId[81];
		char startdate[9];
		char starttime[9];
		char bytes[9];
		char reserved[45];
		char nr[9];
		char duration[9];
		char ns[5];

		static std::istream& __deserialize(std::istream&, edfBasicHeader_t&);
		static std::ostream& __serialize(std::ostream&, const edfBasicHeader_t&);

		friend std::istream& operator>> (std::istream&, Edf::edfBasicHeader_t& );
		friend std::ostream& operator<< (std::ostream&, const Edf::edfBasicHeader_t&);
	};

	struct edfExtendedHeader_t
	{
		char labels[17];
		char transducer[81];
		char dimension[9];
		char physMin[9];
		char physMax[9];
		char digMin[9];
		char digMax[9];
		char prefiltering[81];
		char samples[9];
		char reserved[33];

		static std::istream& __deserialize(std::istream&, edfExtendedHeader_t&);
		static std::ostream& __serialize(std::ostream&, const edfExtendedHeader_t&);

		friend std::istream& operator>> (std::istream&, Edf::edfExtendedHeader_t& );
		friend std::ostream& operator<< (std::ostream&, const Edf::edfExtendedHeader_t&);
	};

protected:
	edfBasicHeader_t basicHeader_;
	edfExtendedHeader_t* extendedHeader_;
	double** data_; // array of data arrays
	const char* filename_;
	static std::istream& __deserialize(std::istream&, Edf&);
	static std::ostream& __serialize(std::ostream&, const Edf& edf);

public:

	Edf( const char* );
	Edf();
	~Edf();

	// stream operators need to be able to access the protected 
	// static serialization/deserialization methods of Edf and 
	// it's private header clases
	friend std::ostream& operator<<( std::ostream&, const Edf& );
	friend std::istream& operator>>( std::istream&, Edf& );
	friend std::istream& operator>> (std::istream&, Edf::edfBasicHeader_t& );
	friend std::ostream& operator<< (std::ostream&, const Edf::edfBasicHeader_t&);
	friend std::istream& operator>> (std::istream&, Edf::edfExtendedHeader_t& );
	friend std::ostream& operator<< (std::ostream&, const Edf::edfExtendedHeader_t&);

	friend class EdfWrapper;
};

// stream operators for edfExtendedHeader_t
std::istream& operator>> (std::istream&, Edf::edfExtendedHeader_t&);
std::ostream& operator<< (std::ostream&, const Edf::edfExtendedHeader_t&);

// stream operators for edfBasicHeader_t
std::istream& operator>> (std::istream&, Edf::edfBasicHeader_t&);
std::ostream& operator<< (std::ostream&, const Edf::edfBasicHeader_t&);

// stream operators for Edf
std::istream& operator>> (std::istream&, Edf&);
std::ostream& operator<< (std::ostream&, const Edf&);

#endif