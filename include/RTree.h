#ifndef RTREE_H
#define RTREE_H

#include <objects.h>
using namespace std;

class RTree {
public:
	RTree(vector<DataInstance> instances);
	~RTree();
};

#endif