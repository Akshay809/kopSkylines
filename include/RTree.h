#ifndef RTREE_H
#define RTREE_H

#include <objects.h>
using namespace std;

class RTree {
public:
	RTree(instanceSet instances);
	void search(DataInstance& From, DataInstance& To, vector<int>& ans);
	~RTree();
};

#endif