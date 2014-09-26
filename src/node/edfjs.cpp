/**
 *
 * Node compatability layer for edflib
 *
 */

#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif

#include "edfjs.h"

#include <iostream>

using namespace v8;

Persistent<Function> EdfWrapper::constructor;

EdfWrapper::EdfWrapper(const char* filename) : Edf(filename) {}
EdfWrapper::EdfWrapper(){}
EdfWrapper::~EdfWrapper(){}

Handle<Value> EdfWrapper::New(const Arguments& args)
{
	// Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope;

	// Invoked as constructor: `new MyObject(...)`
	if (args.IsConstructCall()) {
		String::Utf8Value filename(args[0]->IsUndefined() ? String::New("") : args[0]->ToString());
		EdfWrapper* obj = new EdfWrapper((const char*)(*filename));
		obj->Wrap(args.This());
		return args.This();
	
	// Invoked as plain function `MyObject(...)`, turn into construct call.
	} else {
		const int argc = 1;
		Local<Value> argv[argc] = { args[0] };
		return scope.Close(EdfWrapper::constructor->NewInstance(argc, argv));
	}
}

/**
 *
 * Return the array value at an index
 *
 */
Handle<Value> EdfWrapper::GetData(const Arguments& args)
{
	// open scope and unwrap object
	HandleScope scope;
	EdfWrapper *self = node::ObjectWrap::Unwrap<EdfWrapper>(args.This());

	// read argument
	size_t index = (size_t)args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
	
	// retrieve the number of samples from the header
	size_t samples = (size_t)atoi(self->extendedHeader_[index].samples);

	// get the data
	double* data = self->data_[index];
	Handle<Array> array = Array::New(samples);

	for(size_t i = 0; i < samples; i++)
		array->Set( i, Number::New(data[i]) );

	return scope.Close(array);
}


/**
 *
 * Pardon me while I barf out some code here.
 *
 */
Handle<Value> EdfWrapper::GetHeader(const Arguments& args)
{
	// open scope and unwrap object
	HandleScope scope;
	EdfWrapper *self = node::ObjectWrap::Unwrap<EdfWrapper>(args.This());

	Handle<Object> header = Object::New();

	// translate the basic header into a v8 object
	header->Set( String::New("version"), String::New(self->basicHeader_.version));
	header->Set( String::New("patientId"), String::New(self->basicHeader_.patientId));
	header->Set( String::New("recordId"), String::New(self->basicHeader_.recordId));
	header->Set( String::New("startdate"), String::New(self->basicHeader_.startdate));
	header->Set( String::New("starttime"), String::New(self->basicHeader_.starttime));
	header->Set( String::New("bytes"), String::New(self->basicHeader_.bytes));
	header->Set( String::New("reserved"), String::New(self->basicHeader_.reserved));
	header->Set( String::New("nr"), String::New(self->basicHeader_.nr));
	header->Set( String::New("duration"), String::New(self->basicHeader_.duration));
	header->Set( String::New("ns"), String::New(self->basicHeader_.ns));

	const size_t ns = (const size_t)atoi(self->basicHeader_.ns);

	// prepare arrays for the extended header 
	Handle<Array> labels = Array::New(ns);
	Handle<Array> transducer = Array::New(ns);
	Handle<Array> dimension = Array::New(ns);
	Handle<Array> physMin = Array::New(ns);
	Handle<Array> physMax = Array::New(ns);
	Handle<Array> digMin = Array::New(ns);
	Handle<Array> digMax = Array::New(ns);
	Handle<Array> prefiltering = Array::New(ns);
	Handle<Array> samples = Array::New(ns);
	Handle<Array> reserved = Array::New(ns);

	for (size_t i = 0; i < ns; ++i)
	{
		labels->Set(i, String::New(self->extendedHeader_[i].labels));
		transducer->Set(i, String::New(self->extendedHeader_[i].transducer));
		dimension->Set(i, String::New(self->extendedHeader_[i].dimension));
		physMin->Set(i, String::New(self->extendedHeader_[i].physMin));
		physMax->Set(i, String::New(self->extendedHeader_[i].physMax));
		digMin->Set(i, String::New(self->extendedHeader_[i].digMin));
		digMax->Set(i, String::New(self->extendedHeader_[i].digMax));
		prefiltering->Set(i, String::New(self->extendedHeader_[i].prefiltering));
		samples->Set(i, String::New(self->extendedHeader_[i].samples));
		reserved->Set(i, String::New(self->extendedHeader_[i].reserved));
	}

	// assign the arrays to the header object
	header->Set(String::New("labels"), labels);
	header->Set(String::New("transducer"), transducer);
	header->Set(String::New("dimension"), dimension);
	header->Set(String::New("physMin"), physMin);
	header->Set(String::New("physMax"), physMax);
	header->Set(String::New("digMin"), digMin);
	header->Set(String::New("digMax"), digMax);
	header->Set(String::New("prefiltering"), prefiltering);
	header->Set(String::New("samples"), samples);
	header->Set(String::New("reserved"), reserved);

	return scope.Close(header);
}

/**
 *
 * Export the class to javascript
 *
 */
void EdfWrapper::Export(Handle<Object> exports, const char* name)
{
	Local<FunctionTemplate> tpl = FunctionTemplate::New(EdfWrapper::New);
	tpl->SetClassName(String::NewSymbol(name));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// prototype
	tpl->PrototypeTemplate()->Set(String::NewSymbol("getData"),
		FunctionTemplate::New(EdfWrapper::GetData)->GetFunction());
	tpl->PrototypeTemplate()->Set(String::NewSymbol("getHeader"),
		FunctionTemplate::New(EdfWrapper::GetHeader)->GetFunction());

	constructor = Persistent<Function>::New(tpl->GetFunction());
	exports->Set(String::NewSymbol(name), constructor);
}


void init( Handle<Object> exports, Handle<Object> module ) 
{
	EdfWrapper::Export(exports,"Edf");
}

NODE_MODULE( edflib, init )
