#ifndef PORT_CONTROL_H
#define PORT_CONTROL_H

#include <node.h>

class PortControl : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Handle<v8::Value> NewInstance(int handle);

 private:
  explicit PortControl(int handle);
  ~PortControl();

  int GetControl();
  void SetControl(int value);

  static v8::Handle<v8::Value> New(const v8::Arguments& args);

  static v8::Handle<v8::Value> GetInit(v8::Local<v8::String> property, const v8::AccessorInfo& info);
  static v8::Handle<v8::Value> GetSelect(v8::Local<v8::String> property, const v8::AccessorInfo& info);
  static v8::Handle<v8::Value> GetAutofd(v8::Local<v8::String> property, const v8::AccessorInfo& info);
  static v8::Handle<v8::Value> GetStrobe(v8::Local<v8::String> property, const v8::AccessorInfo& info);
  static void SetInit(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
  static void SetSelect(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
  static void SetAutofd(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
  static void SetStrobe(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

  static v8::Persistent<v8::Function> constructor;

  int handle_;
};

#endif
