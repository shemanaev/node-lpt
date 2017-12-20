#ifndef PORT_STATUS_H
#define PORT_STATUS_H

#include <nan.h>

class PortStatus : public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);
    static void NewInstance(const Nan::PropertyCallbackInfo<v8::Value>& args, int handle);

  private:
    explicit PortStatus(int handle);
    ~PortStatus();

    int GetStatus();

    static NAN_METHOD(New);

    static NAN_GETTER(GetAck);
    static NAN_GETTER(GetBusy);
    static NAN_GETTER(GetError);
    static NAN_GETTER(GetSelect);
    static NAN_GETTER(GetPaperOut);

    static NAN_SETTER(SetStatus);

    static Nan::Persistent<v8::Function> constructor;

    int handle_;
};

#endif
