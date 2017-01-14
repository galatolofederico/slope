#include "names.h"
#include <stdlib.h>
#include <typeinfo>
#include <iostream>
#include <vector>
#include <map>
#include "stack.cpp"

using namespace std;

extern stack<uint32_t> arg_stack;
extern stack<uint32_t> res_stack;

extern uint32_t next_id;
extern map<uint32_t, object*> memory;

class VirtualObject {
	public:
		virtual VirtualObject& operator +(VirtualObject&) = 0;
		virtual VirtualObject& operator -(VirtualObject&) = 0;
		virtual VirtualObject& operator <(VirtualObject&) = 0;
		virtual VirtualObject& operation(uint32_t) = 0;
		virtual object* getObject() = 0;
};

class sNone: public VirtualObject {
	public:
		sNone(){ };
		
		VirtualObject& operator +(VirtualObject& op){
			return *this;
		}

		VirtualObject& operator -(VirtualObject& op){
			return *this;
		}
		
		VirtualObject& operator <(VirtualObject& op){
			return *this;
		}
		
		VirtualObject& operation(uint32_t ord){
			return *this;
		}
		object* getObject(){
			object* ret = (object*)malloc(sizeof(object));
			ret->type = NONE;
			return ret;
		}
};


class sBool : public VirtualObject {
	private:
		uint32_t val;
	public:
		sBool(){ };
		sBool(object* o){
			this->val = *(uint32_t*)(o->pt);
		}
		void setVal(uint32_t val){ this->val = val; }
		uint32_t getVal(){ return this->val; }
		
		VirtualObject& operator +(VirtualObject& op){
			cout << "Unsupported type" << endl;
			return *(new sNone);
		}

		VirtualObject& operator -(VirtualObject& op){
			cout << "Unsupported type" << endl;
			return *(new sNone);
		}
		
		VirtualObject& operator <(VirtualObject& op){
			cout << "Unsupported type" << endl;
			return *(new sNone);
		}
		
		VirtualObject& operation(uint32_t ord){
			return *this;
		}
		
		object* getObject(){
			object* ret = (object*)malloc(sizeof(object));
			ret->type = BOOL;
			uint32_t* res = (uint32_t*)malloc(sizeof(uint32_t));
			*res = getVal();
			ret->pt = (void*)res;
			return ret;
		}
	
};

class sInt : public VirtualObject {
	private:
		uint32_t val;
	public:
		sInt(){ };
		sInt(object* o){
			this->val = *(uint32_t*)(o->pt);
		}
		void setVal(uint32_t val){ this->val = val; }
		uint32_t getVal(){ return this->val; }
		
		VirtualObject& operator +(VirtualObject& op){
			if(typeid(op) == typeid(sInt)){
				sInt* ret = new sInt;
				sInt* intop = (sInt*)&op;
				ret->setVal(this->val + intop->getVal());
				return *ret;
			}
			cout << "a Unsupported type" << endl;
			return *(new sNone);
		}

		VirtualObject& operator -(VirtualObject& op){
			if(typeid(op) == typeid(sInt)){
				sInt* ret = new sInt;
				sInt* intop = (sInt*)&op;
				ret->setVal(this->val - intop->getVal());
				return *ret;
			}
			cout << "Unsupported type" << endl;
			return *(new sNone);
		}
		
		VirtualObject& operator <(VirtualObject& op){
			if(typeid(op) == typeid(sInt)){
				sBool* ret = new sBool;
				sInt* intop = (sInt*)&op;
				ret->setVal(this->val < intop->getVal());
				return *ret;
			}
			cout << "Unsupported type" << endl;
			return *(new sNone);
		}

		VirtualObject& operation(uint32_t ord){
			return *this;
		}
		
		object* getObject(){
			object* ret = (object*)malloc(sizeof(object));
			ret->type = INT;
			uint32_t* res = (uint32_t*)malloc(sizeof(uint32_t));
			*res = getVal();
			ret->pt = (void*)res;
			return ret;
		}
};


class sList : public VirtualObject {
	private:
		vector<uint32_t>* elems;
		object* o;
	public:
		sList(object* o){
			this->o = o;
			if (o->pt == 0)
				o->pt = (void*) new vector<uint32_t>;
			elems = (vector<uint32_t>*) o->pt;
		};
		
		VirtualObject& operator +(VirtualObject& op){
			cout << "Not implemented" << endl;
			return *(new sNone);
		}

		VirtualObject& operator -(VirtualObject& op){
			cout << "Not implemented" << endl;
			return *(new sNone);
		}
		
		VirtualObject& operator <(VirtualObject& op){
			cout << "Not implemented" << endl;
			return *(new sNone);
		}

		VirtualObject& operation(uint32_t ord){
			object* n;
			switch(ord){
				case(0): //push(elem)
					elems->push_back(arg_stack.pop());
					break;
				case(1): //elem pop()
					o = memory[elems->back()];
					elems->pop_back();
					break;
				case(2): //elem index(elem)
					n = memory[arg_stack.pop()];
					if(n->type != INT){
						cout << "Index function accepts only INT as parameter" << endl;
						break;
					}
					o = memory[elems->at(*(uint32_t*)(n->pt))];
					break;
				case(3): //elem len()
					n = new object;
					n->type = INT;
					uint32_t* l = new uint32_t;
					*l = (uint32_t)elems->size();
					n->pt = l;
					memory[next_id] = n;
					o = n;
					next_id++;
					break;
			}
			return *this;
		}
		
		object* getObject(){
			return o;
		}
};


