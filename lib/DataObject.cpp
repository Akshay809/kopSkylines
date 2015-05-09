#include <iostream>

#include <Data.h>

using namespace std;

int DataObject::totalObjects = 0;
DataObject DataObject::Origin;

const DataInstance& DataObject::getUmin() {
	updateMinimumCorner();
	return Umin;
}

const DataInstance& DataObject::getUmax() {
	updateMaximumCorner();
	return Umax;
}

const DataInstance& DataObject::getUmin() const {
	return Umin;
}

const DataInstance& DataObject::getUmax() const {
	return Umax;
}

void DataObject::updateCorners() {
	updateMinimumCorner();
	updateMaximumCorner();
}

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

void DataObject::addInstance(const DataInstance& I) {
	/*Implement instance check*/
	if(I.getObjectID()==id && I.getWeight()>0) {
		/*copy of the instance will be pushed*/
		instances.push_back(I);
		objectWeight += I.getWeight();
		updateCorners();
		DataInstance::Origin = I;
		// instancesAdded++;
	}
}

void DataObject::removeInstance(int instanceId) {
	vector<DataInstance>::iterator itr = instances.begin();
	while(itr!=instances.end()) {
		if(itr->instanceId==instanceId) {
			objectWeight -= itr->getWeight();
			instances.erase(itr);
			updateCorners();
			break;
		}
		itr++;
	}
}

void DataObject::removeInstance(const DataInstance& I) {
	if(I.getObjectID()==id)
		removeInstance(I.instanceId);
}

void DataObject::printDataObject() const {
	cout << "\n\n  Object::Id: " << id << endl;
	cout << "  Object::Weight: " << objectWeight << endl;
	cout << "  Object::Object Memory Location: " << this << endl;
	cout << "  Object::Instances Total Count: " << instances.size() << endl;
	auto itr = instances.begin();
	while(itr!=instances.end()) {
		itr->printDataInstance();
		itr++;
	}
	cout << "  Object::Umin " << endl;
	Umin.printDataInstance();
	cout << "  Object::Umax " << endl;
	Umax.printDataInstance();
}
