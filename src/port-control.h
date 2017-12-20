#ifndef PORT_CONTROL_H
#define PORT_CONTROL_H

#include <nan.h>

class PortControl : public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);
    static void NewInstance(const Nan::PropertyCallbackInfo<v8::Value>& args, int handle);

  private:
    explicit PortControl(int handle);
    ~PortControl();

    int GetControl();
    void SetControl(int value);

    static NAN_METHOD(New);

    static NAN_GETTER(GetInit);
    static NAN_SETTER(SetInit);

    static NAN_GETTER(GetSelect);
    static NAN_SETTER(SetSelect);

    static NAN_GETTER(GetAutofd);
    static NAN_SETTER(SetAutofd);

    static NAN_GETTER(GetStrobe);
    static NAN_SETTER(SetStrobe);

    static Nan::Persistent<v8::Function> constructor;

    int handle_;
};

#endif
