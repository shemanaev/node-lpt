#ifndef PORT_CONTROL_H
#define PORT_CONTROL_H

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

class PortControl : public node::ObjectWrap {
 public:
  static void Init(Local<Object> exports);
  static void NewInstance(const PropertyCallbackInfo<Value>& args, int handle);

 private:
  explicit PortControl(int handle);
  ~PortControl();

  int GetControl();
  void SetControl(int value);

  static void New(const FunctionCallbackInfo<Value>& args);

  static void GetInit(Local<String> property, const PropertyCallbackInfo<Value>& info);
  static void SetInit(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);
  
  static void GetSelect(Local<String> property, const PropertyCallbackInfo<Value>& info);
  static void SetSelect(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);
  
  static void GetAutofd(Local<String> property, const PropertyCallbackInfo<Value>& info);
  static void SetAutofd(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);
  
  static void GetStrobe(Local<String> property, const PropertyCallbackInfo<Value>& info);
  static void SetStrobe(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info);

  static Persistent<Function> constructor;

  int handle_;
};

#endif
