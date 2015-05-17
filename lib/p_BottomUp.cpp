#include <iostream>
#include <string>
#include <unordered_set>
#include <map>

#include <kdTree.h>
#include <Heap.h>

using namespace std;

void printInfo(vector<const DataObject*>& Skyline, unordered_set<int>& nonSkylineIds, vector<double>& pHi, vector<double>& pLo, vector<double>& pMax, vector<double>& layerMax, vector<int>& weightLeft, vector<int>& currentLayer, map<const DataObject*,int>& indexOf) {

	cout << endl;

	cout << "PLo: ";
	for(int i=0; i< pLo.size(); i++) 
		cout << pLo[i] << " ";
	cout << endl;
	cout << "PHi: ";
	for(int i=0; i< pHi.size(); i++)
		cout << pHi[i] << " ";
	cout << endl;
	cout << "LHi: ";
	for(int i=0; i< layerMax.size(); i++)
		cout << layerMax[i] << " ";
	cout << endl;
	cout << "PMx: ";
	for(int i=0; i< pMax.size(); i++)
		cout << pMax[i] << " ";
	cout << endl;
	cout << "Wlt: ";
	for(int i=0; i< weightLeft.size(); i++)
		cout << weightLeft[i] << " ";
	cout << endl;
	cout << "Clr: ";
	for(int i=0; i< currentLayer.size(); i++)
		cout << currentLayer[i] << " ";
	cout << endl;
	cout << "Skyline: ";
	for(int i=0; i< Skyline.size(); i++)
		cout << indexOf[Skyline[i]] << " ";
	cout << endl;
	cout << "Non-Skyline: ";
	for(unordered_set<int>::iterator i = nonSkylineIds.begin(); i != nonSkylineIds.end(); ++i)
		cout << *i << " ";
	cout << endl;
}

void findDominatingObjects(vector<DataInstance>& Umaxs, vector<vector<const DataObject*> >& DominatingObjectsOf, kdTree& T) {

	DataInstance le(DataObject::Origin);
	DataInstance ue(DataObject::Origin);

	le = DataInstance::Origin;

	for (int i = 0; i < Umaxs.size(); ++i) {

		ue = Umaxs[i];
		Rectangle R(le, ue);

		const vector<int>& ans = T.searchR(R);

		for (int j = 0; j < ans.size(); ++j) {
			/*Not including itself*/
			if(ans[j] != i)
				DominatingObjectsOf[i].push_back(Umaxs[ans[j]].getRefObjAdd());
		}
	}
}

struct partitionObject {
	const DataObject& U;
	vector<vector<DataInstance> >& layer;
	const vector<DataInstance>& instances;
	vector<int> indices;

	// partitionObject(DataObject& U, vector<vector<DataInstance> >& layer): U(U), layer(layer), indices(vector<int>()) {
	partitionObject(const DataObject& U, vector<vector<DataInstance> >& layer): U(U), layer(layer), instances(U.getDataInstances()) {
		// instances = U.getDataInstances();
		for(int i=0; i<instances.size();i++)
			indices.push_back(i);
	}

	struct lower {
		const vector<DataInstance>& instances;
		lower(const vector<DataInstance>& instances): instances(instances) {}
		bool operator()(const int& i1, const int& i2) {
			/*TODO: check if >*/
			return instances[i1].getKey() < instances[i2].getKey();
		}
	};

	bool isDominatedAtLayer(const DataInstance& I, int at) {
		if(layer.empty() || layer[at].empty()) {
			return false;
		}
		return I.isDominatedBy(layer[at]);
	}

	void printLayers() {
		cout << "\nLayer Info\n" << U.id << endl;
		if(layer.empty()) {
			cout << "No layers" << endl;
			return;
		}
		for(int i=0; i<layer.size(); i++) {
			cout << "Layer" << i+1 << endl;
			for(int j=0; j<layer[i].size(); j++) {
				layer[i][j].printDataInstance();
			}
		}
	}

	void partition() {
		if(instances.size()==0) return;

		std::sort(indices.begin(), indices.end(), lower(instances));
		vector<int>::iterator itr;

		int maxLayer = 0;

		for(itr = indices.begin() ; itr != indices.end(); ++itr) {
			int correctLayer = maxLayer+1;
			int start = 0, end = maxLayer;
			while(start<=end) {
				int suggestedLayer = (start+end)/2;
				if(isDominatedAtLayer(instances[*itr], suggestedLayer)) {
					start = suggestedLayer+1;
				}
				else {
					correctLayer = suggestedLayer;
					end = suggestedLayer-1;
				}
			}
			if(correctLayer==maxLayer+1) { maxLayer++; }
			if(layer.empty() || correctLayer >= layer.size())
				layer.push_back(vector<DataInstance>{instances[*itr]});
			else
				layer[correctLayer].push_back(instances[*itr]);
		}
		// printLayers();
	}
};

double skylineProbabilityOfInstance(DataInstance& u, vector<const DataObject*>& DominatingObjects) {
	/*Find skyline probability of u, by using a local kdTree to find dominating instances and then doing a brute-force search*/
	double pu = 1;
	for (int i = 0; i < DominatingObjects.size(); ++i) {
		const DataObject& V = *DominatingObjects[i];

		// V.printDataObject();

		vector<DataInstance> final_instances;
		const vector<DataInstance>& instances = V.getDataInstances();

		for(auto itr = instances.cbegin(); itr != instances.cend(); ++itr) {
			if((itr->getKey())<(u.getKey())) {
				// cout << "Pushing..." << endl;
				final_instances.push_back(*itr);
			}
		}

		// cout << "Final instances.. " << endl;

		DataInstance le(DataObject::Origin);
		le = DataInstance::Origin;
		Rectangle R(le, u);

		kdTree * Tv = new kdTree(final_instances);
			const vector<int>& ans = Tv->searchR(R);
		delete Tv;

		double _pu = 0;
		// cout << "Ans size " << ans.size() << endl;
		for(int i=0; i<ans.size(); ++i) {
			DataInstance& v = final_instances[ans[i]];
			if(u.isDominatedBy(v)) {
				// u.printDataInstance();
				// v.printDataInstance();
				// cout << "probability contri.. " << v.getProbability() << endl;
				_pu += v.getProbability();
			}
		}
		pu *= (1-_pu);
		if(pu==0) return pu;
	}
	return pu;
}

