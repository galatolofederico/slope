#include <core.h>
#include <map>
#include <stack.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <virtualobject.h>
#include <util.h>

using namespace std;

uint32_t mem_size = 50;
uint32_t notify_size = 20;
uint32_t last_used = 0;
uint32_t count = 0;

bool* free_ids;
bool run_gc = false;

vector<uint32_t> static_ids;

extern map<uint32_t, object*> memory;
extern stack<uint32_t> res_stack;
extern bool debug;

void gc(){
	uint32_t tmp = count;
	if(debug) cout << "-------" << endl;
	if (debug) cout << "Running the garbage collector" << endl;
	count = 0;
	uint32_t i = 0;
	for(i = 0;i < mem_size;i++) free_ids[i] = true;
	for(i = 0;i < static_ids.size(); i++){
		free_ids[static_ids[i]] = false;
		count++;
	}
	VirtualObject* obj;
	map<uint32_t, object*>::iterator it;
	for(i = 0;i < res_stack.len();i++){
		free_ids[res_stack.index(i)] = false;
		count++;
	}
	last_used = i;
	
	for(it = memory.begin(); it != memory.end(); ++it){
		if(free_ids[it->first]){
			if(debug) cout << "Deleting object with id: " << it->first << "(" << it->second << ")" << endl;
			obj = &getObjectFromVM(it->second);
			obj->finalize();
			memory.erase(it);
			delete obj;
		}
	}
	if (debug) cout << "Clean "<< tmp-count << " objects"<< endl;
	if(debug) cout << "-------" << endl;
	run_gc = false;
}

void gc_init(){
	free_ids = new bool[mem_size];
	uint32_t i = 0;
	for(i = 0;i < mem_size;i++) free_ids[i] = true;
}

uint32_t next_id(){
	uint32_t i, j;
	bool found = false;
	for(i = 0;i < mem_size;i++){
		j = (last_used + i)%mem_size;
		if(free_ids[j]){
			found = true;
			free_ids[j] = false;
			last_used = j;
			break;
		}
	}
	if(count > notify_size) run_gc = true;
	if(found){
		count++;
		return j;
	}
	else{
		cout << "Out Of Memory" << endl;
		exit(0);
	}
}

uint32_t last_id(){
	return last_used;
}

void gc_flag(uint32_t id){
	count++;
	free_ids[id] = false;
	static_ids.push_back(id);
}

