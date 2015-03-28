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

	void updateIfLargerThan(DataValue&);
	void updateIfSmallerThan(DataValue&);
};

class IntDataValue : public DataValue {
private:
	int value;
public:
	IntDataValue(int value): value(value) {}

	void *objectReference() { return this; }
	int getValue() { return value; }

	int compareWith(DataValue&);
	void updateTo(DataValue&);
};

class DoubleDataValue : public DataValue {
private:
	double value;
public:
	DoubleDataValue(double value): value(value) {}

	void *objectReference() { return this; }
	int getValue() { return value; }

	int compareWith(DataValue&);
	void updateTo(DataValue&);
};


typedef map<string, DataValue> DataMap;
typedef map<string, DataValue>::iterator DataMapIterator;


class DataInstance {
private:
	static int totalInstances;
	int instanceId, objectId, weight;
	DataObject& Object;
	DataMap dataStore;
	DataInstance(const DataInstance&);
public:
	DataInstance(int objectId, DataObject& Object): objectId(objectId), Object(Object), instanceId(++totalInstances) {}

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
};

DataInstance::totalInstances = 0;

typedef vector<DataInstance> instanceSet;

class DataObject {
private:
	int id, objectWeight;
	instanceSet instances;
	DataInstance Umin, Umax;
public:
	DataObject(int id): id(id), Umin(id, this), Umax(id, this), objectWeight(0) {}

	void getID() { return id; }
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

#endif