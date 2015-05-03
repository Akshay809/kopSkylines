#ifndef CUSTOM_HEAP_H
#define CUSTOM_HEAP_H

#include <Data.h>
#include <algorithm>

using namespace std;

class Heap {
private:
	vector<DataInstance>& instances;
public:
	struct lower;
	Heap(vector<DataInstance>&);
	bool isEmpty();
	bool isNotEmpty() {	return !isEmpty(); }
	DataInstance& top();
	void pop();
	void push(DataInstance&);
	~Heap() {}
};

struct Heap::lower {
	int x;
	lower(int x): x(x) {}
	bool operator()(const DataInstance& i1, const DataInstance& i2) {
		return (i1.getKey())>(i2.getKey());
	}
};

Heap::Heap(vector<DataInstance>& instances): instances(instances) {
	cout << "Initializing the Heap ...." << endl;
	std::make_heap(instances.begin(), instances.end(), lower(0));
}

bool Heap::isEmpty() {
	return instances.size()==0;
}

DataInstance& Heap::top() {
	instances.front();
}

void Heap::pop() {
	std::pop_heap(instances.begin(), instances.end(), lower(0));
	instances.pop_back();
}

void Heap::push(DataInstance& newInstance) {
	instances.push_back(newInstance);
	std::push_heap(instances.begin(), instances.end(), lower(0));
}

#endif
