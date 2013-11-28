#include <node.h>
#include <sstream>
#include <sys/ioctl.h>
#include <linux/parport.h>
#include <linux/ppdev.h>

#include "port.h"
#include "port-status.h"
#include "node-helpers.h"

using namespace v8;

Persistent<Function> Port::constructor;

Port::Port(int num) {
  std::stringstream device;
  device << "/dev/parport" << num;

  handle_ = open(device.str().c_str(), O_RDWR);
  if (handle_ < 0) {
    THROW_EXCEPTION("Can't open device");
  }
  if (ioctl(handle_, PPCLAIM) != 0) {
    close(handle_);
    THROW_EXCEPTION("Can't claim device");
  }
  int mode = IEEE1284_MODE_COMPAT;
  if (ioctl(handle_, PPNEGOT, &mode) != 0) {
    close(handle_);
    THROW_EXCEPTION("Can't negotiate required mode");
  }
}

Port::~Port() {
  ioctl(handle_, PPRELEASE);
  close(handle_);
}

void Port::Init(Handle<Object> exports) {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Port"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_ACCESSOR(tpl, "data", GetData, SetData);
  NODE_SET_ACCESSOR(tpl, "control", GetControl, SetControl);
  NODE_SET_ACCESSOR(tpl, "status", GetStatus, SetStatus);

  constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("Port"), constructor);
}

Handle<Value> Port::New(const Arguments& args) {
  HandleScope scope;

  if (!args.IsConstructCall()) {
    return THROW_EXCEPTION("Use the new operator to create instances of this object.");
  }

  int num = args[0]->IsUndefined() ? 0 : args[0]->IntegerValue();
  Port* obj = new Port(num);
  obj->Wrap(args.This());
  return args.This();
}

Handle<Value> Port::GetData(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  unsigned char data;
  Port* obj = ObjectWrap::Unwrap<Port>(info.Holder());

  if (ioctl(obj->handle_, PPRDATA, &data) != 0) {
    return THROW_EXCEPTION("Can't read data register");
  }

  return scope.Close(Integer::New(data));
}

void Port::SetData(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  int val = value->IntegerValue();
  Port* obj = ObjectWrap::Unwrap<Port>(info.Holder());

  if (ioctl(obj->handle_, PPWDATA, &val) != 0) {
    THROW_EXCEPTION("Can't write data register");
  }
}

Handle<Value> Port::GetControl(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  Port* obj = ObjectWrap::Unwrap<Port>(info.Holder());

  unsigned char ret;
  if (ioctl(obj->handle_, PPRCONTROL, &ret) != 0) {
    return THROW_EXCEPTION("Can't read control register");
  }

  return scope.Close(Integer::New(ret));
}

void Port::SetControl(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  int val = value->IntegerValue();
  Port* obj = ObjectWrap::Unwrap<Port>(info.Holder());

  if (ioctl(obj->handle_, PPWCONTROL, &val) != 0) {
    THROW_EXCEPTION("Can't write control register");
  }
}

Handle<Value> Port::GetStatus(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  Port* obj = ObjectWrap::Unwrap<Port>(info.Holder());
  return scope.Close(PortStatus::NewInstance(obj->handle_));
}

void Port::SetStatus(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  THROW_EXCEPTION("You can't modify status");
}
