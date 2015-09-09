
#define THROW_EXCEPTION(msg) \
  isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, msg)))


#define NODE_SET_ACCESSOR(obj, name, get, set) \
  (obj)->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, name), get, set)
