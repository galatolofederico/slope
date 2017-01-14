#include <iostream>
#include <map>
#include <stdlib.h>
#include <typeinfo>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <util.h>
#include <virtualobject.h>
#include <stdio.h>
#include <operations.h>

using namespace std;



extern bool debug;
//bool debug = true;



void io_out(object* desc, object* o){
	//cout << "type " << desc->type << endl;
	if(desc->type != IODESC){
		cout << "Aborting I/O" << endl;
		return;
	}
	cout << flush;
	VirtualObject& obj = getObjectFromVM(o);
	string to_out = obj.output() + "\n";
	delete &obj;
	write(*(uint32_t*)(desc->pt), to_out.c_str(), to_out.length());
}

object* io_in(object* desc, object* o){
	VirtualObject& obj = getObjectFromVM(o);
	size_t n = 1024;
	char* buff = (char*)malloc(n);
	if(*(uint32_t*)(desc->pt) == 0)
		fgets(buff, n, stdin);
	else
		fgets(buff, n, (FILE*)(desc->pt));
	string s(buff, 0, n);
	if (!s.empty() && s[s.length()-1] == '\n') {
		s.erase(s.length()-1);
	}
	free(buff);
	obj.input(s);
	object* ret = obj.getObject();
	VirtualObject& old_o = getObjectFromVM(o);
	//old_o.finalize();
	delete &old_o;
	free(o);
	return ret;
}

object* alu(opcode op, object* op1, object* op2, uint32_t id){
	object* ret = (object*)malloc(sizeof(object));
	VirtualObject& obj1 = getObjectFromVM(op1);
	VirtualObject& obj2 = getObjectFromVM(op2);
	switch(op){
		case(SUM):
			ret = (obj1 + obj2).getObject();
			break;
		case(LOWER):
			ret = (obj1 < obj2).getObject();
			break;
		case(MIN):
			ret = (obj1 - obj2).getObject();
			break;
		case(MUL):
			ret = (obj1 * obj2).getObject();
			break;
		case(DIV):
			ret = (obj1 / obj2).getObject();
			break;
		case(MOD):
			ret = (obj1 % obj2).getObject();
			break;
		case(EQUALS):
			ret = (obj1 == obj2).getObject();
			break;
		case(HIGHER):
			ret = (obj1 > obj2).getObject();
			break;
		case(OP):
			ret = obj1.operation(id).getObject();
			break;
		default:
			cout << "Not Implemented" << endl;
			break;
	}
	delete &obj1;
	delete &obj2;
	return ret;
}


