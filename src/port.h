#ifndef PORT_H
#define PORT_H

#include <node.h>

class Port : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  explicit Port(int num, int mode);
  ~Port();

  static v8::Handle<v8::Value> New(const v8::Arguments& args);

  static v8::Handle<v8::Value> GetData(v8::Local<v8::String> property, const v8::AccessorInfo& info);
  static void SetData(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

  static v8::Handle<v8::Value> GetControl(v8::Local<v8::String> property, const v8::AccessorInfo& info);
  static void SetControl(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

  static v8::Handle<v8::Value> GetStatus(v8::Local<v8::String> property, const v8::AccessorInfo& info);
  static void SetStatus(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

  static v8::Persistent<v8::Function> constructor;

  int handle_;
  int dir_;
};

#endif
