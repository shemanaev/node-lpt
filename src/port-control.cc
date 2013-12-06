#include <node.h>
#include <sys/ioctl.h>
#include <linux/parport.h>
#include <linux/ppdev.h>

#include "port-control.h"
#include "node-helpers.h"

using namespace v8;

Persistent<Function> PortControl::constructor;

PortControl::PortControl(int handle) : handle_(handle) {}
PortControl::~PortControl() {}

void invert(int *value) {
  *value ^= PARPORT_CONTROL_SELECT;
  *value ^= PARPORT_CONTROL_AUTOFD;
  *value ^= PARPORT_CONTROL_STROBE;
}

int PortControl::GetControl() {
  int value;

  if (ioctl(handle_, PPRCONTROL, &value) != 0) {
    THROW_EXCEPTION("Can't read control register");
  }
  invert(&value);

  return value;
}

void PortControl::SetControl(int value) {
  invert(&value);
  if (ioctl(handle_, PPWCONTROL, &value) != 0) {
    THROW_EXCEPTION("Can't write control register");
  }
}

void PortControl::Init(Handle<Object> exports) {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("PortControl"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_ACCESSOR(tpl, "init", GetInit, SetInit);
  NODE_SET_ACCESSOR(tpl, "select", GetSelect, SetSelect);
  NODE_SET_ACCESSOR(tpl, "autofd", GetAutofd, SetAutofd);
  NODE_SET_ACCESSOR(tpl, "strobe", GetStrobe, SetStrobe);

  constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("PortControl"), constructor);
}

Handle<Value> PortControl::New(const Arguments& args) {
  HandleScope scope;

  if (!args.IsConstructCall()) {
    return THROW_EXCEPTION("Use the new operator to create instances of this object.");
  }

  int num = args[0]->IsUndefined() ? 0 : args[0]->IntegerValue();
  PortControl* obj = new PortControl(num);
  obj->Wrap(args.This());
  return args.This();
}

Handle<Value> PortControl::NewInstance(int handle) {
  HandleScope scope;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = { Integer::New(handle) };
  Local<Object> instance = constructor->NewInstance(argc, argv);

  return scope.Close(instance);
}

Handle<Value> PortControl::GetInit(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  PortControl* obj = ObjectWrap::Unwrap<PortControl>(info.Holder());
  return scope.Close(Boolean::New((obj->GetControl() & PARPORT_CONTROL_INIT) == PARPORT_CONTROL_INIT));
}

Handle<Value> PortControl::GetSelect(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  PortControl* obj = ObjectWrap::Unwrap<PortControl>(info.Holder());
  return scope.Close(Boolean::New((obj->GetControl() & PARPORT_CONTROL_SELECT) == PARPORT_CONTROL_SELECT));
}

Handle<Value> PortControl::GetAutofd(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  PortControl* obj = ObjectWrap::Unwrap<PortControl>(info.Holder());
  return scope.Close(Boolean::New((obj->GetControl() & PARPORT_CONTROL_AUTOFD) == PARPORT_CONTROL_AUTOFD));
}

Handle<Value> PortControl::GetStrobe(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  PortControl* obj = ObjectWrap::Unwrap<PortControl>(info.Holder());
  return scope.Close(Boolean::New((obj->GetControl() & PARPORT_CONTROL_STROBE) == PARPORT_CONTROL_STROBE));
}

void PortControl::SetInit(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  int val = value->BooleanValue();
  PortControl* obj = ObjectWrap::Unwrap<PortControl>(info.Holder());
  int current = obj->GetControl();

  current &= !PARPORT_CONTROL_INIT;
  current |= val * PARPORT_CONTROL_INIT;
  obj->SetControl(current);
}

void PortControl::SetSelect(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  int val = value->BooleanValue();
  PortControl* obj = ObjectWrap::Unwrap<PortControl>(info.Holder());
  int current = obj->GetControl();

  current &= !PARPORT_CONTROL_SELECT;
  current |= val * PARPORT_CONTROL_SELECT;
  obj->SetControl(current);
}

void PortControl::SetAutofd(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  int val = value->BooleanValue();
  PortControl* obj = ObjectWrap::Unwrap<PortControl>(info.Holder());
  int current = obj->GetControl();

  current &= !PARPORT_CONTROL_AUTOFD;
  current |= val * PARPORT_CONTROL_AUTOFD;
  obj->SetControl(current);
}

void PortControl::SetStrobe(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  int val = value->BooleanValue();
  PortControl* obj = ObjectWrap::Unwrap<PortControl>(info.Holder());
  int current = obj->GetControl();

  current &= !PARPORT_CONTROL_STROBE;
  current |= val * PARPORT_CONTROL_STROBE;
  obj->SetControl(current);
}
