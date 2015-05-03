#include <iostream>
#include <string>
#include <DataReader.h>
#include <kdTree.h>
#include <Heap.h>

using namespace std;

int main(int argc, char const *argv[])
{
	if(argc!=2) return -1;
	string Fname = argv[1];

	vector<DataInstance> Umins;

	vector<DataObject*>& DataObjects = readData(Fname);
	cout << "Number of objects read: " << DataObjects.size() << endl;
	for (vector<DataObject*>::iterator itr = DataObjects.begin(); itr != DataObjects.end(); ++itr) {
		// (*itr)->printDataObject();
		/*pushing copies, not originals*/
		Umins.push_back((*itr)->Umin);
	}


	kdTree T(Umins);
	// T.printTree();
	DataInstance le(DataObject::Origin);
	DataInstance ue(DataObject::Origin);
	// le = DataInstance::Origin;
	vector<double> v1 = {0.0, 0.0};
	vector<double> v2 = {16.0, 14};
	le.updateDS(v1);
	ue.updateDS(v2);
	// le.printDataInstance();
	// ue.printDataInstance();
	Rectangle R(le, ue);
	vector<int>& ans = T.searchR(R);

	// cout << " Answer: " << endl;
	// for (int i = 0; i < ans.size(); ++i) {
	// 	Umins[ans[i]].printDataInstance();
	// }

	cout << "Heap top...." << endl;
	Heap H(Umins);

	// if(H.isNotEmpty()) {
	// 	DataInstance& I = H.top();
	// 	I.printDataInstance();
	// 	H.pop();
	// 	I = H.top();
	// 	I.printDataInstance();
	// 	H.push(ue);
	// }

	return 0;
}