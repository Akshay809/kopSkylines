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
	virtual void* objectReference() = 0;
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

	void* objectReference() { return this; }
	int getValue() { return value; }

	int compareWith(DataValue&);
	void updateTo(DataValue&);
	void minimize() {	value = min; }
};
int IntDataValue::min = 0;

class DoubleDataValue : public DataValue {
private:
	double value;
public:
	static double min;
	DoubleDataValue(double value): value(value) {}

	void* objectReference() { return this; }
	int getValue() { return value; }

	int compareWith(DataValue&);
	void updateTo(DataValue&);
	void minimize() {	value = min; }
};
double DoubleDataValue::min = 0.0;

/*Using Datavalue pointer is essential*/
/*However it does have a use*/
/*When a copy of an instance is added to an object,intead of adding the original instance, the sructure of the instance copied to the object can't be modified but the value can still be edited*/
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

/*Copy of an instance will be saved instead of original instance and that too as a private member of the DataObject, ensuring data security*/
typedef vector<DataInstance> instanceSet;

class DataObject {
private:
	int id, objectWeight;
	instanceSet instances;
	DataInstance Umin, Umax;
public:
	static int totalObjects, instancesAdded;
	static DataObject Origin;
	DataObject(): id(++totalObjects), Umin(*this), Umax(*this), objectWeight(0) {}

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

/*Using a pointer so that if an object is edited, dont have to resave the object agian in the vector*/
typedef vector<DataObject*> objectSet;

int DataObject::totalObjects = 0;
DataObject DataObject::Origin;
/*Following 2 quantities need not be same, I could have created many instances but added nothing. totalInstances needed to assign instance id, while instances added is needed to define origin based on the structure of the first added instance other than itself*/
int DataObject::instancesAdded = 0;
int DataInstance::totalInstances = 0;
DataInstance DataInstance::Origin(DataObject::Origin);

bool isDominated(DataInstance& u, instanceSet& set) {
	instanceSet::iterator itr = set.begin();
	while(itr!=set.end()) {
		if(u.isDominatedBy(*itr++));
			return true;
	}
	return false;
}

#endif