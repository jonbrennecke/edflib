
#include "../lib/edf.h"
#include "mex.h"
#include <string>


/**
 *
 * read an EDF file
 *
 * :param nlhs - Number of output (left-side) arguments, or the size of the plhs array.
 * :param plhs - Array of output arguments.
 * :param nrhs - Number of input (right-side) arguments, or the size of the prhs array.
 * :param prhs - Array of input arguments.
 *
 */
void mexFunction ( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if ( nrhs > 0 && prhs[0] ) 
	{
		char * filename = mxArrayToString(prhs[0]);
		edf::EDF* edfData = edf::read( std::string( filename ) );
		mxFree(filename);

		const char *fieldnames[] = { 
				"version", "patient_id", "rec_id", "start_date",
				"end_time", "header_bytes", "reserved", "num_items",
				"duration", "num_signals", "labels", "transducer", 
				"dimension", "phys_min", "phys_max", "dig_min", 
				"dig_max", "prefiltering", "num_samples", "signal_reserved" };

		int nfields = edfData->general_header.size() + edfData->signal_header.size(); 

		// return the header as the first argument
		plhs[0] = mxCreateStructMatrix( 1, 1, nfields, fieldnames );

		// general header
		for( auto it=edfData->general_header.begin(); it!=edfData->general_header.end(); ++it )
		{
			const char *fieldvalues[0];
			fieldvalues[0] = it->second.c_str();
			mxArray* value = mxCreateCharMatrixFromStrings( 1, fieldvalues );
			mxSetField( plhs[0], 0, it->first.c_str(), value );
		}

		// signal header
		for( auto it=edfData->signal_header.begin(); it!=edfData->signal_header.end(); ++it )
		{
			const char *fieldvalues[ edfData->signal_header.size() ];
			for( auto it2=it->second.begin(); it2!=it->second.end(); ++it2 )
			{
				size_t i = std::distance( it->second.begin(), it2 );
				fieldvalues[i] = ( *it2 ).c_str();
			}
			mxArray* value = mxCreateCharMatrixFromStrings( edfData->signal_header.size(), fieldvalues );
			mxSetField( plhs[0], 0, it->first.c_str(), value );
		}

		// return records as second return value
		plhs[1] = mxCreateCellMatrix( edfData->records.size() + 1, 1 );
		for( auto it=edfData->records.begin(); it!=edfData->records.end(); ++it )
		{
			const int i = it - edfData->records.begin();
			const int samples = atoi( edfData->signal_header["num_samples"][ i ].c_str() );
			mxArray * rec = mxCreateDoubleMatrix( samples, 1, mxREAL );

			double *ptr = mxGetPr(rec);

			for( auto it2=(*it).begin(); it2!=(*it).end(); ++it2 )
				ptr[ it2 - (*it).begin() ] = (int)*it2;

			// TODO
			// each record in edfData is an array of int16_t's, so we can optimize this by
			// using a typed numeric matrix
			// mxArray * rec = mxCreateNumericMatrix( samples, 1, mxINT16_CLASS, mxREAL );
			// memcpy( &ptr, &(*it), samples * 2 );

			mxSetCell( plhs[1], i, rec );
		}
	}
	else {
		mexErrMsgTxt( "File name must be provided as an argument.\n" );
	}
}