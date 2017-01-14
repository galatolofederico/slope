#include <core.h>
#include <iostream>
#include <string>

#ifndef vo
#define vo

class VirtualObject {
	public:
		virtual VirtualObject& operator +(VirtualObject&) = 0;
		virtual VirtualObject& operator -(VirtualObject&) = 0;
		virtual VirtualObject& operator /(VirtualObject&) = 0;
		virtual VirtualObject& operator *(VirtualObject&) = 0;
		virtual VirtualObject& operator %(VirtualObject&) = 0;
		virtual VirtualObject& operator <(VirtualObject&) = 0;
		virtual VirtualObject& operator >(VirtualObject&) = 0;
		virtual VirtualObject& operator ==(VirtualObject&) = 0;
		
		virtual std::string output() = 0;
		virtual void input(std::string) = 0;
		
		
		virtual VirtualObject& operation(uint32_t) = 0;
		virtual object* getObject() = 0;
		virtual vm_type getType() = 0;
		virtual void finalize() = 0;
		virtual ~VirtualObject() { }
};

#endif
