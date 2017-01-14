#include <map>
#include <iostream>
#include <core.h>
#include <virtualobject.h>
#include <coreobjects.h>
#include <stack.h>
#include <slist.h>

#include <util.h>

extern stack<uint32_t> arg_stack;
extern stack<uint32_t> res_stack;

extern uint32_t next_id;
extern map<uint32_t, object*> memory;

object* build_type(uint32_t id){
	object* ret = new object;
	ret->type = CUSTOM;
	ret->id = id;
	ret->pt = 0;
	return ret;
}


VirtualObject* getArgument(){
	return &getObjectFromVM(memory[arg_stack.pop()]);
}

VirtualObject& getObjectFromVM(object* o){
	if(o == 0) return *(new sNone());
	if(o->type == INT){
		return *(new sInt(o));
	}
	if(o->type == BOOL){
		return *(new sBool(o));
	}
	if(o->type == STRING){
		return *(new sString(o));
	}
	if(o->type == CUSTOM){
		if(o->id == 256){
			return *(new sList(o));
		}
	}
	cout << "Errore nella traduzione da tipo vm a tipo c++" << endl;
	return *(new sNone());
}


const char *stropcode[] = {
	"LOAD_STACK",
	"STORE_CONST",
	"CALL",
	"STORE_RES",
	"STORE_STACK",
	"OUT",
	"SUM",
	"COPY",
	"RET",
	"CLEAN_STACK",
	"DISCARD_RES",
	"COPY_OPERANDS",
	"STORE_OP",
	"LOWER",
	"JMP_TRUE",
	"JMP",
	"MIN",
	"OP",
	"BUILD_TYPE",
	"MUL",
	"DIV",
	"MOD",
	"HIGHER",
	"EQUALS",
	"IN"
};
