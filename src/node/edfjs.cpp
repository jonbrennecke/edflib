/**
 *
 * Node compatability layer for edflib
 *
 */

// #define BUILDING_NODE_EXTENSION
#include <v8.h>
#include <node.h>
#include <string>
#include <iostream>
#include <typeinfo>
#include "../bin/edf.hpp"

using namespace v8;

/**
 *
 * convert an vector into a v8 Array
 *
 */
template <class T>
Handle<Array> v8_array_factory ( std::vector<T> vec ) {
	HandleScope scope;
	Handle<Array> array = Array::New();

	for( auto iter=vec.begin(); iter!=vec.end(); ++iter )
	{
		array->Set( iter - vec.begin(), Handle<T>(*iter) );
	}
	// array->Set(0, Integer::New(x));
	// array->Set(1, Integer::New(y));
	// array->Set(2, Integer::New(z));


	return scope.Close(array);
}


/**
 *
 * convert an EDF struct from edflib into a v8 Object
 *
 */
Handle<Object> v8_edf_factory ( edf::EDF * es )
{
	HandleScope scope;
	Handle<Object> header = Object::New();

	for( auto iter=es->general_header.begin(); iter!=es->general_header.end(); ++iter )
	{
		header->Set( String::New( iter->first.c_str() ), 
			String::New( iter->second ) );
	}

	for( auto iter=es->signal_header.begin(); iter!=es->signal_header.end(); ++iter )
	{
		v8_array_factory(iter->second);
		// header->Set( String::New( (*iter).c_str() ), 
			// String::New( edfStruct->signal_header[*iter] ) );
	}

	return scope.Close(header);
}

/**
 *
 * read an edf
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

	// Handle<Object> header = Object::New();
 //    Result->Set(String::New("header"), String::New("Stackoverflow"));
 //    Result->Set(String::New("url"), String::New("http://stackoverflow.com"));
 //    Result->Set(String::New("javascript_tagged"), Number::New(317566));

	return scope.Close( v8_edf_factory( edfStruct ) );
}



void init( Handle<Object> exports ) 
{
	exports->Set( String::NewSymbol( "read" ),
		FunctionTemplate::New( read )->GetFunction() );
}

NODE_MODULE( edflib, init )