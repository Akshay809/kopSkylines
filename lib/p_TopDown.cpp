#include <Skyline.h>
#include <RTree.h>
#include <Heap.h>
#include <unordered_set>
#include <map>

using namespace std;

objectSet& p_TopDown(objectSet& data, double p) {

	objectSet Skyline;

	/*Probability bounds*/
	double *pHi = new double[data.size()];
	double *pLo = new double[data.size()];
	int lastUsedIndex = 0;
	vector<DataInstances> Umins;

	/*Saving a map of DataObject vs Probability array index, just in case an element gets removed from vector, It won't effect as order no longer matters*/
	map<DataObject*,int> indexOf;
	objectSet::iterator itr;
	for(itr=data.begin();itr!=data.end();++itr) {
		indexOf[*itr] = ++lastUsedIndex;
			pHi[lastUsedIndex] = 1;
			pLo[lastUsedIndex] = 0;
		Umins.push_back((*itr)->Umin);
	}

	RTree T(Umins); // Build an RTree to store all Umins
	Heap H(Umins); // Build a heap on instances, currently on Umin

	return Skyline;
}