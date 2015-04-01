#include <Data.h>

DataInstance& DataInstance::operator= (const DataInstance& I) {
	if(this!=&I) {
		weight = 0;
		/*Should not shallow copy o.w. editing Datastore of this object will affect I as well*/
		dataStore = I.getDataStore();
		/*Performming deep copy*/
		DataIterator itr = dataStore.begin();
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
	DataMap& dataStoreOfI = I.getDataStore();
	DataIterator itrI, itrU = dataStore.begin();

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

void DataInstance::minimizeWRT(DataInstance& I) {
	DataMap& dataStoreOfI = I.getDataStore();
	DataIterator itrI, itrU = dataStore.begin();

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
	DataIterator itrI, itrU = dataStore.begin();

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
	DataIterator itr = dataStore.begin();
	while(itr!=dataStore.end()) {
		if(itr->second!=NULL)
			(itr->second)->minimize();
		itr++;
	}
}