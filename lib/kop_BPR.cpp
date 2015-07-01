/*Bounding_Pruning_Refining-version of kOp-Skyline*/
#include <Data.h>
#include <map>
#include <unordered_set>
#include <time.h>
#include <iostream>

using namespace std;

struct partitionObject {

	int d, currentLayer, currentOffset;
	int *layerCount;
	int **layer;

	partitionObject(int d, int *layerCount, int **layer): d(d), layerCount(layerCount), layer(layer) {}

	void printLayers() {
		for(int i = d; i >= 0; i--) {
			cout << "Layer" << i << "\t";
			for(int j=0; j<layerCount[i]; j++)
				cout << layer[i][j] << " ";
			cout << endl;
		}
	}

	int getNextIndex() {
		while(currentLayer>=0 && currentOffset >= layerCount[currentLayer]) {
			currentOffset = 0;
			currentLayer--;
		}

		if(currentLayer>=0)
			return layer[currentLayer][currentOffset++];
	
		return -1;
	}

	int getCurrentLayer() {
		return currentLayer;
	}

	void reset() {
		currentLayer = d; currentOffset = 0;
	}

	void partition(const vector<DataInstance>& instances) {
// cout << "Partioning" << endl;

		currentLayer = d; currentOffset = 0;
		for(int i = 0; i <= d; ++i)
			layerCount[i] = 0;

		int currentIndex = 0;
		for(auto itr = instances.begin(); itr != instances.end(); itr++) {
			int k = 0;
			for(auto it = instances.begin(); it != instances.end(); it++) {
				if(it==itr) continue;
				int r = (*itr).maxDominate(*it);
				if(k<r) k=r;
			}

			layer[k][layerCount[k]] = currentIndex;
			layerCount[k]++;
			currentIndex++;
		}

		// printLayers();
	}
};


void kopSkyline(const objectSet& data, double p, int k, vector<const DataObject*>& Skyline) {

	clock_t tStart;
	double executionTime = 0;

	int n = data.size(), m = 0, d = 0;
	if(n>0)	d = data[0]->getUmin().getDim();

		/*Object properties*/
		vector<DataInstance> Umins, Umaxs;

	map<const DataObject*,int> indexOf;	/*Used for finding object's property value*/
		int lastUsedIndex = -1;


	for(auto itr=data.cbegin(); itr!=data.cend(); ++itr) {
		indexOf[*itr] = ++lastUsedIndex;
			Umins.push_back((*itr)->getUmin());
			Umaxs.push_back((*itr)->getUmax());

		if(m < (*itr)->getInstancesCount())
			m = (*itr)->getInstancesCount();
	}

		int **layer = new int*[d+1];
			for(int i = 0; i <= d; ++i)
				layer[i] = new int[m];

		int *layerCount = new int[d+1];

	partitionObject P(d, layerCount, layer);


	for(auto itr=data.cbegin(); itr!=data.cend(); ++itr) {

// cout << "Evaluating next Object" << endl;

		const DataObject* U = *itr;
		int indexOfU = indexOf[U];
		const vector<DataInstance>& instancesOfU = U->getDataInstances();

		/*1. Check if Umin is k-dominated by any Vmax*/
		if(Umins[indexOfU].isKDominatedBy(Umaxs, k))
			continue;
// cout << "\tNot completely k-dominated" << endl;

		/*2. Layerize*/
		P.partition(U->getDataInstances());

		tStart = clock();

		int U_m = U->getInstancesCount();

		double *Pku = new double[U_m];
			for(int i=0;i<U_m;i++)
				Pku[i] = 1;

		double pMax = 0;

		for(auto it = data.cbegin(); it != data.cend(); ++it) {
// cout << "\tComparing with another object" << endl;

			const DataObject* V = *it;
			int indexOfV = indexOf[V];

			if(V==U || !(U->getUmax().isKDominatedBy(V->getUmin(), k)))
				continue;

// cout << "\tAnother object's best-case k-dominates my worst case, some possibility exists" << endl;

			double *Vdomu = new double[U_m];
				for(int i=0;i<U_m;i++)
					Vdomu[i] = 0;

			const vector<DataInstance>& instancesOfV = V->getDataInstances();

			for(auto inst = instancesOfV.begin(); inst!= instancesOfV.end(); ++inst) {
// cout << "\tFinding all instances of mine, dominated by this instance" << endl;

				const DataInstance& v = *inst;

				bool *isDecided = new bool[U_m];
				for(int j=0;j<U_m;j++)
					isDecided[j] = false;

				P.reset();
				int currentIndex = P.getNextIndex();
				while(currentIndex!=-1) {
// cout << "\tCurrent Index : " << currentIndex << endl;
					int l = P.getCurrentLayer();

					if(isDecided[currentIndex]) {
						currentIndex = P.getNextIndex();
						continue;
					}
					isDecided[currentIndex] = true;

					int r = v.maxDominate(instancesOfU[currentIndex]);
					if(r<k) {
						currentIndex = P.getNextIndex();
						continue;
					}

					Vdomu[currentIndex] += v.getProbability();

						for(int l_ = d-l; l_ <= r-k; l_++) {
							for(int i=0;i<layerCount[l_];i++) {
								cout << "Solved an extra question Mark" << endl;
								int newIndex = layer[l_][i];
								isDecided[newIndex] = true;

								Vdomu[newIndex] += v.getProbability();
							}
						}

					currentIndex = P.getNextIndex();
				}
			}

			pMax = 0;
			for(int i=0;i<U_m;i++) {
				Pku[i] *= (1-Vdomu[i]);
				pMax += instancesOfU[i].getProbability() * Pku[i];
			}

/*cout << pMax << " " ;
	for(int i=0;i<U_m;i++)
		cout << Pku[i] << " ";
	cout << endl;
*/
			if(pMax<p)
				break;
		}

		if(pMax>=p)
			Skyline.push_back(U);

		executionTime += (double)(clock() - tStart)/CLOCKS_PER_SEC;
	}

	cout << p << " " << k << " " << Skyline.size() << " " << executionTime << endl;

}
