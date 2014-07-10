// #include <octave/oct.h>

#include "../bin/edf.hpp"
#include "mex.h"
#include "mexutils.hpp"
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
	// edf::EDF* edfStruct = edf::read( std::string( cstrFromMx(prhs[0]) ) );

	// for( auto iter=edfStruct->general_header.begin(); iter!=edfStruct->general_header.end(); ++iter )
	// {
	// 	mexPrintf(iter->first.c_str() );
	// }

	// mxArray *mxCreateStructArray()


}