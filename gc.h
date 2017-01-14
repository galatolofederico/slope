#include <core.h>
#include <stdint.h>

void gc();
void gc_init();
void gc_flag(uint32_t);
uint32_t next_id();
uint32_t last_id();
