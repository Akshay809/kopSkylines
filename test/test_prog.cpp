#include <iostream>
#include <string>
#include <DataReader.h>
#include <kdTree.h>

using namespace std;

int main(int argc, char const *argv[])
{
	if(argc!=2) return -1;
	string Fname = argv[1];

	vector<DataInstance> Umins;

	vector<DataObject*>& DataObjects = readData(Fname);
	cout << "Number of objects read: " << DataObjects.size() << endl;
	for (vector<DataObject*>::iterator itr = DataObjects.begin(); itr != DataObjects.end(); ++itr) {
//		(*itr)->printDataObject();
		Umins.push_back((*itr)->Umin);
	}
	kdTree T(Umins);
	// T.printTree();
	DataInstance le(DataObject::Origin);
	DataInstance ue(DataObject::Origin);
	// le = DataInstance::Origin;
	vector<double> v1 = {0.0, 0.0};
	vector<double> v2 = {28.0, 5};
	le.updateDS(v1);
	ue.updateDS(v2);
	// le.printDataInstance();
	// ue.printDataInstance();
	Rectangle R(le, ue);
	unordered_set<DataObject*>* ans = T.searchR(R);
	// cout << " Answer: " << endl;
	// for (unordered_set<DataObject*>::iterator itr = ans->begin(); itr != ans->end(); ++itr) {
	// 	(*itr)->printDataObject();
	// }

	return 0;
}