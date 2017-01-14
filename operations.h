#include <core.h>

#ifndef operations

void io_out(object* , object* );
object* io_in(object*, object*);
object* alu(opcode , object* , object* , uint32_t id = 0 );

#define operations
#endif
