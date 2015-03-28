#include <Data.h>

void DataObject::updateMinimumCorner() {
	instanceSet::iterator itr = instances.begin();

	Umin = **itr++;
	while(itr!=instances.end())
		Umin.minimizeWRT(**itr++);
}

void DataObject::updateMaximumCorner() {
	instanceSet::iterator itr = instances.begin();

	Umax = **itr++;
	while(itr!=instances.end())
		Umax.maximizeWRT(**itr++);
}

void DataObject::updateCorners() {
	updateMinimumCorner();
	updateMaximumCorner();
}

void DataObject::addInstance(DataInstance I) {
	/*Purposefully inserting copied instance object, so that an instance once added can be edited only through object*/
	/*Implement instance check*/
	if(I.getObjectID()==id && I.getWeight()>0) {
		instances.push_back(&I);
		objectWeight += I.getWeight();
		updateCorners();
		if(instancesAdded++ == 0) {
			DataInstance::Origin = I;
		}
	}
}

void DataObject::removeInstance(DataInstance &I) {
	removeInstance(I.getInstanceID());
}

void DataObject::removeInstance(int instanceID) {
	instanceSet::iterator itr = instances.begin();
	while(itr!=instances.end()) {
		if(*itr->getInstanceID()==instanceID && *itr->getObjectID()==id) {
			objectWeight -= *itr->getWeight();
			instances.erase(itr);
			updateCorners();
			break;
		}
		itr++;
	}
}
