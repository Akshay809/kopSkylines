#include <Skyline.h>
#include <RTree.h>
#include <Heap.h>
#include <unordered_set>
#include <map>

using namespace std;

objectSet& findDominatingObjectsOf(DataObject& U, RTree& R) {
	objectSet DominatingObjects;
		/*Find DominatingObjects of U using R*/
	return DominatingObjects;
}

double skylineProbabilityOfInstance(DataInstance& u, objectSet& DominatingObjects) {
	/*Find skyline probability of u, by using a local Rtree to find dominating instances and then doing a brute-force search*/
}


objectSet& p_TopDown(objectSet& data, double p) {

	objectSet Skyline;
	unordered_set<int> nonSkylineIds;

	/*Probability bounds*/
	double *pHi = new double[data.size()];
	double *pLo = new double[data.size()];
	int lastUsedIndex = 0;
	vector<DataInstance> Umins, Umaxs;

	/* 3 options: 
		- Alter original DataObject definition to have probability bounds, no,  not generic
		- Extend DataObject class, no, will have to create a full copy of data
		- Use bounds array, but an element can be removed from array, thus assuming linear mapping won't work
	*/
	map<DataObject*,int> indexOf;
	objectSet::iterator itr;
	for(itr=data.begin();itr!=data.end();++itr) {
		indexOf[*itr] = ++lastUsedIndex;
			pHi[lastUsedIndex] = 1;
			pLo[lastUsedIndex] = 0;
		Umins.push_back((*itr)->Umin);
		Umaxs.push_back((*itr)->Umax);
	}

	RTree T(Umins); // Build an RTree to store all Umins
	Heap H(Umins); // Build a heap on instances, currently on Umin

	while(H.isNotEmpty()) {
		DataInstance& u = H.top();
		DataObject& U = u.getObjectRef();
		H.pop();
		
		if(nonSkylineIds.find(U.getID())!=nonSkylineIds.end())
			continue;

		if(u.isDominatedBy(Umaxs)) {
			//// goto line 20 as in algo
		}

		if(u.isMinimumCornerOfU()) {
			objectSet& DominatingObjects = findDominatingObjectsOf(U,R);
			double pu = skylineProbabilityOfInstance(u, DominatingObjects);
			if(pu>=p) {
				/*partition instances of U to layers*/
			}
			else {
				nonSkylineIds.push_back(U.getID());
				continue;				
			}
		}
		else {
			/*TODO: save this in an array for every object*/
			objectSet& DominatingObjects = findDominatingObjectsOf(U,R);
			double pu = skylineProbabilityOfInstance(u, DominatingObjects);
			pLo[indexOf[&U]] += ((double)u.getWeight()/U.getObjectWeight()) * pu;
			if(u.isLastInstanceOfLayer()) {
				/*Update Uprmax*/
			}
			pHi[indexOf[&U]] += pLo[indexOf[&U]] + pMax[indexOf[&U]] * SOMETHING;
			//Don't think beyond this, first complete till this point
		}
	}

	return Skyline;
}