
#include "../bin/edf.hpp"
#include "mex.h"
#include <string>


/**
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
				"duration", "num_signals" };

		int nfields = edfData->general_header.size(); 

		plhs[0] = mxCreateStructMatrix( 1, 1, nfields, fieldnames );

		for( auto it=edfData->general_header.begin(); it!=edfData->general_header.end(); ++it )
		{
			// int i = std::distance(edfData->general_header.begin(), it);
			// edf::trim( it->second );
			const char *fieldvalues[0];
			fieldvalues[0] = it->second;
			mxArray* value = mxCreateCharMatrixFromStrings( (mwSize)1, fieldvalues );
			mxChar *charData = (mxChar *)mxGetData(value);

			for (int j = 0; j < mxGetNumberOfElements(value); ++j)
			{
				mexPrintf("%c\n",charData[j]);
				// if ( charData[j] == (mxChar) 0 )
					// charData[j] = (mxChar) ' ';

			}

			mxSetField( plhs[0], 0, it->first.c_str(), value );
		}
	}
	else {
		mexErrMsgTxt( "File name must be provided as an argument.\n" );
	}
}