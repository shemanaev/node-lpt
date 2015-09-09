#include <node.h>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/parport.h>
#include <linux/ppdev.h>

#include "port.h"
#include "port-status.h"
#include "port-control.h"
#include "node-helpers.h"

using v8::Exception;
using v8::FunctionTemplate;
using v8::Integer;

int GetModeByName(std::string mode) {
  if (mode == "nibble") {
    return IEEE1284_MODE_NIBBLE;
  } else if (mode == "ecp") {
    return IEEE1284_MODE_ECP;
  } else if (mode == "epp") {
    return IEEE1284_MODE_EPP;
  } else if (mode == "spp") {
    return IEEE1284_MODE_COMPAT;
  }
  return IEEE1284_MODE_BYTE;
}

Persistent<Function> Port::constructor;

Port::Port(Isolate* isolate, int num, int mode, bool negotiate) {
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
  if (ioctl(handle_, negotiate ? PPNEGOT : PPSETMODE, &mode) != 0) {
    close(handle_);
    THROW_EXCEPTION("Can't negotiate required mode");
  }
}

Port::~Port() {
  ioctl(handle_, PPRELEASE);
  close(handle_);
}

void Port::Init(Local<Object> exports) {
  Isolate* isolate = Isolate::GetCurrent();

  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "Port"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_ACCESSOR(tpl, "data", GetData, SetData);
  NODE_SET_ACCESSOR(tpl, "control", GetControl, SetControl);
  NODE_SET_ACCESSOR(tpl, "status", GetStatus, SetStatus);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "Port"), tpl->GetFunction());
}

void Port::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (!args.IsConstructCall()) {
    THROW_EXCEPTION("Use the new operator to create instances of this object.");
    return;
  }

  int num = args[0]->IsUndefined() ? 0 : args[0]->IntegerValue();
  std::string mode = args[1]->IsUndefined() ? "byte" : *String::Utf8Value(args[1]);
  bool negotiate = args[2]->IsUndefined() ? false : args[2]->BooleanValue();
  Port* obj = new Port(isolate, num, GetModeByName(mode), negotiate);
  obj->Wrap(args.This());
  args.GetReturnValue().Set(args.This());
}

void Port::GetData(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();

  unsigned char data;
  int dir;
  Port* obj = ObjectWrap::Unwrap<Port>(info.Holder());

  dir = 1;
  ioctl(obj->handle_, PPDATADIR, &dir);

  if (ioctl(obj->handle_, PPRDATA, &data) != 0) {
    THROW_EXCEPTION("Can't read data register");
    return;
  }

  info.GetReturnValue().Set(Integer::New(isolate, data));
}

void Port::SetData(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
  Isolate* isolate = info.GetIsolate();

  int val = value->IntegerValue();
  int dir;
  Port* obj = ObjectWrap::Unwrap<Port>(info.Holder());

  dir = 0;
  ioctl(obj->handle_, PPDATADIR, &dir);

  if (ioctl(obj->handle_, PPWDATA, &val) != 0) {
    THROW_EXCEPTION("Can't write data register");
  }
}

void Port::GetControl(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  // Isolate* isolate = info.GetIsolate();

  Port* obj = ObjectWrap::Unwrap<Port>(info.Holder());
  PortControl::NewInstance(info, obj->handle_);
}

void Port::SetControl(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
  Isolate* isolate = info.GetIsolate();

  THROW_EXCEPTION("You can't modify control itself");
}

void Port::GetStatus(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  // Isolate* isolate = info.GetIsolate();

  Port* obj = ObjectWrap::Unwrap<Port>(info.Holder());
  PortStatus::NewInstance(info, obj->handle_);
}

void Port::SetStatus(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
  Isolate* isolate = info.GetIsolate();
  
  THROW_EXCEPTION("You can't modify status");
}
