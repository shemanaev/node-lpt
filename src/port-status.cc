#include <node.h>
#include <sys/ioctl.h>
#include <linux/parport.h>
#include <linux/ppdev.h>

#include "port-status.h"
#include "node-helpers.h"

using v8::Boolean;
using v8::Exception;
using v8::FunctionTemplate;
using v8::Integer;

Persistent<Function> PortStatus::constructor;

PortStatus::PortStatus(int handle) : handle_(handle) {}
PortStatus::~PortStatus() {}

int PortStatus::GetStatus() {
  int val;

  ioctl(handle_, PPRSTATUS, &val);
  val ^= PARPORT_STATUS_BUSY; /* /BUSY needs to get inverted */

  return val;
}

void PortStatus::Init(Local<Object> exports) {
  Isolate* isolate = Isolate::GetCurrent();

  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "PortStatus"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_ACCESSOR(tpl, "ack", GetAck, SetStatus);
  NODE_SET_ACCESSOR(tpl, "busy", GetBusy, SetStatus);
  NODE_SET_ACCESSOR(tpl, "error", GetError, SetStatus);
  NODE_SET_ACCESSOR(tpl, "select", GetSelect, SetStatus);
  NODE_SET_ACCESSOR(tpl, "paperOut", GetPaperOut, SetStatus);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "PortStatus"), tpl->GetFunction());
}

void PortStatus::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (!args.IsConstructCall()) {
    THROW_EXCEPTION("Use the new operator to create instances of this object.");
    return;
  }

  int num = args[0]->IsUndefined() ? 0 : args[0]->IntegerValue();
  PortStatus* obj = new PortStatus(num);
  obj->Wrap(args.This());
  args.GetReturnValue().Set(args.This());
}

void PortStatus::NewInstance(const PropertyCallbackInfo<Value>& args, int handle) {
  Isolate* isolate = args.GetIsolate();

  const unsigned argc = 1;
  Local<Value> argv[argc] = { Integer::New(isolate, handle) };
  Local<Function> cons = Local<Function>::New(isolate, constructor);
  Local<Object> instance = cons->NewInstance(argc, argv);

  args.GetReturnValue().Set(instance);
}

void PortStatus::GetAck(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();

  PortStatus* obj = ObjectWrap::Unwrap<PortStatus>(info.Holder());
  info.GetReturnValue().Set(Boolean::New(isolate, (obj->GetStatus() & PARPORT_STATUS_ACK) == PARPORT_STATUS_ACK));
}

void PortStatus::GetBusy(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();

  PortStatus* obj = ObjectWrap::Unwrap<PortStatus>(info.Holder());
  info.GetReturnValue().Set(Boolean::New(isolate, (obj->GetStatus() & PARPORT_STATUS_BUSY) == PARPORT_STATUS_BUSY));
}

void PortStatus::GetError(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();

  PortStatus* obj = ObjectWrap::Unwrap<PortStatus>(info.Holder());
  info.GetReturnValue().Set(Boolean::New(isolate, (obj->GetStatus() & PARPORT_STATUS_ERROR) == PARPORT_STATUS_ERROR));
}

void PortStatus::GetSelect(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();

  PortStatus* obj = ObjectWrap::Unwrap<PortStatus>(info.Holder());
  info.GetReturnValue().Set(Boolean::New(isolate, (obj->GetStatus() & PARPORT_STATUS_SELECT) == PARPORT_STATUS_SELECT));
}

void PortStatus::GetPaperOut(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();

  PortStatus* obj = ObjectWrap::Unwrap<PortStatus>(info.Holder());
  info.GetReturnValue().Set(Boolean::New(isolate, (obj->GetStatus() & PARPORT_STATUS_PAPEROUT) == PARPORT_STATUS_PAPEROUT));
}

void PortStatus::SetStatus(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
  Isolate* isolate = info.GetIsolate();

  THROW_EXCEPTION("You can't modify status fields");
}
