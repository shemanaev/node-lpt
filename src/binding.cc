#include <nan.h>

#include "port.h"
#include "port-status.h"
#include "port-control.h"

NAN_MODULE_INIT(InitAll) {
  Port::Init(target);
  PortStatus::Init(target);
  PortControl::Init(target);
}

NODE_MODULE(lpt, InitAll)
