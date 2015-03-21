#include <vector>
#include <map>
#include <string>

using namespace std;


#ifndef OBJECTS_H
#define OBJECTS_H

	union DataValue {
		string val_s;
		long int val_i;
		float val_f;
		double val_d;
	};

	typedef map<string, DataValue> DataMap;
	typedef map<string, DataValue>::iterator DataIterator;

	class DataInstance {
	private:
		int weight;
		DataMap dataStore;
	public:
		DataInstance();
		~DataInstance();
	};

	class DataObject	{
	private:
		vector<DataInstance> instances;
	public:
		DataObject();
		~DataObject();
	};

#endif