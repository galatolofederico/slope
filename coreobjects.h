#include <string>
#include <virtualobject.h>
#include <core.h>
#include <string>

#ifndef core_objects

using namespace std;

class sNone: public VirtualObject {
	public:	
		sNone();
		
		VirtualObject& operator +(VirtualObject& op);
		VirtualObject& operator -(VirtualObject& op);
		VirtualObject& operator /(VirtualObject& op);
		VirtualObject& operator *(VirtualObject& op);
		VirtualObject& operator <(VirtualObject& op);
		VirtualObject& operator >(VirtualObject& op);
		VirtualObject& operator %(VirtualObject& op);
		VirtualObject& operator ==(VirtualObject& op);
		
		string output();
		void input(string);
		
		VirtualObject& operation(uint32_t ord);
		object* getObject();
		vm_type getType();
		void finalize();
		~sNone();
};

class sBool : public VirtualObject {
	private:
		uint32_t val;
		object* o;
	public:
		sBool();
		sBool(object* o);
		void setVal(uint32_t val);
		uint32_t getVal();
		
		VirtualObject& operator +(VirtualObject& op);
		VirtualObject& operator -(VirtualObject& op);
		VirtualObject& operator /(VirtualObject& op);
		VirtualObject& operator *(VirtualObject& op);
		VirtualObject& operator <(VirtualObject& op);
		VirtualObject& operator >(VirtualObject& op);
		VirtualObject& operator %(VirtualObject& op);
		VirtualObject& operator ==(VirtualObject& op);
		
		string output();
		void input(string);
		
		VirtualObject& operation(uint32_t ord);
		object* getObject();
		vm_type getType();
		void finalize();
		~sBool();
};

class sInt : public VirtualObject {
	private:
		int32_t val;
		object* o;
	public:
		sInt();
		sInt(int32_t);
		sInt(object* o);
		void setVal(int32_t val);
		int32_t getVal();
		
		VirtualObject& operator +(VirtualObject& op);
		VirtualObject& operator -(VirtualObject& op);
		VirtualObject& operator /(VirtualObject& op);
		VirtualObject& operator *(VirtualObject& op);
		VirtualObject& operator <(VirtualObject& op);
		VirtualObject& operator >(VirtualObject& op);
		VirtualObject& operator %(VirtualObject& op);
		VirtualObject& operator ==(VirtualObject& op);

		string output();
		void input(string);
		
		VirtualObject& operation(uint32_t ord);
		object* getObject();
		vm_type getType();
		void finalize();
		~sInt();
};


class sString : public VirtualObject {
	private:
		string* str;
		object* o;
	public:
		sString();
		sString(string* s);
		sString(object* o);
		sString(string s);
		
		VirtualObject& operator +(VirtualObject& op);
		VirtualObject& operator -(VirtualObject& op);
		VirtualObject& operator /(VirtualObject& op);
		VirtualObject& operator *(VirtualObject& op);
		VirtualObject& operator <(VirtualObject& op);
		VirtualObject& operator >(VirtualObject& op);
		VirtualObject& operator %(VirtualObject& op);
		VirtualObject& operator ==(VirtualObject& op);
		
		string output();
		void input(string);
		
		VirtualObject& operation(uint32_t ord);
		string* getStr();
		object* getObject();
		vm_type getType();
		void finalize();
		~sString();
};

#define core_objects
#endif
