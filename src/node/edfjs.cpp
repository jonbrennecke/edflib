/**
 *
 * Node compatability layer for edflib
 *
 */

// #define BUILDING_NODE_EXTENSION
#include <v8.h>
#include <node.h>
#include <string>
#include "../lib/edf.h"

using namespace v8;

/**
 *
 * convert an EDF struct from edflib into a v8 Object
 *
 * :param es - EDF struct
 * :return javascript object with the properties:
 *		- header - array
 *		- records - array of arrays of numbers
 *		
 *
 */
Handle<Value> v8_edf_factory ( edf::EDF * es )
{
	HandleScope scope;
	Handle<Object> v8EDF = Object::New();
	Handle<Object> header = Object::New();

	// the general header is a map of c-strings
	for( auto it=es->general_header.begin(); it!=es->general_header.end(); ++it )
	{
		header->Set( String::New( it->first.c_str() ), 
			String::New( it->second.c_str() ) );
	}

	// the signal header contains a map of vectors
	// each of the vectors is converted into a v8 Array and added to the header Object
	for( auto it=es->signal_header.begin(); it!=es->signal_header.end(); ++it )
	{
		Handle<Array> array = Array::New();
		for( auto it2=it->second.begin(); it2!=it->second.end(); ++it2 )
		{
			array->Set( it2 - it->second.begin(), String::New( (*it2).c_str() ) );
		}	
		header->Set( String::New( it->first.c_str() ), array );
	}

	v8EDF->Set( String::NewSymbol("header"), header );

	// data record
	Handle<Array> records = Array::New();
	for( auto it=es->records.begin(); it!=es->records.end(); ++it )
	{
		const int i = it - es->records.begin();

		Handle<Array> tmp = Array::New();
		for( auto it2=(*it).begin(); it2!=(*it).end(); ++it2 )
			tmp->Set(it2 - (*it).begin(), Number::New( (double)(*it2) ) );

		records->Set( i, tmp );
	}

	v8EDF->Set( String::NewSymbol("records"), records );
	return scope.Close( v8EDF );
}

/**
 *
 * read an edf file
 *
 * V8 wrapper around edf::read
 *
 */
Handle<Value> read(const Arguments& args) 
{
	HandleScope scope;

	if ( ! args.Length() || !args[0]->IsString() )
	{
		ThrowException(Exception::TypeError(String::New( "Invalid input arguments" )));
		return scope.Close( Undefined() );
	}

	std::string filename = *String::Utf8Value( args[0]->ToString() );

	edf::EDF * edfStruct = edf::read( filename );

	return scope.Close( v8_edf_factory( edfStruct ) );
}



void init( Handle<Object> exports ) 
{
	exports->Set( String::NewSymbol( "read" ),
		FunctionTemplate::New( read )->GetFunction() );
}

NODE_MODULE( edflib, init )