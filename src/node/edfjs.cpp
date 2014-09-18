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

	// get the data
	double* data = self->data_[index];
	Handle<Array> array = Array::New();

	size_t samples = (size_t)atoi(self->extendedHeader_[index]->samples);
	std::cout << samples << std::endl;
	for(size_t i = 0; i < samples; i++) {
		array->Set( i, Number::New(data[i]) );
	}

	return scope.Close(array);
}

// a couple awful little macros
#define STRINGIFY(var) #var
#define DOFIELD(field,header,obj) (obj)->Set( String::New(STRINGIFY((field))), (header)->(version));

Handle<Value> EdfWrapper::GetHeader(const Arguments& args)
{
	// open scope and unwrap object
	HandleScope scope;
	EdfWrapper *self = node::ObjectWrap::Unwrap<EdfWrapper>(args.This());


	Handle<Object> header = Object::New();

	DOFIELD(version,self->basicHeader_,header)
	DOFIELD(patientId,self->basicHeader_,header)
	DOFIELD(recordId,self->basicHeader_,header)
	DOFIELD(start,self->basicHeader_,header)
	DOFIELD(end,self->basicHeader_,header)
	DOFIELD(bytes,self->basicHeader_,header)
	DOFIELD(reserved,self->basicHeader_,header)
	DOFIELD(nr,self->basicHeader_,header)
	DOFIELD(duration,self->basicHeader_,header)
	DOFIELD(ns,self->basicHeader_,header)
	
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
