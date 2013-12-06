#include <node.h>

#include "port.h"
#include "port-status.h"
#include "port-control.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  Port::Init(exports);
  PortStatus::Init(exports);
  PortControl::Init(exports);
}

NODE_MODULE(lpt, InitAll)
