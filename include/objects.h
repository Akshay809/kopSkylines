#ifndef OBJECTS_H
#define OBJECTS_H

	#include <vector>
	#include <map>
	#include <string>
	using namespace std;

	class IntDataValue : public DataValue
	{
	private:
	public:
		IntDataValue(int val): value(val);

		void compareWith(DataValue &I) {
			if(I.value < this->value) return -1;
			else if(I.value == this->value) return 0;
			return 1;
		}

		void updateTo(DataValue &I) {
			this->value = I.value;
		}

		int value;
	};

	class DoubleDataValue : public DataValue
	{
	private:
	public:
		DoubleDataValue(double val): value(val);

		void compareWith(DataValue &I) {
			if(this->value < I.value) return -1;
			else if(I.value == this->value) return 0;
			return 1;
		}

		void updateTo(DataValue &I) {
			this->value = I.value;
		}

		double value;
	};

	class DataValue {
	public:
	 	DataValue();
		// CompareWith(I): return -1 if lesser, 0 if equal, 1 if greater
		virtual void compareWith(DataValue &I) = 0;
		// UpdateTo(I): updates value of current object to match I
		virtual void updateTo(DataValue &I) = 0;

		void updateIfLargerThan(DataValue &I) {
			/*Implement : check for typeId matching*/
			if(this->compareWith(I)==1)
				this->updateTo(I);
		}

	 	~DataValue();	 	
	};

	typedef map<string, DataValue> DataMap;
	typedef map<string, DataValue>::iterator DataIterator;

	class DataInstance {
	private:
		int weight;
		DataMap dataStore;
	public:
		DataInstance();
		DataInstance(DataInstance &instance);
		~DataInstance();
	};

	class DataObject {
	private:
		vector<DataInstance> instances;
	public:
		DataObject();
		~DataObject();
	};

#endif