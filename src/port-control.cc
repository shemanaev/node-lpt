#include <node.h>
#include <sys/ioctl.h>
#include <linux/parport.h>
#include <linux/ppdev.h>

#include "port-control.h"
#include "node-helpers.h"

using v8::Boolean;
using v8::Exception;
using v8::FunctionTemplate;
using v8::Integer;

Persistent<Function> PortControl::constructor;

PortControl::PortControl(int handle) : handle_(handle) {}
PortControl::~PortControl() {}

void invert(int *value) {
  *value ^= PARPORT_CONTROL_SELECT;
  *value ^= PARPORT_CONTROL_AUTOFD;
  *value ^= PARPORT_CONTROL_STROBE;
}

int PortControl::GetControl() {
  Isolate* isolate = Isolate::GetCurrent();
  int value;

  if (ioctl(handle_, PPRCONTROL, &value) != 0) {
    THROW_EXCEPTION("Can't read control register");
  }
  invert(&value);

  return value;
}

void PortControl::SetControl(int value) {
  Isolate* isolate = Isolate::GetCurrent();

  invert(&value);
  if (ioctl(handle_, PPWCONTROL, &value) != 0) {
    THROW_EXCEPTION("Can't write control register");
  }
}

void PortControl::Init(Local<Object> exports) {
  Isolate* isolate = Isolate::GetCurrent();

  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "PortControl"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_ACCESSOR(tpl, "init", GetInit, SetInit);
  NODE_SET_ACCESSOR(tpl, "select", GetSelect, SetSelect);
  NODE_SET_ACCESSOR(tpl, "autofd", GetAutofd, SetAutofd);
  NODE_SET_ACCESSOR(tpl, "strobe", GetStrobe, SetStrobe);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "PortControl"), tpl->GetFunction());
}

void PortControl::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (!args.IsConstructCall()) {
    THROW_EXCEPTION("Use the new operator to create instances of this object.");
    return;
  }

  int num = args[0]->IsUndefined() ? 0 : args[0]->IntegerValue();
  PortControl* obj = new PortControl(num);
  obj->Wrap(args.This());
  args.GetReturnValue().Set(args.This());
}

void PortControl::NewInstance(const PropertyCallbackInfo<Value>& args, int handle) {
  Isolate* isolate = args.GetIsolate();

  const unsigned argc = 1;
  Local<Value> argv[argc] = { Integer::New(isolate, handle) };
  Local<Function> cons = Local<Function>::New(isolate, constructor);
  Local<Object> instance = cons->NewInstance(argc, argv);

  args.GetReturnValue().Set(instance);
}

void PortControl::GetInit(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();

  PortControl* obj = ObjectWrap::Unwrap<PortControl>(info.Holder());
  info.GetReturnValue().Set(Boolean::New(isolate, (obj->GetControl() & PARPORT_CONTROL_INIT) == PARPORT_CONTROL_INIT));
}

void PortControl::GetSelect(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();

  PortControl* obj = ObjectWrap::Unwrap<PortControl>(info.Holder());
  info.GetReturnValue().Set(Boolean::New(isolate, (obj->GetControl() & PARPORT_CONTROL_SELECT) == PARPORT_CONTROL_SELECT));
}

void PortControl::GetAutofd(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();

  PortControl* obj = ObjectWrap::Unwrap<PortControl>(info.Holder());
  info.GetReturnValue().Set(Boolean::New(isolate, (obj->GetControl() & PARPORT_CONTROL_AUTOFD) == PARPORT_CONTROL_AUTOFD));
}

void PortControl::GetStrobe(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();

  PortControl* obj = ObjectWrap::Unwrap<PortControl>(info.Holder());
  info.GetReturnValue().Set(Boolean::New(isolate, (obj->GetControl() & PARPORT_CONTROL_STROBE) == PARPORT_CONTROL_STROBE));
}

void PortControl::SetInit(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
  int val = value->BooleanValue();
  PortControl* obj = ObjectWrap::Unwrap<PortControl>(info.Holder());
  int current = obj->GetControl();

  current &= !PARPORT_CONTROL_INIT;
  current |= val * PARPORT_CONTROL_INIT;
  obj->SetControl(current);
}

void PortControl::SetSelect(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
  int val = value->BooleanValue();
  PortControl* obj = ObjectWrap::Unwrap<PortControl>(info.Holder());
  int current = obj->GetControl();

  current &= !PARPORT_CONTROL_SELECT;
  current |= val * PARPORT_CONTROL_SELECT;
  obj->SetControl(current);
}

void PortControl::SetAutofd(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
  int val = value->BooleanValue();
  PortControl* obj = ObjectWrap::Unwrap<PortControl>(info.Holder());
  int current = obj->GetControl();

  current &= !PARPORT_CONTROL_AUTOFD;
  current |= val * PARPORT_CONTROL_AUTOFD;
  obj->SetControl(current);
}

void PortControl::SetStrobe(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
  int val = value->BooleanValue();
  PortControl* obj = ObjectWrap::Unwrap<PortControl>(info.Holder());
  int current = obj->GetControl();

  current &= !PARPORT_CONTROL_STROBE;
  current |= val * PARPORT_CONTROL_STROBE;
  obj->SetControl(current);
}