void p_BottomUp(const objectSet& data, double p, vector<const DataObject*>& Skyline) {

	unordered_set<int> nonSkylineIds;

		/*Object properties*/
		vector<double> pHi, pLo, pMax, layerMax;
		vector<DataInstance> Umins, Umaxs;
		vector<vector<const DataObject*> > DominatingObjectsOf;
		vector<int> weightLeft;
		vector<vector<vector<DataInstance> > > layers;
		vector<int> currentLayer;
		vector<int> currentLayerIndex;
		vector<bool> isMin;

	map<const DataObject*,int> indexOf;	/*Used for finding object's property value*/
		int lastUsedIndex = -1;

	// cout << "*******************************\n";
	for(auto itr=data.cbegin(); itr!=data.cend(); ++itr) {
		// (*itr)->printDataObject();
		indexOf[*itr] = ++lastUsedIndex;
			pHi.push_back(1);
			pLo.push_back(0);
			pMax.push_back(1);
			layerMax.push_back(0);
		Umins.push_back((*itr)->getUmin());
		Umaxs.push_back((*itr)->getUmax());
		weightLeft.push_back((*itr)->getObjectWeight());
		DominatingObjectsOf.push_back(vector<const DataObject*>());

		layers.push_back(vector<vector<DataInstance> >());
		currentLayer.push_back(0);
		currentLayerIndex.push_back(0);
		isMin.push_back(true);
	}
	// cout << "*******************************\n";

	kdTree * T = new kdTree(Umins); // Build a kdTree to store all Umins
		findDominatingObjects(Umaxs, DominatingObjectsOf, *T);
	delete T;

	Heap H(Umins); 	// Build a heap on instances, currently on Umin
	/*SideEffect: Umins gets changed by heap operations, donot swap above two operation order*/

	while(H.isNotEmpty()) {
		
		// printInfo(Skyline, nonSkylineIds, pHi, pLo, pMax, layerMax, weightLeft, currentLayer, indexOf);
		
		DataInstance& u = H.top();
		H.pop();

		// cout << "Instance popped" << endl;
		// u.printDataInstance();

		const DataObject* U = u.getRefObjAdd();
		int indexOfU = indexOf[U];
		double pu;

		if(nonSkylineIds.find(U->id)!=nonSkylineIds.end())
			continue;

		/*Updating Umaxs to remove Umax of same object not necessary, since my Umax cannot dominate me*/
		if(u.isDominatedBy(Umaxs)) pu = 0;
		else pu = skylineProbabilityOfInstance(u, DominatingObjectsOf[indexOfU]);

		// cout << "Skyline probability: " << pu << endl;

		vector<vector<DataInstance> >& layerOfU = layers[indexOfU];
		int at = currentLayer[indexOfU];

		if(isMin[indexOfU]) {

			isMin[indexOfU] = false;
			// cout << "Min Object, partioning..." << endl;

			if(pu>=p) {
				/*partition instances of U to layers*/
				partitionObject P(*U, layerOfU);
				P.partition();

				/*insert next element to heap*/
				if(layerOfU[at].size()==currentLayerIndex[indexOfU]) {
					cout << "Nothing left to insert. Exiting." << endl;
					continue;
				}
				DataInstance& pushInstance = layerOfU[at][currentLayerIndex[indexOfU]];
				H.push(pushInstance);
				currentLayerIndex[indexOfU]++;
				// layerOfU[at].erase(itr);
			}
			else {
				nonSkylineIds.insert(U->id);
			}
			continue;
		}
		else {
			layerMax[indexOfU] = std::max(pu, layerMax[indexOfU]);
			weightLeft[indexOfU] -= u.getWeight();
			// cout << "PLo compute.. " << pLo[indexOfU] << " " << u.getProbability() << " " << pu << endl;
			pLo[indexOfU] += u.getProbability() * pu;
		}

		if(currentLayerIndex[indexOfU]==layerOfU[at].size()) {
			/*Update pMax[indexOfU]*/
			if(layerMax[indexOfU]!=0)
				pMax[indexOfU] = layerMax[indexOfU];
			layerMax[indexOfU] = 0;
			currentLayer[indexOfU]++;
			currentLayerIndex[indexOfU] = 0;
		}
		pHi[indexOfU] = pLo[indexOfU] + pMax[indexOfU] * (((double)weightLeft[indexOfU])/U->getObjectWeight());

		if(pLo[indexOfU]>=p) {
			Skyline.push_back(U);
			continue;
		}

		if(pHi[indexOfU]<p) {
			nonSkylineIds.insert(U->id);
			continue;
		}

		/*insert next element to heap*/
		at = currentLayer[indexOfU];
		if(layerOfU[at].size()==currentLayerIndex[indexOfU]) continue;
		DataInstance& pushInstance = layerOfU[at][currentLayerIndex[indexOfU]];
		H.push(pushInstance);
		currentLayerIndex[indexOfU]++;
		// layerOfU[at].erase(itr);
	}
}