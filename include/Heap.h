#ifndef HEAP_H
#define HEAP_H

#include <Data.h>
using namespace std;

class Heap {
public:
	Heap(vector<DataInstance> instances);
	bool isEmpty();
	bool isNotEmpty() {
		return !this->isEmpty();
	}
	DataInstance top();
	~Heap();
};

#endif