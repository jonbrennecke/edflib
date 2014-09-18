#ifndef EDFJS_H
#define EDFJS_H

#include "../lib/edf.h"
#include <v8.h>
#include <node.h>

using namespace v8;

class EdfWrapper : public node::ObjectWrap, public Edf
{
private:
	static Persistent<Function> constructor;
public:
	static Handle<Value> New(const Arguments& args);
	static void Export(Handle<Object> exports, const char* name);
	static Handle<Value> GetData(const Arguments& args);
	static Handle<Value> GetHeader(const Arguments& args);

	EdfWrapper(const char* filename);
	EdfWrapper();
	~EdfWrapper();
	
};

#endif