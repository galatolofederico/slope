#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <core.h>
#include <stack.h>
#include <util.h>
#include <operations.h>
#include <gc.h>
#include <virtualobject.h>

using namespace std;

extern const char *stropcode[];
extern bool run_gc;

bool debug;


map<uint32_t, object*> memory;


stack<uint32_t> arg_stack;
stack<uint32_t> res_stack;


vm_type get_type(uint32_t type){
	switch(type){
		case(uint32_t('c')):
			return CODE;
		case(uint32_t('i')):
			return INT;
		case(uint32_t('d')):
			return IODESC;
		case(uint32_t('s')):
			return STRING;
	}
	return NONE;
}


void static_loader(uint8_t* code, uint32_t type, uint32_t id, uint32_t len){
	object* o = new object;
	uint32_t i = 0;
	vector<bytecode>* codes;
	string* s;
	uint32_t* desc;
	switch(get_type(type)){
	case(CODE):
		o->type = CODE;
		codes = new vector<bytecode>();
		o->pt = (void*)codes;
		while(i < len){
			bytecode& op = *(new bytecode);
			op.arg = 0;
			switch(code[i]){
				case(uint8_t(1)):
					op.op = LOAD_STACK;
					op.arg = 0;
					if(debug) cout << "LOAD_STACK " << endl;
					break;
				case(uint8_t(2)):
					op.op = STORE_CONST;
					op.arg = *((int32_t*)(code+i+1));
					i = i + 4;
					if(debug) cout << "STORE_CONST " << (int32_t)op.arg << endl;
					break;
				case(uint8_t(3)):
					op.op = CALL;
					op.arg = *((uint32_t*)(code+i+1));
					i = i + 4;
					if(debug) cout << "CALL " << op.arg << endl;
					break;
				case(uint8_t(4)):
					op.op = STORE_RES;
					if(debug) cout << "STORE_RES " << endl;
					break;
				case(uint8_t(5)):
					op.op = STORE_STACK;
					op.arg = *((int32_t*)(code+i+1));
					i = i + 4;
					if(debug) cout << "STORE_STACK " << (int32_t)op.arg << endl;
					break;
				case(uint8_t(6)):
					op.op = OUT;
					if(debug) cout << "OUT " << op.arg << endl;
					break;
				case(uint8_t(7)):
					op.op = SUM;
					if(debug) cout << "SUM " << endl;
					break;
				case(uint8_t(8)):
					op.op = COPY;
					if(debug) cout << "COPY " << endl;
					break;
				case(uint8_t(9)):
					op.op = RET;
					if(debug) cout << "RET " << endl;
					break;
				case(uint8_t(10)):
					op.op = CLEAN_STACK;
					if(debug) cout << "CLEAN_STACK " << endl;
					break;
				case(uint8_t(11)):
					op.op = DISCARD_RES;
					if(debug) cout << "DISCARD_RES " << endl;
					break;
				case(uint8_t(12)):
					op.op = COPY_OPERANDS;
					op.arg = *((uint32_t*)(code+i+1));
					i = i + 4;
					if(debug) cout << "COPY_OPERANDS "  << (uint32_t)op.arg << endl;
					break;
				case(uint8_t(13)):
					op.op = STORE_OP;
					if(debug) cout << "STORE_OP " << endl;
					break;
				case(uint8_t(14)):
					op.op = LOWER;
					if(debug) cout << "LOWER " << endl;
					break;
				case(uint8_t(15)):
					op.op = JMP_TRUE;
					op.arg = *((int32_t*)(code+i+1));
					i = i + 4;
					if(debug) cout << "JMP_TRUE " << (int32_t)op.arg << endl;
					break;
				case(uint8_t(16)):
					op.op = JMP;
					op.arg = *((int32_t*)(code+i+1));
					i = i + 4;
					if(debug) cout << "JMP " << (int32_t)op.arg << endl;
					break;
				case(uint8_t(17)):
					op.op = MIN;
					if(debug) cout << "MIN " << endl;
					break;
				case(uint8_t(18)):
					op.op = OP;
					op.arg = *((uint32_t*)(code+i+1));
					i = i + 4;
					if(debug) cout << "OP " << op.arg << endl;
					break;
				case(uint8_t(19)):
					op.op = BUILD_TYPE;
					op.arg = *((uint32_t*)(code+i+1));
					i = i + 4;
					if(debug) cout << "BUILD_TYPE " << op.arg << endl;
					break;
				case(uint8_t(20)):
					op.op = MUL;
					if(debug) cout << "MUL " << endl;
					break;
				case(uint8_t(21)):
					op.op = DIV;
					if(debug) cout << "DIV " << endl;
					break;
				case(uint8_t(22)):
					op.op = MOD;
					if(debug) cout << "MOD " << endl;
					break;
				case(uint8_t(23)):
					op.op = HIGHER;
					if(debug) cout << "HIGHER " << endl;
					break;
				case(uint8_t(24)):
					op.op = EQUALS;
					if(debug) cout << "EQUALS " << endl;
					break;
				case(uint8_t(25)):
					op.op = IN;
					if(debug) cout << "IN " << endl;
					break;
				
			}
			codes->push_back(op);
			i++;
		}
		break;
	case(IODESC):
		o->type = IODESC;
		desc = new uint32_t;
		memcpy(desc, code, sizeof(uint32_t));
		o->pt = desc;
		if(debug) cout << "I/O DESCRIPTOR id: " << id << " val: " << *desc << endl;
		break;
	case(STRING):
		o->type = STRING;
		s = new string((const char*)code, 0, len);
		o->pt = (void*)s;
		if(debug) cout << "STRING id: " << id << " val: " << *s << endl;
		break;
	}
	memory[id] = o;
	gc_flag(id);
}



