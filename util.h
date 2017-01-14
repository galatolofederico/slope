#include <core.h>
#include <virtualobject.h>

#ifndef util

object* build_type(uint32_t id);
VirtualObject& getObjectFromVM(object* o);
VirtualObject* getArgument();

#define util
#endif
