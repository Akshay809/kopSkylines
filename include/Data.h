#ifndef DATA_DEFINITION_H
#define DATA_DEFINITION_H

#include <vector>
#include <string>

#include <Exceptions.h>
#include <Helper.h>

using namespace std;

class DataObject;
class DataInstance;

// typedef vector<double>::iterator DataStoreIterator;

class DataInstance {
private:
	int weight;					/*For occurence probability*/
	vector<string> nameStore;		/*Keys*/
	vector<double> dataStore;		/*Values*/
	/*Cant be declared const, since Umin and Umax needs updates*/
	/*A1: assuming values are in the same order and no missing values*/
public:
	const int instanceId;		
	const DataObject& Object;		/*Referred object*/
	/*Only initialized once*/

	static int totalInstances;		/*To assign new instance ID*/
	static DataInstance Origin;		/*Default(1st) and best instance*/


	/*General purpose constructors*/
	DataInstance(DataObject& Object): Object(Object), instanceId(++totalInstances), weight(0) {}
	DataInstance(DataObject& Object, int weight): Object(Object), instanceId(++totalInstances), weight(weight) {}

	/*Overridden copy constructor and assignment operator*/
	DataInstance(const DataInstance&);
	DataInstance& operator= (const DataInstance&);

	int getObjectID() const;
	const DataObject* getRefObjAdd() const { return &Object; }
	const vector<string>& getNameStore() const { return nameStore; }
	const vector<double>& getDataStore() const { return dataStore; }

	double getKey() const;
	double getProbability() const;
	int getWeight() const { return weight; }

	/*R1: Lesser value preferred*/
	bool isDominatedBy(const DataInstance&) const;
	bool isDominatedBy(const vector<DataInstance>&) const;
	bool operator==(const DataInstance& I) const { return dataStore==I.dataStore; }

	/*Non-constant functions*/
		void setWeight(int wt) { weight = wt; }
		void updateStoreTo(const vector<string>&, const vector<double>&);
		void addPair(string, double);

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

	/*copy constructor and assignment operator made unavailable*/
	DataObject(const DataObject&);
	DataObject& operator= (const DataObject&);

public:
	const int id;

	static int totalObjects;
	static DataObject Origin;

	DataObject(): id(++totalObjects), Umin(*this), Umax(*this), objectWeight(0) {}

	int getObjectWeight() const { return objectWeight; }
	const vector<DataInstance>& getDataInstances() const { return instances; }

	const DataInstance& getUmin();
	const DataInstance& getUmax();

	const DataInstance& getUmin() const;
	const DataInstance& getUmax() const;

	/*Non-constant functions*/
		void updateCorners();
		void updateMinimumCorner();
		void updateMaximumCorner();

		/*IMP: Use heap and not stack memory for creating new instance and use desctructors carefully*/
		void addInstance(const DataInstance&); /*Check for A1*/
		void removeInstance(int);
		void removeInstance(const DataInstance&);

	void printDataObject() const;
};

struct Rectangle{
	DataInstance &lowerEnd, &upperEnd;
	Rectangle(DataInstance& lowerEnd, DataInstance& upperEnd): lowerEnd(lowerEnd), upperEnd(upperEnd) {}
};

typedef vector<DataObject*> objectSet;

#endif