#include <Data.h>
#include <Rtree.h>
#include <Heap.h>
#include <unordered_set>

using namespace std;

struct ObjectInfo {
	vector<int>& DominatingObjectIDs;
	double high, low;
	ObjectInfo(int low, int high, vector<int> &v): low(low), high(high), DominatingObjectIDs(v) {}
};

void topDownSkyline(objectSet& data, int threshold, objectSet& skyline) {

	map<int, ObjectInfo*> about;
	unordered_set<int> nonSkylineIds;
	instanceSet minimumCorners, maximumCorners;

	objectSet::iterator itr = data.begin();
	while(itr!=data.end()) {
	
		int id = *itr->getID();
		DataInstance& Umin = *itr->getMinimumCorner();
		DataInstance& Umax = *itr->getMaximumCorner();
	
		vector<int> v;
		ObjectInfo info(0,1,v);
		about[id] = &info;

		minimumCorners.push_back(&Umin);
		maximumCorners.push_back(&Umax);
		itr++;
	}

	Rtree GlobalRT(minimumCorners);
	Heap H(minimumCorners);

	objectSet::iterator itr = data.begin();
	while(itr!=data.end()) {
		int id = *itr.getID();
		DataInstance &Umax = *itr.getMaximumCorner();
		vector<int>& v = about[id]->DominatingObjectIDs;
		GlobalRT.search(DataInstance::Origin, Umax, v);
		itr++;
	}

	//Define origin of an instance

	while(H.isNotEmpty()) {
		DataInstance& u = H.top();
		DataObject& U = u.getObjectRef();
		H.pop();
		if(nonSkylineIds.find(U.getID())!=nonSkylineIds.end())
			continue;
		if(isDominated(u, maximumCorners)) {
			// update layer, Line 20 as in algo
		}
		if(u.isMinimumCornerOfU()) {
			/*Line 13-16*/
			vector<int>& v = about[U.getID()]->DominatingObjectIDs;
			if(v.size()==1)	continue;
			int Pu = computeSkylineProbabilityOf(u, PossibleDominatingObjectsIDs);
			if(Pu>=threshold) {
				partition(U);
			}
			else {
				nonSkylineIds.push_back(U.getID());
				continue;
			}
		}
		else {
			/*Line 18-28*/
		}
	}

	return;
}