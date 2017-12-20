#include <nan.h>

#include <sys/ioctl.h>
#include <linux/parport.h>
#include <linux/ppdev.h>

#include "port-status.h"

Nan::Persistent<v8::Function> PortStatus::constructor;

PortStatus::PortStatus(int handle) : handle_(handle) {}
PortStatus::~PortStatus() {}

int PortStatus::GetStatus() {
  int val;

  ioctl(handle_, PPRSTATUS, &val);
  val ^= PARPORT_STATUS_BUSY; /* /BUSY needs to get inverted */

  return val;
}

NAN_MODULE_INIT(PortStatus::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  auto tplInst = tpl->InstanceTemplate();
  tpl->SetClassName(Nan::New("PortStatus").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetAccessor(tplInst, Nan::New("ack").ToLocalChecked(), GetAck, SetStatus);
  Nan::SetAccessor(tplInst, Nan::New("busy").ToLocalChecked(), GetBusy, SetStatus);
  Nan::SetAccessor(tplInst, Nan::New("error").ToLocalChecked(), GetError, SetStatus);
  Nan::SetAccessor(tplInst, Nan::New("select").ToLocalChecked(), GetSelect, SetStatus);
  Nan::SetAccessor(tplInst, Nan::New("paperOut").ToLocalChecked(), GetPaperOut, SetStatus);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("PortStatus").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(PortStatus::New) {
  if (!info.IsConstructCall()) {
    Nan::ThrowError("Use the new operator to create instances of this object.");
    return;
  }

  int num = info[0]->IsUndefined() ? 0 : info[0]->IntegerValue();
  PortStatus* obj = new PortStatus(num);
  obj->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

void PortStatus::NewInstance(const Nan::PropertyCallbackInfo<v8::Value>& args, int handle) {
  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = { Nan::New(handle) };
  v8::Local<v8::Function> cons = Nan::New(constructor);
  //v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);

  Nan::MaybeLocal<v8::Object> maybeInstance = Nan::NewInstance(cons, argc, argv);
  v8::Local<v8::Object> instance;

  if (maybeInstance.IsEmpty()) {
    Nan::ThrowError("Could not create new PortStatus instance");
  } else {
    instance = maybeInstance.ToLocalChecked();
  }

  args.GetReturnValue().Set(instance);
}

NAN_GETTER(PortStatus::GetAck) {
  PortStatus* obj = ObjectWrap::Unwrap<PortStatus>(info.Holder());
  info.GetReturnValue().Set((obj->GetStatus() & PARPORT_STATUS_ACK) == PARPORT_STATUS_ACK);
}

NAN_GETTER(PortStatus::GetBusy) {
  PortStatus* obj = ObjectWrap::Unwrap<PortStatus>(info.Holder());
  info.GetReturnValue().Set((obj->GetStatus() & PARPORT_STATUS_BUSY) == PARPORT_STATUS_BUSY);
}

NAN_GETTER(PortStatus::GetError) {
  PortStatus* obj = ObjectWrap::Unwrap<PortStatus>(info.Holder());
  info.GetReturnValue().Set((obj->GetStatus() & PARPORT_STATUS_ERROR) == PARPORT_STATUS_ERROR);
}

NAN_GETTER(PortStatus::GetSelect) {
  PortStatus* obj = ObjectWrap::Unwrap<PortStatus>(info.Holder());
  info.GetReturnValue().Set((obj->GetStatus() & PARPORT_STATUS_SELECT) == PARPORT_STATUS_SELECT);
}

NAN_GETTER(PortStatus::GetPaperOut) {
  PortStatus* obj = ObjectWrap::Unwrap<PortStatus>(info.Holder());
  info.GetReturnValue().Set((obj->GetStatus() & PARPORT_STATUS_PAPEROUT) == PARPORT_STATUS_PAPEROUT);
}

NAN_SETTER(PortStatus::SetStatus) {
  Nan::ThrowError("You can't modify status fields");
}
