#include <vector>
#include <map>
#include <core.h>
#include <iostream>

using namespace std;

#ifndef _stack

template <class T>
class stack{
	private:
		vector<T>* elems;
	public:
		stack();
		T pop();
		void push(T);
		T elem(int32_t);
		void print(map<uint32_t, object*>&);
		void clear();
		int len();
		T index(int i);
		void append(stack<T>*, uint32_t);
		vector<T>* getVector();
};



template <class T>
vector<T>* stack<T>::getVector(){
	return elems;
}

template <class T>
stack<T>::stack(){
	elems = new vector<T>;
}

template <class T>
int stack<T>::len(){
	return elems->size();
}

template <class T>
T stack<T>::index(int i){
	return elems->at(i);
}


template <class T>
void stack<T>::push(T e){
	elems->push_back(e);
}

template <class T>
T stack<T>::pop(){
	T r = elems->back();
	elems->pop_back();
	return r;
}

template <class T>
T stack<T>::elem(int32_t id){
	return elems->at(elems->size() - 1 + id);
}


template <class T>
void stack<T>::print(map<uint32_t, object*>& memory){
	cout << "[ ";
	unsigned int i;
	for(i = 0; i < elems->size(); ++i){
		cout << "(val:" << elems->at(i) << " ";
		if(memory.find(elems->at(i)) != memory.end()){
			object* o = memory[elems->at(i)];
			cout << "*type:" << o->type << " ";
			if (o->type == INT)
				cout << "*int:" << *(uint32_t*)o->pt << " ";
		}
		cout << ") ";
		}
	cout << "]" << endl;
	
}

template <class T>
void stack<T>::clear(){
	elems = new vector<T>;
	//elems.clear();
}

template <class T>
void stack<T>::append(stack<T>* s, uint32_t n){
	for(int i = s->len()-n;i < s->len(); i++)
		push(s->index(i));
}



#define _stack
#endif
