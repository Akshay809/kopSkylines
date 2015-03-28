#include <Data.h>

DataInstance& DataInstance::operator= (const DataInstance& I) {
	if(this!=&I) {
		weight = 0;
		dataStore = I.getDataStore();
	}
	return *this;
}

bool DataInstance::isMinimumCornerOfU() {
	DataInstance Umin = Object.getMinimumCorner();
	return this==&Umin;
}

bool DataInstance::isDominatedBy(DataInstance& I) {
	DataMap dataStoreOfI = I.getDataStore();
	DataIterator itrU = dataStore.begin();
	DataIterator itrI = dataStoreOfI.begin();
	while(itrU!=dataStore.end() && itrI!=dataStoreOfI.end()) {
		if((itrU->second)->compareWith(*(itrI->second)) == -1)
			return false;
		itrU++; itrI++;
	}
	return true;
}

void DataInstance::minimizeWRT(DataInstance& I) {
	DataMap dataStoreOfI = I.getDataStore();
	DataIterator itrU = dataStore.begin();
	DataIterator itrI = dataStoreOfI.begin();
	while(itrU!=dataStore.end() && itrI!=dataStoreOfI.end()) {
		(itrU->second)->updateIfLargerThan(*(itrI->second));
		itrU++; itrI++;
	}
}

void DataInstance::maximizeWRT(DataInstance& I) {
	DataMap dataStoreOfI = I.getDataStore();
	DataIterator itrU = dataStore.begin();
	DataIterator itrI = dataStoreOfI.begin();
	while(itrU!=dataStore.end() && itrI!=dataStoreOfI.end()) {
		(itrU->second)->updateIfSamllerThan(*(itrI->second));
		itrU++; itrI++;
	}
}

void DataInstance::minimizeDS() {
	DataIterator itr = dataStore.begin();
	while(itr!=dataStore.end()) {
		(itr->second)->minimize();
		itr++;
	}
}