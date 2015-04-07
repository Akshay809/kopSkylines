#ifndef HEAP_H
#define HEAP_H

#include <Data.h>
using namespace std;

class Heap {
public:
	Heap(instanceSet instances);
	bool isEmpty();
	bool isNotEmpty() {	return !isEmpty(); }
	DataInstance& top();
	void pop();
	~Heap();
};

#endif
