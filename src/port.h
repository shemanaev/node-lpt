#ifndef PORT_H
#define PORT_H

#include <nan.h>

class Port : public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);

  private:
    explicit Port(int num, int mode, bool set);
    ~Port();

    static NAN_METHOD(New);

    static NAN_GETTER(GetData);
    static NAN_SETTER(SetData);

    static NAN_GETTER(GetControl);
    static NAN_SETTER(SetControl);

    static NAN_GETTER(GetStatus);
    static NAN_SETTER(SetStatus);

    static Nan::Persistent<v8::Function> constructor;

    int handle_;
};

#endif
