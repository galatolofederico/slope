#include <stdint.h>
#ifndef types

typedef enum{LOAD_STACK, STORE_CONST, CALL, STORE_RES, STORE_STACK, OUT, SUM, COPY, RET,
	CLEAN_STACK, DISCARD_RES, COPY_OPERANDS, STORE_OP, LOWER, JMP_TRUE, JMP, MIN} opcode;

struct bytecode {opcode op; uint32_t arg;};
typedef enum{CODE, INT, STRING, NONE, IODESC, BOOL} vm_type;
struct object{vm_type type; void* pt;};

#define types
#endif
