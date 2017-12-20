#include <nan.h>

#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/parport.h>
#include <linux/ppdev.h>

#include "port.h"
#include "port-status.h"
#include "port-control.h"

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

Nan::Persistent<v8::Function> Port::constructor;

Port::Port(int num, int mode, bool negotiate) {
  std::stringstream device;
  device << "/dev/parport" << num;

  handle_ = open(device.str().c_str(), O_RDWR);
  if (handle_ < 0) {
    Nan::ThrowError("Can't open device");
  }
  if (ioctl(handle_, PPCLAIM) != 0) {
    close(handle_);
    Nan::ThrowError("Can't claim device");
  }
  if (ioctl(handle_, negotiate ? PPNEGOT : PPSETMODE, &mode) != 0) {
    close(handle_);
    Nan::ThrowError("Can't negotiate required mode");
  }
}

Port::~Port() {
  ioctl(handle_, PPRELEASE);
  close(handle_);
}

NAN_MODULE_INIT(Port::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  auto tplInst = tpl->InstanceTemplate();
  tpl->SetClassName(Nan::New("Port").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetAccessor(tplInst, Nan::New("data").ToLocalChecked(), GetData, SetData);
  Nan::SetAccessor(tplInst, Nan::New("control").ToLocalChecked(), GetControl, SetControl);
  Nan::SetAccessor(tplInst, Nan::New("status").ToLocalChecked(), GetStatus, SetStatus);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("Port").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(Port::New) {
  if (!info.IsConstructCall()) {
    Nan::ThrowError("Use the new operator to create instances of this object.");
    return;
  }

  int num = info[0]->IsUndefined() ? 0 : info[0]->IntegerValue();
  std::string mode = info[1]->IsUndefined() ? "byte" : *v8::String::Utf8Value(info[1]);
  bool negotiate = info[2]->IsUndefined() ? false : info[2]->BooleanValue();
  Port* obj = new Port(num, GetModeByName(mode), negotiate);
  obj->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

NAN_GETTER(Port::GetData) {
  unsigned char data;
  int dir;
  Port* obj = ObjectWrap::Unwrap<Port>(info.Holder());

  dir = 1;
  ioctl(obj->handle_, PPDATADIR, &dir);

  if (ioctl(obj->handle_, PPRDATA, &data) != 0) {
    Nan::ThrowError("Can't read data register");
    return;
  }

  info.GetReturnValue().Set(data);
}

NAN_SETTER(Port::SetData) {
  int val = value->IntegerValue();
  int dir;
  Port* obj = Nan::ObjectWrap::Unwrap<Port>(info.Holder());

  dir = 0;
  ioctl(obj->handle_, PPDATADIR, &dir);

  if (ioctl(obj->handle_, PPWDATA, &val) != 0) {
    Nan::ThrowError("Can't write data register");
  }
}

NAN_GETTER(Port::GetControl) {
  Port* obj = ObjectWrap::Unwrap<Port>(info.Holder());
  PortControl::NewInstance(info, obj->handle_);
}

NAN_SETTER(Port::SetControl) {
  Nan::ThrowError("You can't modify control itself");
}

NAN_GETTER(Port::GetStatus) {
  Port* obj = ObjectWrap::Unwrap<Port>(info.Holder());
  PortStatus::NewInstance(info, obj->handle_);
}

NAN_SETTER(Port::SetStatus) {
  Nan::ThrowError("You can't modify status");
}
