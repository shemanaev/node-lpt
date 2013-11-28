
#define THROW_EXCEPTION(msg) \
  ThrowException(Exception::TypeError(String::New(msg)))

#define NODE_SET_ACCESSOR(obj, name, get, set) \
  (obj)->InstanceTemplate()->SetAccessor(String::NewSymbol(name), get, set)
