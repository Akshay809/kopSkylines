#ifndef DATA_DEFINITION_H
#define DATA_DEFINITION_H

#include <vector>
#include <map>
#include <string>

using namespace std;

class DataObject;
class DataInstance;

typedef vector<double>::iterator DataStoreIterator;

class DataInstance {
private:
public:
	const int instanceId;		
	const DataObject& Object;		/*Referred object*/
	/*Only initialized once*/

	const int weight;					/*For occurence probability*/
	const vector<string> nameStore;		/*Keys*/
	const vector<double> dataStore;		/*Values*/
	/*A1: assuming values are in the same order and no missing values*/

	static int totalInstances;		/*To assign new instance ID*/
	static DataInstance Origin;		/*Default(1st) and best instance*/


	/*General purpose constructors*/
	DataInstance(DataObject& Object): Object(Object), instanceId(++totalInstances), weight(0) {}
	DataInstance(DataObject& Object, int weight, vector<string> nameStore, vector<double> dataStore): Object(Object), instanceId(++totalInstances), weight(weight), nameStore(nameStore), dataStore(dataStore) {}

	/*Overridden copy constructor and assignment operator*/
	DataInstance(const DataInstance&);
	DataInstance& operator= (const DataInstance&);

	int getObjectID() const;
	const DataObject* getRefObjAdd() const { return &Object; }

	double getKey() const;
	double getProbability() const;

	/*R1: Lesser value preferred*/
	bool isDominatedBy(const DataInstance&) const;
	bool isDominatedBy(const vector<DataInstance>&) const;
	bool operator==(const DataInstance& I) const { return dataStore==I.dataStore; }

	/*Non-constant functions*/
		void minimizeWRT(const DataInstance&);
		void maximizeWRT(const DataInstance&);
		void minimizeDS();

	void printDataInstance() const;
};

class DataObject {
private:
	vector<DataInstance> instances;		/*instances of this DataObject*/
	DataInstance Umin, Umax;			/*Best and worst possible form*/
	int objectWeight;					/*To compute occurence probability*/
public:
	const int id;

	static int totalObjects;
	static DataObject Origin;

	DataObject(): id(++totalObjects), Umin(*this), Umax(*this), objectWeight(0) {}

	/*Overridden copy constructor and assignment operator*/
	DataObject(const DataObject&);
	DataObject& operator= (const DataObject&);

	int getObjectWeight() const { return objectWeight; }
	const vector<DataInstance>& getDataInstances() const { return instances; }

	/*Non-constant functions*/
		void updateCorners();
		void updateMinimumCorner();
		void updateMaximumCorner();

		/*IMP: Use heap and not stack memory for creating new instance and use desctructors carefully*/
		void addInstance(DataInstance&); /*Check for A1*/
		void removeInstance(int);
		void removeInstance(DataInstance&);

	void printDataObject() const;
};

struct Rectangle{
	DataInstance &lowerEnd, &upperEnd;
	Rectangle(DataInstance& lowerEnd, DataInstance& upperEnd): lowerEnd(lowerEnd), upperEnd(upperEnd) {}
};

typedef vector<DataObject*> objectSet;

#endif