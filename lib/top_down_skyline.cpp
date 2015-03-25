#include <objects.h>
#include <Rtree.h>
#include <heap.h>
#include <unordered_set>

typedef vector<DataObject> objectSet;
typedef vector<DataInstance> instanceSet;

void minimize(DataInstance &Umin, DataInstance &I) {

	DataMap attributeMapOfUmin = Umin.dataStore;
	DataMap attributeMapOfI = I.dataStore;

	DataIterator itrUmin = attributeMapOfUmin.begin();
	DataIterator itrI = attributeMapOfI.begin();
	while(itrUmin!=attributeMapOfUmin.end()) {
		itrUmin->second.updateIfLargerThan(itrI->second);
		itrUmin++; itrI++;
	}

}

void findMinimumCorner(DataObject &U, instanceSet &minimumCorners) {

		instanceSet instances = U.instances;
		DataInstance Umin(instances[0]);

		for(int j=1; j<instances.size(); j++)
			minimize(Umin, instances[j]);

		minimumCorners.push_back(Umin);
	}

}

void topDownSkyline(objectSet &data, int threshold, objectSet &skyline) {

	unordered_set<int> nonSkylineId;
	instanceSet minimumCorners;

	double * probabilityUpperBound = new double[data.size()];
	double * probabilityLowerBound = new double[data.size()];

	for(int i=0; i<data.size(); i++) {
		probabilityLowerBound[i] = 0;
		probabilityUpperBound[i] = 1;
		findMinimumCorner(data[i], minimumCorners);
	}

	Rtree Tg(minimumCorners);
	Heap Hg(minimumCorners);

	while(Hg.isNotEmpty()) {
		DataInstance u = Hg.top();
	}

	return;
}