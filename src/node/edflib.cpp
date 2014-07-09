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
#include "../bin/edf.h"

using namespace v8;


/**
 *
 * convert an edf::EDF struct into a v8 Object
 *
 */
Handle<Object> v8_edf_factory ( edf::EDF * edfStruct )
{
	Handle<Object> header = Object::New();

	const std::vector<std::string> general_keys = {
		"version", "patient_id", "rec_id", "start_date",
		"end_time", "header_bytes", "reserved", "num_items",
		"duration", "num_signals" }; 
	const std::vector<std::string> signal_keys = { 
		"labels", "transducer", "dimension", "phys_min",
		"phys_max", "dig_min", "dig_max", "prefiltering", "num_samples" };

	for( auto iter=general_keys.begin(); iter!=general_keys.end(); ++iter )
	{
		header->Set( String::New( (*iter).c_str() ), 
			String::New( edfStruct->general_header[*iter] ) );
	}

	for( auto iter=signal_keys.begin(); iter!=signal_keys.end(); ++iter )
	{
		header->Set( String::New( (*iter).c_str() ), 
			String::New( edfStruct->signal_header[*iter] ) );
	}

	return header;
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