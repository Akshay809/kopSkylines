#include <objects.h>
#include <Rtree.h>
#include <heap.h>
#include <unordered_set>

using namespace std;

typedef vector<DataObject> objectSet;

bool isDominated(DataInstance &u, instanceSet &set) {
	for(instanceSet::iterator itr = set.begin(); itr!=set.end();++itr) {
		if(u.isDominatedBy(*itr));
			return true;
	}
	return false;
}

void topDownSkyline(objectSet &data, int threshold, objectSet &skyline) {

	unordered_set<int> nonSkylineObjectIds;
	instanceSet minimumCorners, maximumCorners;

	double * probabilityUpperBound = new double[data.size()];
	double * probabilityLowerBound = new double[data.size()];

	for(int i=0; i<data.size(); i++) {
		probabilityLowerBound[i] = 0;
		probabilityUpperBound[i] = 1;
		minimumCorners.push_back(data[i].getMinimumCorner());
		maximumCorners.push_back(data[i].getMaximumCorner());
	}

	Rtree Tg(minimumCorners);
	Heap Hg(minimumCorners);

	while(Hg.isNotEmpty()) {
		DataInstance u = Hg.top();
		Hg.pop();
		if(nonSkylineObjectIds.find(u.getObjectID())!=nonSkylineObjectIds.end())
			continue;
		if(isDominated(u, maximumCorners)) {
			// call heap update function
		}
		if(u.isMinimumCornerOfU()) {
			/*Line 13-16*/
		}
		else {
			/*Line 18-28*/
		}
	}

	return;
}