void exec(uint32_t id){
	vector<bytecode>* code = (vector<bytecode>*)memory[id]->pt;
	vector<bytecode>::iterator instr = code->begin();
	VirtualObject* app;
	while(instr != code->end()){
		//char a; cin >> a;
		if(debug) cout << "instr: " << stropcode[instr->op] << " arg: " << instr->arg << endl;
		uint32_t len, i, *data;
		object* o;
		uint32_t id1, id2;
		switch(instr->op){
			case(LOAD_STACK):
				o = new object;
				o->type = get_type(arg_stack.pop());
				len = arg_stack.pop();
				data = new uint32_t[len];
				i = 0;
				while(i < len)
					data[i++] = arg_stack.pop();
				o->pt = data;
				memory[next_id()] = o;
				res_stack.push(last_id());
				break;
			case(STORE_CONST):
				arg_stack.push(instr->arg);
				break;
			case(CALL):
				exec(instr->arg);
				break;
			case(STORE_RES):
				if(res_stack.len() > 0)
					arg_stack.push(res_stack.pop());
				break;
			case(STORE_STACK):
				if(-(int32_t)instr->arg < res_stack.len())
					arg_stack.push(res_stack.elem((int32_t)instr->arg));
				break;
			case(OUT):
				id1 = arg_stack.pop();
				id2 = arg_stack.pop();
				io_out(memory[id1], memory[id2]);
				break;
			case(SUM):
				id1 = arg_stack.pop();
				id2 = arg_stack.pop();			
				o = alu(SUM, memory[id2], memory[id1]);
				memory[next_id()] = o;
				res_stack.push(last_id());
				break;
			case(COPY):
				id1 = arg_stack.pop();
				id2 = arg_stack.pop();
				app = &getObjectFromVM(memory[id2]);
				memory[id1] = app->getObject();
				delete app;
				break;
			case(RET):
				return;
			case(CLEAN_STACK):
				if(arg_stack.len() > 0)
					arg_stack.clear();
				break;
			case(DISCARD_RES):
				res_stack.pop();
				break;
			case(COPY_OPERANDS):
				res_stack.append(&arg_stack, instr->arg);
				arg_stack.clear();
				break;
			case(STORE_OP):
				if(arg_stack.len() > 0)
					res_stack.push(arg_stack.pop());
				break;
			case(LOWER):
				id1 = arg_stack.pop();
				id2 = arg_stack.pop();			
				o = alu(LOWER, memory[id2], memory[id1]);
				memory[next_id()] = o;
				res_stack.push(last_id());
				break;
			case(JMP_TRUE):
				o = memory[arg_stack.pop()];
				if(o->type != BOOL)
					cout << "Qualcosa forse non va!" << endl;
				if(*(uint32_t*)(o->pt))
					instr += (int32_t)instr->arg;
				break;
			case(JMP):
				instr += (int32_t)instr->arg;
				break;
			case(MIN):
				id1 = arg_stack.pop();
				id2 = arg_stack.pop();			
				o = alu(MIN, memory[id2], memory[id1]);
				memory[next_id()] = o;
				res_stack.push(last_id());
				break;
			case(OP):
				id1 = arg_stack.pop();
				o = alu(OP, memory[id1], (object*)0, instr->arg);
				memory[next_id()] = o;
				// le funzioni ritornano nello stack degli argomenti
				arg_stack.push(last_id());
				break;
			case(BUILD_TYPE):
				o = build_type(instr->arg);
				memory[next_id()] = o;
				arg_stack.push(last_id());
				break;
			case(MUL):
				id1 = arg_stack.pop();
				id2 = arg_stack.pop();			
				o = alu(MUL, memory[id2], memory[id1]);
				memory[next_id()] = o;
				res_stack.push(last_id());
				break;
			case(MOD):
				id1 = arg_stack.pop();
				id2 = arg_stack.pop();			
				o = alu(MOD, memory[id2], memory[id1]);
				memory[next_id()] = o;
				res_stack.push(last_id());
				break;
			case(DIV):
				id1 = arg_stack.pop();
				id2 = arg_stack.pop();			
				o = alu(DIV, memory[id2], memory[id1]);
				memory[next_id()] = o;
				res_stack.push(last_id());
				break;
			case(HIGHER):
				id1 = arg_stack.pop();
				id2 = arg_stack.pop();			
				o = alu(HIGHER, memory[id2], memory[id1]);
				memory[next_id()] = o;
				res_stack.push(last_id());
				break;
			case(EQUALS):
				id1 = arg_stack.pop();
				id2 = arg_stack.pop();			
				o = alu(EQUALS, memory[id2], memory[id1]);
				memory[next_id()] = o;
				res_stack.push(last_id());
				break;
			case(IN):
				id1 = arg_stack.pop();
				id2 = arg_stack.pop();
				memory[id2] = io_in(memory[id1], memory[id2]);
				break;
		}
		if(debug) arg_stack.print(memory);
		if(debug) res_stack.print(memory);
		++instr;
		if(arg_stack.len() == 0 && run_gc) gc();
	}
	
	
}


int main(int argc, char **argv){
	debug = false;
	if(argc > 2)
		if(strcmp(argv[2], "-d") == 0)
			debug = true;
	if(argc < 2){
		cout << "Usage ./vm <bytecode> <flags>" << endl;
		return(0);
	}
	gc_init();
	FILE* f;
	f = fopen(argv[1], "rb");
	uint32_t type, id, len;
	while(fread(&type, 1, 4, f) > 0){
		fread(&id, 1, 4, f);
		fread(&len, 1, 4, f);
		if(debug) cout << "found type " << type << " id " << id  << " len " << len <<  endl;
		uint8_t* bytes = (uint8_t*)malloc(sizeof(uint8_t)*len);
		fread(bytes, 1, len, f);
		static_loader(bytes, type, id, len);
		free(bytes);
	}
	exec(0);
}
	
