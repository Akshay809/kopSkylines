#include <iostream>

#include <Data.h>

using namespace std;

int DataInstance::totalInstances = 0;
DataInstance DataInstance::Origin(DataObject::Origin);

int DataObject::totalObjects = 0;
DataObject DataObject::Origin;


DataInstance::DataInstance(const DataInstance& I): Object(I.Object), instanceId(++totalInstances), weight(I.getWeight()), dataStore(I.getDataStore()), nameStore(I.getNameStore()) {}

DataInstance& DataInstance::operator= (const DataInstance& I) {
	if(this!=&I) {
		/*Performming deep copy*/
		// weight = I.getWeight();
		nameStore = I.getNameStore();
		dataStore = I.getDataStore();
		/*Check for origin and minimize*/
		if(this->instanceId==1)
			minimizeDS();
	}
	return *this;
}

int DataInstance::getObjectID() const {
	return Object.id;
}

double DataInstance::getKey() const {
	double key = 0;
	for(int i=0; i<dataStore.size(); ++i)
		key += dataStore[i];
	return key;
}

double DataInstance::getProbability() const {
	return ((double)weight)/Object.getObjectWeight();
}

bool DataInstance::isDominatedBy(const DataInstance& I) const {
	const vector<double>& dataStoreOfI = I.getDataStore();

	if(dataStoreOfI.size()!=dataStore.size()) return false;

	for(int i = 0; i < dataStore.size(); ++i) {
		if(dataStore[i] < dataStoreOfI[i])
			return false;
	}
	for(int i = 0; i < dataStore.size(); ++i) {
		if(dataStore[i] != dataStoreOfI[i])
			return true;
	}
	return false;
}

bool DataInstance::isDominatedBy(const vector<DataInstance>& set) const {
	auto itr = set.cbegin();
	while(itr!=set.cend()) {
		if(isDominatedBy(*itr++))
			return true;
	}
	return false;
}

bool DataInstance::isKDominatedBy(const DataInstance& I, int k) const {
	const vector<double>& dataStoreOfI = I.getDataStore();

	if(dataStoreOfI.size()!=dataStore.size()) return false;

	int atleastAsGood = k, better = 1;

	for(int i = 0; i < dataStore.size(); ++i) {
		if(dataStoreOfI[i] == dataStore[i])
			atleastAsGood--;
		else if(dataStoreOfI[i] < dataStore[i]) {
			atleastAsGood--;
			better--;
		}
	}

	if(better<=0 && atleastAsGood<=0)
		return true;
	return false;
}

bool DataInstance::isKDominatedBy(const vector<DataInstance>& set, int k) const {
	auto itr = set.cbegin();
	while(itr!=set.cend()) {
		if(isKDominatedBy(*itr++, k))
			return true;
	}
	return false;
}

int DataInstance::maxDominate(const DataInstance& I) const {
	const vector<double>& dataStoreOfI = I.getDataStore();

	if(dataStoreOfI.size()!=dataStore.size()) return 0;

	int atleastAsGood = 0, better = 0;

	for(int i = 0; i < dataStore.size(); ++i) {
		if(dataStore[i] == dataStoreOfI[i])
			atleastAsGood++;
		else if(dataStore[i] < dataStoreOfI[i]) {
			atleastAsGood++;
			better++;
		}
	}

	if(better>0)
		return atleastAsGood;
	return 0;
}

void DataInstance::updateStoreTo(const vector<string>& names, const vector<double>& values) {
	if(values.size()!=names.size())
		throw UpdateLengthMismatchException("Number of Keys and Values differ");
	nameStore = names;
	dataStore = values;
}

void DataInstance::addPair(string key, double value) {
	nameStore.push_back(key);
	dataStore.push_back(value);
}

void DataInstance::minimizeWRT(const DataInstance& I) {
	const vector<double>& dataStoreOfI = I.getDataStore();
	for(int i = 0; i < dataStore.size(); ++i) {
		if(dataStore[i] > dataStoreOfI[i])
			dataStore[i] = dataStoreOfI[i];
	}
}

void DataInstance::maximizeWRT(const DataInstance& I) {
	const vector<double>& dataStoreOfI = I.getDataStore();
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

void DataInstance::printDataInstance() const {
	cout << "    Instance::ID: " << instanceId << endl;
	cout << "    Instance::ObjectID: " << getObjectID() << endl;
	cout << "    Instance::Weight: " << weight << endl;
	cout << "    Instance::Memory Location: " << this << endl;
	cout << "    Instance::Object Memory Location: " << &Object << endl;
	cout << "    Instance::DataStore Memory Location: " << &dataStore << endl;

	auto itr = dataStore.cbegin();
	cout << "       ";
	while(itr!=dataStore.cend()) {
		cout << *itr << " ";
		itr++;
	}
	cout << endl;
}
