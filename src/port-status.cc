#include <node.h>
#include <sys/ioctl.h>
#include <linux/parport.h>
#include <linux/ppdev.h>

#include "port-status.h"
#include "node-helpers.h"

using namespace v8;

Persistent<Function> PortStatus::constructor;

PortStatus::PortStatus(int handle) : handle_(handle) {}
PortStatus::~PortStatus() {}

int PortStatus::GetStatus() {
  int val;

  ioctl(handle_, PPRSTATUS, &val);
  val ^= PARPORT_STATUS_BUSY; /* /BUSY needs to get inverted */

  return val;
}

void PortStatus::Init(Handle<Object> exports) {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("PortStatus"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_ACCESSOR(tpl, "ack", GetAck, SetStatus);
  NODE_SET_ACCESSOR(tpl, "busy", GetBusy, SetStatus);
  NODE_SET_ACCESSOR(tpl, "error", GetError, SetStatus);
  NODE_SET_ACCESSOR(tpl, "select", GetSelect, SetStatus);
  NODE_SET_ACCESSOR(tpl, "paperOut", GetPaperOut, SetStatus);

  constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("PortStatus"), constructor);
}

Handle<Value> PortStatus::New(const Arguments& args) {
  HandleScope scope;

  if (!args.IsConstructCall()) {
    return THROW_EXCEPTION("Use the new operator to create instances of this object.");
  }

  int num = args[0]->IsUndefined() ? 0 : args[0]->IntegerValue();
  PortStatus* obj = new PortStatus(num);
  obj->Wrap(args.This());
  return args.This();
}

Handle<Value> PortStatus::NewInstance(int handle) {
  HandleScope scope;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = { Integer::New(handle) };
  Local<Object> instance = constructor->NewInstance(argc, argv);

  return scope.Close(instance);
}

Handle<Value> PortStatus::GetAck(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  PortStatus* obj = ObjectWrap::Unwrap<PortStatus>(info.Holder());
  return scope.Close(Boolean::New((obj->GetStatus() & PARPORT_STATUS_ACK) == PARPORT_STATUS_ACK));
}

Handle<Value> PortStatus::GetBusy(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  PortStatus* obj = ObjectWrap::Unwrap<PortStatus>(info.Holder());
  return scope.Close(Boolean::New((obj->GetStatus() & PARPORT_STATUS_BUSY) == PARPORT_STATUS_BUSY));
}

Handle<Value> PortStatus::GetError(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  PortStatus* obj = ObjectWrap::Unwrap<PortStatus>(info.Holder());
  return scope.Close(Boolean::New((obj->GetStatus() & PARPORT_STATUS_ERROR) == PARPORT_STATUS_ERROR));
}

Handle<Value> PortStatus::GetSelect(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  PortStatus* obj = ObjectWrap::Unwrap<PortStatus>(info.Holder());
  return scope.Close(Boolean::New((obj->GetStatus() & PARPORT_STATUS_SELECT) == PARPORT_STATUS_SELECT));
}

Handle<Value> PortStatus::GetPaperOut(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  PortStatus* obj = ObjectWrap::Unwrap<PortStatus>(info.Holder());
  return scope.Close(Boolean::New((obj->GetStatus() & PARPORT_STATUS_PAPEROUT) == PARPORT_STATUS_PAPEROUT));
}

void PortStatus::SetStatus(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  THROW_EXCEPTION("You can't modify status fields");
}
