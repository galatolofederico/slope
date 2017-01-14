#include <vector>
#include <core.h>
#include <map>
#include <stdint.h>
#include <iostream>
#include <coreobjects.h>
#include <stack.h>
#include <util.h>
#include <typeinfo>
#include <string>

#include <slist.h>

using namespace std;

extern map<uint32_t, object*> memory;

extern stack<uint32_t> arg_stack;
extern stack<uint32_t> res_stack;

extern uint32_t next_id;

sList::sList(object* o){
	this->o = o;
	if (o->pt == 0){
		vector_pointer* e = new vector_pointer;
		e->p = new vector<VirtualObject*>;
		e->counter = 0;
		o->pt = (void*)e;
	}
	l = (vector_pointer*)o->pt;
	l->counter++;
};

VirtualObject& sList::operator +(VirtualObject& op){
	cout << "Not implemented" << endl;
	return *(new sNone);
}

VirtualObject& sList::operator -(VirtualObject& op){
	cout << "Not implemented" << endl;
	return *(new sNone);
}

VirtualObject& sList::operator /(VirtualObject& op){
	cout << "Not implemented" << endl;
	return *(new sNone);
}

VirtualObject& sList::operator *(VirtualObject& op){
	cout << "Not implemented" << endl;
	return *(new sNone);
}


VirtualObject& sList::operator %(VirtualObject& op){
	cout << "Not implemented" << endl;
	return *(new sNone);
}

VirtualObject& sList::operator >(VirtualObject& op){
	cout << "Not implemented" << endl;
	return *(new sNone);
}


VirtualObject& sList::operator <(VirtualObject& op){
	cout << "Not implemented" << endl;
	return *(new sNone);
}

VirtualObject& sList::operator ==(VirtualObject& op){
	cout << "Not implemented" << endl;
	return *(new sNone);
}


string sList::output(){
	vector<VirtualObject*>* elems = l->p;
	string ret("[");
	vector<VirtualObject*>::iterator it;
	for(it = elems->begin(); it != elems->end(); ++it){
		ret += (*it)->output();
		if(it != elems->end()-1)
			ret += ", ";
	}
	ret += "]";
	return ret;
}

void sList::input(string str){
	
}


VirtualObject& sList::operation(uint32_t ord){
	vector<VirtualObject*>* elems = l->p;
	object* n;
	VirtualObject* arg = 0;
	sInt* intarg;
	switch(ord){
		case(0): //push(elem)
			arg = getArgument();
			elems->push_back(arg);
			break;
		case(1): //elem pop()
			o = elems->back()->getObject();
			elems->pop_back();
			break;
		case(2): //elem index(elem)
			arg = getArgument();
			if(arg->getType() != INT){
				cout << "Index function accepts only INT as parameter" << endl;
				break;
			}
			intarg = (sInt*)arg;
			o = elems->at(intarg->getVal())->getObject();
			break;
		case(3): //elem len()
			arg = new sInt(elems->size());
			o = arg->getObject();
			break;
	}
	if(arg != 0 && ord != 0){
		delete arg;
	}
	return *this;
}

object* sList::getObject(){
	object* oo =new object;
	oo->type = o->type;
	oo->id = o->id;
	oo->pt = o->pt;
	return oo;
}


vm_type sList::getType(){
	return CUSTOM;
}

void sList::finalize(){
	l->counter--;
	if(l->counter == 0){
		delete (vector<VirtualObject*>*)o->pt;
		delete l;
	}
}

sList::~sList(){
	
}
