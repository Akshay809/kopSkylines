#ifndef CUSTOM_HEAP_H
#define CUSTOM_HEAP_H

#include <iostream>
#include <algorithm>

#include <Data.h>

using namespace std;

/*Imp Side effect: Heap operation changes the original array passed as reference, therefore pass a copy if the original is needed elsewhere */

class Heap {
private:
	vector<DataInstance>& instances;
	vector<int> indices;
	int currentIndex;
public:
	struct lower;
	Heap(vector<DataInstance>&);
	bool isEmpty();
	bool isNotEmpty() {	return !isEmpty(); }
	DataInstance& top();
	void pop();
	void push(const DataInstance&);
	void printHeap();
	~Heap() {}
};

struct Heap::lower {
	vector<DataInstance>& instances;
	lower(vector<DataInstance>& instances): instances(instances) {}

	bool operator()(const int& i1, const int& i2) {
		return instances[i1].getKey() > instances[i2].getKey();
	}
};

#endif
