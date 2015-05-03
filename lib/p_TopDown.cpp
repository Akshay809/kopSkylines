#include <Skyline.h>
#include <kdTree.h>
#include <Heap.h>
#include <unordered_set>
#include <map>

using namespace std;

void findDominatingObjects(vector<DataInstance>& Umaxs, vector<vector<DataObject*> >& DominatingObjectsOf, kdTree T) {
	for (int i = 0; i < Umaxs.size(); ++i) {
		/*Find DominatingObjects of U using T*/
		DataInstance le(DataObject::Origin);
		DataInstance ue(DataObject::Origin);
		le = DataInstance::Origin;
		// vector<double> v1 = {0.0, 0.0};
		// le.updateDS(v1);
		ue = Umaxs[i];
		// vector<double> v2 = {28.0, 5};
		// ue.updateDS(v2);
		Rectangle R(le, ue);
		vector<int>& ans = T.searchR(R);

		/*Not including itself*/
		for (int j = 0; j < ans.size(); ++j) {
			if(ans[j] != i)
				DominatingObjectsOf[i].push_back(Umaxs[ans[j]].getRefObjAdd());
		}
	}
}

struct partitionObject {
	DataObject& U;
	vector<vector<DataInstance> >& layer;
	vector<DataInstance> instances;

	partitionObject(DataObject& U, vector<vector<DataInstance> >& layer): U(U), layer(layer) {
		instances = U.getDataInstances();
	}

	bool lower(const DataInstance& i1, const DataInstance& i2) {
		return (i1.getKey())<(i2.getKey());
	}

	bool isDominatedAtLayer(DataInstance& I, int at) {
		return I.isDominatedBy(layer[at]);
	}

	void partition() {
		if(instances.size()==0) return;

		std::sort(instances.begin(), instances.end(), lower);
		vector<DataInstance>::iterator itr;

		int maxLayer = 0;

		for(itr = instances.begin() ; itr != instances.end(); ++itr) {
			int correctLayer = maxLayer+1;
			int start = 0, end = maxLayer;
			while(start<=end) {
				int suggestedLayer = (start+end)/2;
				if(isDominatedAtLayer(I, suggestedLayer)) {
					start = suggestedLayer+1;
				}
				else {
					correctLayer = suggestedLayer;
					end = suggestedLayer-1;
				}
			}
			layer[correctLayer].push_back(*itr);
		}
	}
};

double skylineProbabilityOfInstance(DataInstance& u, vector<DataObject*>& DominatingObjects) {
	/*Find skyline probability of u, by using a local kdTree to find dominating instances and then doing a brute-force search*/
	double pu = 1;
	for (int i = 0; i < DominatingObjects.size(); ++i) {
		DataObject& V = *DominatingObjects[i];

		vector<DataInstance> instances = V.getDataInstances();
		for(vector<DataInstance>::iterator itr = instances.begin(); itr != instances.end(); ++itr) {
			if(itr->getKey()>u.getKey())
				instances.erase(itr--);
		}

		kdTree Tv(instances);

		DataInstance le(DataObject::Origin);
		le = DataInstance::Origin;
		Rectangle R(le, u);

		vector<int>& ans = Tv.searchR(R);

		double _pu = 0;
		for(int i=0; i<ans.size(); ++i) {
			DataInstance& v = instances[ans[i]];
			if(u.isDominatedBy(v))
				_pu += v.getProbability();
		}
		pu *= (1-_pu);
		if(pu==0) return pu;
	}
	return pu;
}


objectSet& p_TopDown(objectSet& data, double p) {

	objectSet Skyline;
	unordered_set<int> nonSkylineIds;

	/*Probability bounds*/
	vector<double> pHi, pLo, pMax, layerMax;
	vector<DataInstance> Umins, Umaxs;
	vector<vector<DataObject*> > DominatingObjectsOf;
	vector<int> weightLeft;
	vector<vector<vector<DataInstance> > > layers;
	vector<int> currentLayer;

	map<DataObject*,int> indexOf;
	int lastUsedIndex = -1;

	objectSet::iterator itr;
	for(itr=data.begin();itr!=data.end();++itr) {
		indexOf[*itr] = ++lastUsedIndex;
		pHi.push_back(1);
		pLo.push_back(0);
		pMax.push_back(1);
		layerMax.push_back(0);
		Umins.push_back((*itr)->Umin);
		Umaxs.push_back((*itr)->Umax);
		weightLeft.push_back((*itr)->getObjectWeight());
		layers.push_back(vector<vector<DataInstance> >());
		currentLayer.push_back(0);
	}

	kdTree T(Umins); // Build a kdTree to store all Umins
	Heap H(Umins); // Build a heap on instances, currently on Umin

	findDominatingObjects(Umaxs, DominatingObjectsOf, T);

	while(H.isNotEmpty()) {
		DataInstance& u = H.top();
		DataObject* U = u.getRefObjAdd();
		H.pop();
		double pu;

		if(nonSkylineIds.find(U->getID())!=nonSkylineIds.end())
			continue;

		/*Update Umaxs to remove Umax of same object, not necessary though, since my Umax cannot dominate me*/
			vector<DataInstance> Umaxs_ = Umaxs;
			Umaxs_.erase(Umaxs_.begin()+indexOf[U]);
		if(u.isDominatedBy(Umaxs_)) pu = 0;
		else pu = skylineProbabilityOfInstance(u, DominatingObjectsOf[indexOf[U]]);

		vector<vector<DataInstance> >& layer = layers[indexOf[U]];
		int at = currentLayer[indexOf[U]];

		if(u.getInstanceID() == U->Umin.getInstanceID()) {
			if(pu>=p) {
				/*partition instances of U to layers*/
				partitionObject P(*U, layers[indexOf[U]]);
				P.partition();

				/*insert next element to heap*/
				if(layer[at].size()==0) continue;
				vector<DataInstance>::iterator itr = layer[at].begin();
				H.push(*itr);
				layer[at].erase(itr);
			}
			else {
				nonSkylineIds.push_back(U->getID());
			}
			continue;
		}
		else {
			layerMax[indexOf[U]] = std::max(pu, layerMax[indexOf[U]]);
			weightLeft[indexOf[U]] -= u.weight;
			pLo[indexOf[U]] += u.getProbability() * pu;
		}

		if(layer[at].size()==0) {
			/*Update pMax[indexOf[U]]*/
			pMax[indexOf[U]] = layerMax[indexOf[U]];
			layerMax[indexOf[U]] = 0;
			currentLayer[indexOf[U]]++;
		}
		pHi[indexOf[U]] = pLo[indexOf[U]] + pMax[indexOf[U]] * (((double)weightLeft[indexOf[U]])/U->getObjectWeight());

		if(pLo[indexOf[U]]>=p) {
			Skyline.push_back(U);
			continue;
		}

		if(pHi[indexOf[U]]<p) {
			nonSkylineIds.push_back(U->getID());
			continue;
		}

		/*insert next element to heap*/
		at = currentLayer[indexOf[U]];
		if(layer[at].size()==0) continue;
		vector<DataInstance>::iterator itr = layer[at].begin();
		H.push(*itr);
		layer[at].erase(itr);
	}

	return Skyline;
}