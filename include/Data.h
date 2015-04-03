#ifndef DATA_DEFINITION_H
#define DATA_DEFINITION_H

#include <vector>
#include <map>
#include <string>

using namespace std;

class DataValue {
private:
	int type;
public:
 	DataValue(int type): type(type) {}
	// CompareWith(I): return -1 if lesser, 0 if equal, 1 if greater, 2 if can't be compared
	// UpdateTo(I): updates value of current object to match I
	virtual int compareWith(DataValue&) = 0;
	virtual void updateTo(DataValue&) = 0;
	virtual void* objectReference() = 0;
	virtual void minimize() = 0;

 	int getType() { return type; }
	void updateIfLargerThan(DataValue&);
	void updateIfSmallerThan(DataValue&);
	virtual DataValue& createCopy() = 0;
	virtual void printDataValue() = 0;
};

class IntDataValue : public DataValue {
private:
	int value;
public:
	static int min;
	static const int type;
	IntDataValue(int value): value(value), DataValue(IntDataValue::type) {}

	void* objectReference() { return this; }
	int getValue() { return value; }

	int compareWith(DataValue&);
	void updateTo(DataValue&);
	void minimize() {	value = min; }
	DataValue& createCopy();
	void printDataValue();
};
// int IntDataValue::min = 0;
// const int IntDataValue::type = 1; //Should be unique

class DoubleDataValue : public DataValue {
private:
	double value;
public:
	static double min;
	static const int type;
	DoubleDataValue(double value): value(value), DataValue(DoubleDataValue::type) {}

	void* objectReference() { return this; }
	double getValue() { return value; }

	int compareWith(DataValue&);
	void updateTo(DataValue&);
	void minimize() {	value = min; }
	DataValue& createCopy();
	void printDataValue();
};
// double DoubleDataValue::min = 0.0;
// const int DoubleDataValue::type = 2; //Should be unique

class StringDataValue : public DataValue {
private:
	string value;
public:
	static string min;
	static const int type;
	StringDataValue(string value): value(value), DataValue(StringDataValue::type) {}

	void* objectReference() { return this; }
	string getValue() { return value; }

	int compareWith(DataValue&);
	void updateTo(DataValue&);
	void minimize() {	value = min; }
	DataValue& createCopy();
	void printDataValue();
};
// string StringDataValue::min = "";
// const int StringDataValue::type = 3; //Should be unique


class DataObject;
class DataInstance;


typedef map<string, DataValue*> DataMap;
typedef map<string, DataValue*>::iterator DataMapIterator;
/*Copy of an instance will be saved instead of original instance and that too as a private member of the DataObject, ensuring data security*/
typedef vector<DataInstance> instanceSet;

class DataInstance {
private:
	int instanceId, weight;
	DataObject& Object;
	DataMap dataStore;
	// DataInstance(const DataInstance&);
public:
	static int totalInstances;
	static DataInstance Origin;
	DataInstance(DataObject& Object): Object(Object), instanceId(++totalInstances), weight(0) {}

	DataInstance& operator= (const DataInstance&);

	void setWeight(int wt) { weight = wt; }
	int getObjectID();// { return Object.getID(); }
	int getWeight() { return weight; }
	int getInstanceID() { return instanceId; }

	DataObject& getObjectRef() { return Object; }
	const DataMap& getDataStore() const { return dataStore; }
	DataMap& getDataStore() { return dataStore; }

	bool isMinimumCornerOfU();
	bool isDominatedBy(DataInstance&);
	bool isDominatedBy(instanceSet&);

	void minimizeWRT(DataInstance&);
	void maximizeWRT(DataInstance&);
	void minimizeDS();

	void printDataInstance();
};

class DataObject {
private:
	int id, objectWeight;
	instanceSet instances;
	DataInstance Umin, Umax;
public:
	static int totalObjects;//, instancesAdded;
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

	void printDataObject();
};


/*Using a pointer so that if an object is edited, dont have to resave the object agian in the vector*/
typedef vector<DataObject*> objectSet;

// int DataObject::totalObjects = 0;
// DataObject DataObject::Origin;
/*instancesAdded and totalInstances need not be same, I could have created many instances but added nothing. totalInstances needed to assign instance id, while instances added is needed to define origin based on the structure of the first added instance other than itself*/

// int DataObject::instancesAdded = 0;

// int DataInstance::totalInstances = 0;
// DataInstance DataInstance::Origin(DataObject::Origin);

#endif