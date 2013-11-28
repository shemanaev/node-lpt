#ifndef PORT_STATUS_H
#define PORT_STATUS_H

#include <node.h>

class PortStatus : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Handle<v8::Value> NewInstance(int handle);

 private:
  explicit PortStatus(int handle);
  ~PortStatus();

  int GetStatus();

  static v8::Handle<v8::Value> New(const v8::Arguments& args);

  static v8::Handle<v8::Value> GetAck(v8::Local<v8::String> property, const v8::AccessorInfo& info);
  static v8::Handle<v8::Value> GetBusy(v8::Local<v8::String> property, const v8::AccessorInfo& info);
  static v8::Handle<v8::Value> GetError(v8::Local<v8::String> property, const v8::AccessorInfo& info);
  static v8::Handle<v8::Value> GetSelect(v8::Local<v8::String> property, const v8::AccessorInfo& info);
  static v8::Handle<v8::Value> GetPaperOut(v8::Local<v8::String> property, const v8::AccessorInfo& info);
  static void SetStatus(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

  static v8::Persistent<v8::Function> constructor;

  int handle_;
};

#endif
