#include <vector>
#include <core.h>
#include <map>
#include <string>

#include <virtualobject.h>
#include <core.h>

using namespace std;

#ifndef slist

struct vector_pointer {vector<VirtualObject*>* p; int counter;};

class sList : public VirtualObject {
	private:
		//vector<VirtualObject*>* elems;
		vector_pointer* l;
		object* o;
	public:
		sList(object* o);
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
		~sList();
};

#define slist
#endif
