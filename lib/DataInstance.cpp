#include <Data.h>
#include <iostream>

using namespace std;

int DataInstance::totalInstances = 0;
DataInstance DataInstance::Origin(DataObject::Origin);

int DataInstance::getObjectID() { return Object.getID(); }

DataInstance& DataInstance::operator= (const DataInstance& I) {
	if(this!=&I) {
		weight = 0;
		/*Should not shallow copy o.w. editing Datastore of this object will affect I as well*/
		dataStore = I.getDataStore();
		/*Performming deep copy*/
		DataMapIterator itr = dataStore.begin();
		while(itr!=dataStore.end()) {
			if(itr->second!=NULL){
				DataValue& copyValue = (itr->second)->createCopy();
				itr->second = &copyValue;
			}
			itr++;
		}
		/*Check for origin and minimize*/
		if(this->instanceId==1)
			minimizeDS();
	}
	return *this;
}

bool DataInstance::isMinimumCornerOfU() {
	DataInstance& Umin = Object.getMinimumCorner();
	return this==&Umin;
}

bool DataInstance::isDominatedBy(DataInstance& I) {
	DataMap dataStoreOfI = I.getDataStore();
	DataMapIterator itrI, itrU = dataStore.begin();

	while(itrU!=dataStore.end()) {
		itrI = dataStoreOfI.find(itrU->first);

		if ( itrI==dataStoreOfI.end() || (itrU->second==NULL && itrI->second==NULL)) {
			itrU++;
			continue;
		}
		// Handling missing values
		if(itrU->second==NULL || itrI->second==NULL) {
			return false;
		}
		//IMP: Using strictly less than as preference function for each attribute, thus -1
		if((itrU->second)->compareWith(*(itrI->second)) == -1)
			return false;
		itrU++;
	}
	return true;
}

bool DataInstance::isDominatedBy(instanceSet& set) {
	instanceSet::iterator itr = set.begin();
	while(itr!=set.end()) {
		if(this->isDominatedBy(*itr++));
			return true;
	}
	return false;
}

void DataInstance::minimizeWRT(DataInstance& I) {
	DataMap dataStoreOfI = I.getDataStore();
	DataMapIterator itrI, itrU = dataStore.begin();

	while(itrU!=dataStore.end()) {
		itrI = dataStoreOfI.find(itrU->first);

		if ( itrI==dataStoreOfI.end() || itrI->second==NULL) {
			itrU++;
			continue;
		}

		if( itrU->second!=NULL )
			(itrU->second)->updateIfLargerThan(*(itrI->second));
		else {
			DataValue& copyValue = (itrI->second)->createCopy();
			itrU->second = &copyValue;
		}
		itrU++;
	}
}

void DataInstance::maximizeWRT(DataInstance& I) {
	DataMap dataStoreOfI = I.getDataStore();
	DataMapIterator itrI, itrU = dataStore.begin();

	while(itrU!=dataStore.end()) {
		itrI = dataStoreOfI.find(itrU->first);

		if ( itrI==dataStoreOfI.end() || itrI->second==NULL) {
			itrU++;
			continue;
		}

		if( itrU->second!=NULL )
			(itrU->second)->updateIfSmallerThan(*(itrI->second));
		else {
			DataValue& copyValue = (itrI->second)->createCopy();
			itrU->second = &copyValue;
		}
		itrU++;
	}
}

void DataInstance::minimizeDS() {
	DataMapIterator itr = dataStore.begin();
	while(itr!=dataStore.end()) {
		if(itr->second!=NULL)
			(itr->second)->minimize();
		itr++;
	}
}

void DataInstance::printDataInstance() {
	cout << "  Instance::ID: " << instanceId << endl;
	cout << "  Instance::ObjectID: " << getObjectID() << endl;
	cout << "  Instance::Weight: " << weight << endl;
	cout << "  Instance::Memory Location: " << this << endl;
	cout << "  Instance::Object Memory Location: " << &Object << endl;
	cout << "  Instance::DataStore Memory Location: " << &dataStore << endl;

	DataMapIterator itr = dataStore.begin();
	while(itr!=dataStore.end()) {
		cout << "    " << itr->first << " -> ";
		if(itr->second!=NULL)
			itr->second->printDataValue();
		itr++;
	}
}
