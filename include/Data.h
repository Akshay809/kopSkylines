#ifndef DATA_DEFINITION_H
#define DATA_DEFINITION_H

#include <vector>
#include <map>

using namespace std;

class DataObject;
class DataInstance;

typedef vector<double>::iterator DataStoreIterator;


class DataInstance {
private:
	int instanceId;
	DataObject& Object;
public:
	int weight;
	vector<double> dataStore;

	static int totalInstances;
	static DataInstance Origin;

	DataInstance(DataObject& Object): Object(Object), instanceId(++totalInstances), weight(0) {}
	DataInstance(DataObject& Object, int weight): Object(Object), instanceId(++totalInstances), weight(weight) {}

	int getObjectID();
	int getInstanceID() { return instanceId; }
	DataObject& getObjectRef() { return Object; }

	/*R1: Lesser value preferred*/
	bool isDominatedBy(DataInstance&);
	bool isDominatedBy(vector<DataInstance>&);

	DataInstance& operator= (const DataInstance&);
	bool operator==(const DataInstance& I) { return dataStore==I.dataStore; }

	void minimizeWRT(DataInstance&);
	void maximizeWRT(DataInstance&);
	void minimizeDS();

	void printDataInstance();
};

class DataObject {
private:
	int id, objectWeight;
	vector<DataInstance> instances;
	/*A1: assuming values are in the same order and no missing values*/
public:
	DataInstance Umin, Umax;

	static int totalObjects;
	static DataObject Origin;

	DataObject(): id(++totalObjects), Umin(*this), Umax(*this), objectWeight(0) {}

	int getID() { return id; }
	int getObjectWeight() { return objectWeight; }

	void updateCorners();
	void updateMinimumCorner();
	void updateMaximumCorner();

	void addInstance(DataInstance&); /*Check for A1*/
	void removeInstance(int);
	void removeInstance(DataInstance&);

	void printDataObject();
};

#endif