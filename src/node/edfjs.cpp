/**
 *
 * Node compatability layer for edflib
 *
 */

#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif

#include "edfjs.h"

using namespace v8;

Persistent<Function> EdfWrapper::constructor;

EdfWrapper::EdfWrapper(const char* filename) : Edf(filename) {}
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
 * Export the class to javascript
 *
 */
void EdfWrapper::Export(Handle<Object> exports, const char* name)
{
	Local<FunctionTemplate> tpl = FunctionTemplate::New(EdfWrapper::New);
	tpl->SetClassName(String::NewSymbol(name));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// prototype
	// tpl->PrototypeTemplate()->Set(String::NewSymbol("get"),
	// 	FunctionTemplate::New(EdfWrapper::Get)->GetFunction());
	// tpl->PrototypeTemplate()->Set(String::NewSymbol("set"),
	// 	FunctionTemplate::New(EdfWrapper::Set)->GetFunction());
	// tpl->PrototypeTemplate()->Set(String::NewSymbol("length"),
	// 	FunctionTemplate::New(EdfWrapper::Length)->GetFunction());

	EdfWrapper::constructor = Persistent<Function>::New(tpl->GetFunction());
	exports->Set(String::NewSymbol(name), EdfWrapper::constructor);
}


void init( Handle<Object> exports ) 
{
	EdfWrapper::Export(exports,"Edf");
}

NODE_MODULE( edflib, init )