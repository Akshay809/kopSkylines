#include <Data.h>
#include <iostream>

using namespace std;

int DataObject::totalObjects = 0;
DataObject DataObject::Origin;

void DataObject::updateMinimumCorner() {
	vector<DataInstance>::iterator itr = instances.begin();

	Umin = *itr++;
	while(itr!=instances.end())
		Umin.minimizeWRT(*itr++);
}

void DataObject::updateMaximumCorner() {
	vector<DataInstance>::iterator itr = instances.begin();

	Umax = *itr++;
	while(itr!=instances.end())
		Umax.maximizeWRT(*itr++);
}

void DataObject::updateCorners() {
	updateMinimumCorner();
	updateMaximumCorner();
}

void DataObject::addInstance(DataInstance& I) {
	/*Implement instance check*/
	if(I.getObjectID()==id && I.weight>0) {
		/*copy of the instance will be pushed*/
		instances.push_back(I);
		objectWeight += I.weight;
		updateCorners();
		DataInstance::Origin = I;
		// instancesAdded++;
	}
}

void DataObject::removeInstance(DataInstance &I) {
	if(I.getObjectID()==id)
		removeInstance(I.getInstanceID());
}

void DataObject::removeInstance(int instanceID) {
	vector<DataInstance>::iterator itr = instances.begin();
	while(itr!=instances.end()) {
		if(itr->getInstanceID()==instanceID) {
			objectWeight -= itr->weight;
			instances.erase(itr);
			updateCorners();
			break;
		}
		itr++;
	}
}

void DataObject::printDataObject() {
	cout << "\n\n  Object::Id: " << id << endl;
	cout << "  Object::Weight: " << objectWeight << endl;
	cout << "  Object::Object Memory Location: " << this << endl;
	cout << "  Object::Instances Total Count: " << instances.size() << endl;
	vector<DataInstance>::iterator itr = instances.begin();
	while(itr!=instances.end()) {
		itr->printDataInstance();
		itr++;
	}
	cout << "  Object::Umin " << endl;
	Umin.printDataInstance();
	cout << "  Object::Umax " << endl;
	Umax.printDataInstance();
}
