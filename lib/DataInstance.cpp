#include <Data.h>
#include <iostream>

using namespace std;

int DataInstance::totalInstances = 0;
DataInstance DataInstance::Origin(DataObject::Origin);

int DataInstance::getObjectID() { return Object.getID(); }

DataInstance& DataInstance::operator= (const DataInstance& I) {
	if(this!=&I) {
		/*Performming deep copy*/
		dataStore = I.getDataStore();
		/*Check for origin and minimize*/
		if(this->instanceId==1)
			minimizeDS();
	}
	return *this;
}

const double DataInstance::getKey() const {
	double key = 0;
	for(int i=0; i<dataStore.size(); ++i)
		key += dataStore[i];
	return key;
}

double DataInstance::getProbability() { 
	return ((double)weight)/Object.getObjectWeight();
}

void DataInstance::updateDS(vector<double>& newDataStore) { 
	if(dataStore.size()==0 || dataStore.size()==newDataStore.size())
		dataStore = newDataStore;
}

bool DataInstance::isDominatedBy(DataInstance& I) {
	vector<double> dataStoreOfI = I.getDataStore();
	if(dataStoreOfI.size()!=dataStore.size()) return false;
	for(int i = 0; i < dataStore.size(); ++i) {
		if(dataStore[i] < dataStoreOfI[i])
			return false;
	}
	return true;
}

bool DataInstance::isDominatedBy(vector<DataInstance>& set) {
	vector<DataInstance>::iterator itr = set.begin();
	while(itr!=set.end()) {
		if(this->isDominatedBy(*itr++))
			return true;
	}
	return false;
}

void DataInstance::minimizeWRT(DataInstance& I) {
	vector<double> dataStoreOfI = I.getDataStore();
	DataStoreIterator itrI, itrU = dataStore.begin();
	for(int i = 0; i < dataStore.size(); ++i) {
		if(dataStore[i] > dataStoreOfI[i])
			dataStore[i] = dataStoreOfI[i];
	}
}

void DataInstance::maximizeWRT(DataInstance& I) {
	vector<double> dataStoreOfI = I.getDataStore();
	DataStoreIterator itrI, itrU = dataStore.begin();
	for(int i = 0; i < dataStore.size(); ++i) {
		if(dataStore[i] < dataStoreOfI[i])
			dataStore[i] = dataStoreOfI[i];
	}
}

void DataInstance::minimizeDS() {
	for(int i = 0; i < dataStore.size(); ++i) {
		dataStore[i] = 0;
	}
}

void DataInstance::printDataInstance() {
	cout << "    Instance::ID: " << instanceId << endl;
	cout << "    Instance::ObjectID: " << getObjectID() << endl;
	cout << "    Instance::Weight: " << weight << endl;
	cout << "    Instance::Memory Location: " << this << endl;
	cout << "    Instance::Object Memory Location: " << &Object << endl;
	cout << "    Instance::DataStore Memory Location: " << &dataStore << endl;

	DataStoreIterator itr = dataStore.begin();
	cout << "       ";
	while(itr!=dataStore.end()) {
		cout << *itr << " ";
		itr++;
	}
	cout << endl;
}
