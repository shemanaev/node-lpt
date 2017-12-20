#include <nan.h>

#include <sys/ioctl.h>
#include <linux/parport.h>
#include <linux/ppdev.h>

#include "port-control.h"

Nan::Persistent<v8::Function> PortControl::constructor;

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
    Nan::ThrowError("Can't read control register");
  }
  invert(&value);

  return value;
}

void PortControl::SetControl(int value) {
  invert(&value);
  if (ioctl(handle_, PPWCONTROL, &value) != 0) {
    Nan::ThrowError("Can't write control register");
  }
}

NAN_MODULE_INIT(PortControl::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  auto tplInst = tpl->InstanceTemplate();
  tpl->SetClassName(Nan::New("PortControl").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetAccessor(tplInst, Nan::New("init").ToLocalChecked(), GetInit, SetInit);
  Nan::SetAccessor(tplInst, Nan::New("select").ToLocalChecked(), GetSelect, SetSelect);
  Nan::SetAccessor(tplInst, Nan::New("autofd").ToLocalChecked(), GetAutofd, SetAutofd);
  Nan::SetAccessor(tplInst, Nan::New("strobe").ToLocalChecked(), GetStrobe, SetStrobe);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("PortControl").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(PortControl::New) {
  if (!info.IsConstructCall()) {
    Nan::ThrowError("Use the new operator to create instances of this object.");
    return;
  }

  int num = info[0]->IsUndefined() ? 0 : info[0]->IntegerValue();
  PortControl* obj = new PortControl(num);
  obj->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

void PortControl::NewInstance(const Nan::PropertyCallbackInfo<v8::Value>& args, int handle) {
  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = { Nan::New(handle) };
  v8::Local<v8::Function> cons = Nan::New(constructor);
  //v8:: Local<v8::Object> instance = cons->NewInstance(argc, argv);

  Nan::MaybeLocal<v8::Object> maybeInstance = Nan::NewInstance(cons, argc, argv);
  v8::Local<v8::Object> instance;

  if (maybeInstance.IsEmpty()) {
    Nan::ThrowError("Could not create new PortControl instance");
  } else {
    instance = maybeInstance.ToLocalChecked();
  }

  args.GetReturnValue().Set(instance);
}

NAN_GETTER(PortControl::GetInit) {
  PortControl* obj = Nan::ObjectWrap::Unwrap<PortControl>(info.Holder());
  info.GetReturnValue().Set((obj->GetControl() & PARPORT_CONTROL_INIT) == PARPORT_CONTROL_INIT);
}

NAN_GETTER(PortControl::GetSelect) {
  PortControl* obj = Nan::ObjectWrap::Unwrap<PortControl>(info.Holder());
  info.GetReturnValue().Set((obj->GetControl() & PARPORT_CONTROL_SELECT) == PARPORT_CONTROL_SELECT);
}

NAN_GETTER(PortControl::GetAutofd) {
  PortControl* obj = Nan::ObjectWrap::Unwrap<PortControl>(info.Holder());
  info.GetReturnValue().Set((obj->GetControl() & PARPORT_CONTROL_AUTOFD) == PARPORT_CONTROL_AUTOFD);
}

NAN_GETTER(PortControl::GetStrobe) {
  PortControl* obj = Nan::ObjectWrap::Unwrap<PortControl>(info.Holder());
  info.GetReturnValue().Set((obj->GetControl() & PARPORT_CONTROL_STROBE) == PARPORT_CONTROL_STROBE);
}

NAN_SETTER(PortControl::SetInit) {
  int val = value->BooleanValue();
  PortControl* obj = Nan::ObjectWrap::Unwrap<PortControl>(info.Holder());
  int current = obj->GetControl();

  current &= !PARPORT_CONTROL_INIT;
  current |= val * PARPORT_CONTROL_INIT;
  obj->SetControl(current);
}

NAN_SETTER(PortControl::SetSelect) {
  int val = value->BooleanValue();
  PortControl* obj = Nan::ObjectWrap::Unwrap<PortControl>(info.Holder());
  int current = obj->GetControl();

  current &= !PARPORT_CONTROL_SELECT;
  current |= val * PARPORT_CONTROL_SELECT;
  obj->SetControl(current);
}

NAN_SETTER(PortControl::SetAutofd) {
  int val = value->BooleanValue();
  PortControl* obj = Nan::ObjectWrap::Unwrap<PortControl>(info.Holder());
  int current = obj->GetControl();

  current &= !PARPORT_CONTROL_AUTOFD;
  current |= val * PARPORT_CONTROL_AUTOFD;
  obj->SetControl(current);
}

NAN_SETTER(PortControl::SetStrobe) {
  int val = value->BooleanValue();
  PortControl* obj = Nan::ObjectWrap::Unwrap<PortControl>(info.Holder());
  int current = obj->GetControl();

  current &= !PARPORT_CONTROL_STROBE;
  current |= val * PARPORT_CONTROL_STROBE;
  obj->SetControl(current);
}
