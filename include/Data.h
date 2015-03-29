#ifndef DATA_H
#define DATA_H

#include <vector>
#include <map>
#include <string>

using namespace std;

class DataValue {
public:
 	DataValue() {}
	// CompareWith(I): return -1 if lesser, 0 if equal, 1 if greater
	// UpdateTo(I): updates value of current object to match I
	virtual int compareWith(DataValue&) = 0;
	virtual void updateTo(DataValue&) = 0;
	virtual void *objectReference() = 0;
	virtual void minimize() = 0;

	void updateIfLargerThan(DataValue&);
	void updateIfSmallerThan(DataValue&);
};

class IntDataValue : public DataValue {
private:
	int value;
public:
	static int min;
	IntDataValue(int value): value(value) {}

	void *objectReference() { return this; }
	int getValue() { return value; }

	int compareWith(DataValue&);
	void updateTo(DataValue&);
	void minimize() {
		value = min;
	}
};
int IntDataValue::min = 0;

class DoubleDataValue : public DataValue {
private:
	double value;
public:
	static double min;
	DoubleDataValue(double value): value(value) {}

	void *objectReference() { return this; }
	int getValue() { return value; }

	int compareWith(DataValue&);
	void updateTo(DataValue&);
	void minimize() {
		value = min;
	}
};
double DoubleDataValue::min = 0.0;

typedef map<string, DataValue*> DataMap;
typedef map<string, DataValue*>::iterator DataMapIterator;


class DataInstance {
private:
	int instanceId, objectId, weight;
	DataObject& Object;
	DataMap dataStore;
	// DataInstance(const DataInstance&);
public:
	static int totalInstances;
	static DataInstance Origin;
	DataInstance(DataObject& Object): objectId(Object.getID()), Object(Object), instanceId(++totalInstances) {}

	DataInstance& operator= (const DataInstance&);

	int getObjectID() { return objectId; }
	int getWeight() { return weight; }
	int getInstanceID() { return instanceId; }

	DataObject& getObjectRef() { return Object; }
	const DataMap& getDataStore() { return dataStore; }

	bool isMinimumCornerOfU();
	bool isDominatedBy(DataInstance&);
	void minimizeWRT(DataInstance&);
	void maximizeWRT(DataInstance&);
	void minimizeDS();
};

typedef vector<DataInstance*> instanceSet;

class DataObject {
private:
	int id, objectWeight;
	instanceSet instances;
	DataInstance Umin, Umax;
public:
	static int totalObjects, instancesAdded;
	static DataObject Origin;
	DataObject(): id(++totalObjects), Umin(id, this), Umax(id, this), objectWeight(0) {}

	int getID() { return id; }
	int getObjectWeight() { return objectWeight; }

	void updateCorners();
	void updateMinimumCorner();
	void updateMaximumCorner();
	DataInstance& getMinimumCorner() { return Umin; }
	DataInstance& getMaximumCorner() { return Umax;	}

	void addInstance(DataInstance&);
	void removeInstance(int);
	void removeInstance(DataInstance&);
};

typedef vector<DataObject*> objectSet;

int DataObject::totalObjects = 0;
DataObject DataObject::Origin;
int DataObject::instancesAdded = 0;
int DataInstance::totalInstances = 0;
DataInstance DataInstance::Origin(DataObject::Origin);

bool isDominated(DataInstance& u, instanceSet& set) {
	instanceSet::iterator itr = set.begin();
	while(itr!=set.end()) {
		if(u.isDominatedBy(**itr++));
			return true;
	}
	return false;
}

#endif