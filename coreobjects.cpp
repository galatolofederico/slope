#include <virtualobject.h>
#include <core.h>
#include <iostream>
#include <stdlib.h>
#include <typeinfo>
#include <string>

#include <coreobjects.h>

using namespace std;


sNone::sNone(){}

VirtualObject& sNone::operator +(VirtualObject& op){
	return *this;
}

VirtualObject& sNone::operator -(VirtualObject& op){
	return *this;
}

VirtualObject& sNone::operator /(VirtualObject& op){
	return *this;
}

VirtualObject& sNone::operator *(VirtualObject& op){
	return *this;
}
		
VirtualObject& sNone::operator <(VirtualObject& op){
	return *this;
}

VirtualObject& sNone::operator >(VirtualObject& op){
	return *this;
}

VirtualObject& sNone::operator %(VirtualObject& op){
	return *this;
}

VirtualObject& sNone::operator ==(VirtualObject& op){
	return *this;
}


string sNone::output(){
	
}

void sNone::input(string str){
	
}


VirtualObject& sNone::operation(uint32_t ord){
	return *this;
}
object* sNone::getObject(){
	object* ret = (object*)malloc(sizeof(object));
	ret->type = NONE;
	return ret;
}

vm_type sNone::getType(){
	return NONE;
}

void sNone::finalize(){

}





sBool::sBool(){
	this->val = 0;
}

sBool::sBool(object* o){
	this->o = o;
	this->val = *(uint32_t*)(o->pt);
}

void sBool::setVal(uint32_t val){
	this->val = val;
}

uint32_t sBool::getVal(){
	return this->val;
}

