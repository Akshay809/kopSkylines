#include <Heap.h>

Heap::Heap(vector<DataInstance>& instances): instances(instances) {
	indices.resize(instances.size());
	// cout << "Initializing the Heap ...." << endl;
	for(currentIndex = 0; currentIndex<instances.size(); currentIndex++) {
		indices[currentIndex] = currentIndex;
	}
	std::make_heap(indices.begin(), indices.end(), lower(instances));
	// cout << "make" << endl;
	// printHeap();
}

bool Heap::isEmpty() {
	return indices.size()==0;
}

DataInstance& Heap::top() {
	// cout << "top" << endl;
	// printHeap();
	return instances[indices.front()];
}

void Heap::pop() {
	std::pop_heap(indices.begin(), indices.end(), lower(instances));
	indices.pop_back();
	// cout << "pop" << endl;
	// printHeap();
}

void Heap::push(const DataInstance& newInstance) {
	// cout << "Pushed instance .. " << endl;
	// newInstance.printDataInstance();
	instances.push_back(newInstance);
	indices.push_back(currentIndex++);
	std::push_heap(indices.begin(), indices.end(), lower(instances));
	// cout << "push" << endl;
	// printHeap();
}

void Heap::printHeap() {
	// return;
	cout << "Current Heap state" << endl;
	for(int i=0;i<indices.size();i++)
		instances[indices[i]].printDataInstance();
}
