#ifndef PORT_H
#define PORT_H

#include <node.h>
#include <node_object_wrap.h>

using v8::Function;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::Persistent;
using v8::PropertyCallbackInfo;
using v8::String;
using v8::Value;

class Port : public node::ObjectWrap {
 public:
  static void Init(Local<Object> exports);

 private:
  explicit Port(Isolate* isolate, int num, int mode, bool set);
  ~Port();

  static void New(const FunctionCallbackInfo<Value>& args);

  static void GetData(Local<String> property, const PropertyCallbackInfo<Value>& info);
  static void SetData(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

  static void GetControl(Local<String> property, const PropertyCallbackInfo<Value>& info);
  static void SetControl(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

  static void GetStatus(Local<String> property, const PropertyCallbackInfo<Value>& info);
  static void SetStatus(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

  static Persistent<Function> constructor;

  int handle_;
};

#endif