VirtualObject& sBool::operator +(VirtualObject& op){
	if(typeid(op) == typeid(sBool)){
		sBool* ret = new sBool;
		sBool* intop = (sBool*)&op;
		ret->setVal(this->val || intop->getVal());
		return *ret;
	}
	cout << "Unsupported type: bool + " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sBool::operator -(VirtualObject& op){
	cout << "Unsupported operation: bool - " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sBool::operator /(VirtualObject& op){
	cout << "Unsupported operation: bool / " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sBool::operator *(VirtualObject& op){
	if(typeid(op) == typeid(sBool)){
		sBool* ret = new sBool;
		sBool* intop = (sBool*)&op;
		ret->setVal(this->val && intop->getVal());
		return *ret;
	}
	cout << "Unsupported type: bool + " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sBool::operator <(VirtualObject& op){
	cout << "Unsupported operation: bool < " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sBool::operator >(VirtualObject& op){
	cout << "Unsupported operation: bool - " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sBool::operator %(VirtualObject& op){
	cout << "Unsupported operation: bool - " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sBool::operator ==(VirtualObject& op){
	cout << "Unsupported operation: bool - " << typeid(op).name() << endl;
	return *(new sNone);
}


string sBool::output(){
	if(this->val > 0)
		return string("true");
	else
		return string("false");
}

void sBool::input(string str){
	
}


VirtualObject& sBool::operation(uint32_t ord){
	cout << "Unsupported operation: bool op ord " << ord << endl;
	return *this;
}

object* sBool::getObject(){
	object* ret = (object*)malloc(sizeof(object));
	ret->type = BOOL;
	uint32_t* res = (uint32_t*)malloc(sizeof(uint32_t));
	*res = getVal();
	ret->pt = (void*)res;
	return ret;
}

vm_type sBool::getType(){
	return BOOL;
}

void sBool::finalize(){
	free((uint32_t*)o->pt);
}



sInt::sInt(){
	this->val = 0;
}

sInt::sInt(int32_t val){
	this->val = val;
}

sInt::sInt(object* o){
	this->o = o;
	this->val = *(int32_t*)(o->pt);
}

void sInt::setVal(int32_t val){
	this->val = val;
}

int32_t sInt::getVal(){
	return this->val;
}

VirtualObject& sInt::operator +(VirtualObject& op){
	if(typeid(op) == typeid(sInt)){
		sInt* ret = new sInt;
		sInt* intop = (sInt*)&op;
		ret->setVal(this->val + intop->getVal());
		return *ret;
	}
	cout << "Unsupported type: int + " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sInt::operator -(VirtualObject& op){
	if(typeid(op) == typeid(sInt)){
		sInt* ret = new sInt;
		sInt* intop = (sInt*)&op;
		ret->setVal(this->val - intop->getVal());
		return *ret;
	}
	cout << "Unsupported type: int - " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sInt::operator /(VirtualObject& op){
	if(typeid(op) == typeid(sInt)){
		sInt* ret = new sInt;
		sInt* intop = (sInt*)&op;
		ret->setVal(this->val / intop->getVal());
		return *ret;
	}
	cout << "Unsupported type: int + " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sInt::operator *(VirtualObject& op){
	if(typeid(op) == typeid(sInt)){
		sInt* ret = new sInt;
		sInt* intop = (sInt*)&op;
		ret->setVal(this->val * intop->getVal());
		return *ret;
	}
	cout << "Unsupported type: int + " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sInt::operator %(VirtualObject& op){
	if(typeid(op) == typeid(sInt)){
		sInt* ret = new sInt;
		sInt* intop = (sInt*)&op;
		ret->setVal(this->val % intop->getVal());
		return *ret;
	}
	cout << "Unsupported type: int + " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sInt::operator <(VirtualObject& op){
	if(typeid(op) == typeid(sInt)){
		sBool* ret = new sBool;
		sInt* intop = (sInt*)&op;
		ret->setVal(this->val < intop->getVal());
		return *ret;
	}
	cout << "Unsupported type: int < " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sInt::operator >(VirtualObject& op){
	if(typeid(op) == typeid(sInt)){
		sBool* ret = new sBool;
		sInt* intop = (sInt*)&op;
		ret->setVal(this->val > intop->getVal());
		return *ret;
	}
	cout << "Unsupported type: int < " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sInt::operator ==(VirtualObject& op){
	if(typeid(op) == typeid(sInt)){
		sBool* ret = new sBool;
		sInt* intop = (sInt*)&op;
		ret->setVal(this->val == intop->getVal());
		return *ret;
	}
	cout << "Unsupported type: int < " << typeid(op).name() << endl;
	return *(new sNone);
}


string sInt::output(){
	return to_string(val);
}

void sInt::input(string str){
	val = stoi(str);
}


VirtualObject& sInt::operation(uint32_t ord){
	cout << "Unsupported operation: int op ord " << ord << endl;
	return *this;
}


object* sInt::getObject(){
	object* ret = (object*)malloc(sizeof(object));
	ret->type = INT;
	uint32_t* res = (uint32_t*)malloc(sizeof(uint32_t));
	*res = getVal();
	ret->pt = (void*)res;
	return ret;
}

vm_type sInt::getType(){
	return INT;
}

void sInt::finalize(){
	free((uint32_t*)o->pt);
}



sString::sString(){
	str = new string();
}

sString::sString(object* o){
	this->o = o;
	str = (string*)o->pt;
}

sString::sString(string* s){
	str = new string(*s);
}

sString::sString(string s){
	str = new string(s);
}

string* sString::getStr(){
	return str;
}

VirtualObject& sString::operator +(VirtualObject& op){
	return *(new sString(*(str) + op.output()));
}


VirtualObject& sString::operator -(VirtualObject& op){
	cout << "Unsupported operation: String - " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sString::operator *(VirtualObject& op){
	string ret("");
	if(typeid(op) == typeid(sInt)){
		sInt* intop = (sInt*)&op;
		int max = (intop->getVal() > 0) ? intop->getVal() : -intop->getVal();
		for(int i = 0; i < max;i++)
			ret += *this->str;
		return *(new sString(ret));
	} else {
		cout << "Unsupported operations: String * " << typeid(op).name() << endl;
	}
	return *(new sNone);
}

VirtualObject& sString::operator /(VirtualObject& op){
	cout << "Unsupported operation: String / " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sString::operator %(VirtualObject& op){
	cout << "Unsupported operation: String % " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sString::operator <(VirtualObject& op){
	cout << "Unsupported operation: String < " << typeid(op).name() << endl;
	return *(new sNone);
}


VirtualObject& sString::operator >(VirtualObject& op){
	cout << "Unsupported operation: String > " << typeid(op).name() << endl;
	return *(new sNone);
}

VirtualObject& sString::operator ==(VirtualObject& op){
	cout << "Unsupported operation: String == " << typeid(op).name() << endl;
	return *(new sNone);
}


string sString::output(){
	return *str;
}

void sString::input(string str){
	delete this->str;
	this->str = new string(str);
}

VirtualObject& sString::operation(uint32_t id){
	cout << "Not yet implemented!" << endl;
	return *(new sNone);
}

	
vm_type sString::getType(){
	return STRING;
}


object* sString::getObject(){
	object* ret = new object;
	ret->type = STRING;
	ret->pt = (void*)str;
	return ret;
}

void sString::finalize(){
	delete (string*)o->pt;
}


sBool::~sBool()
{

}
sInt::~sInt()
{

}
sNone::~sNone()
{

}
sString::~sString()
{

}

