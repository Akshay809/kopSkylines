/*
	**************IMPORTANT**************
	********subset-JSON Data Reader******
	JSON file to be read needs to be structured in the following manner:
	{
		objects: [
			{},
			{
				instances: [
					{},
					{
						"Key1": Value1
						"Key2": Value2
						.....
						"Weight": Value
					},
					{},
					.....
				]
			},
			{},
			.....
		]
	}
	- data is an array of objects
	- object is an array of instances
	- instance is a dictionary
	- Key is always a string
	- Only standard(int, double, string(not now)) type values allowed currently, if required to add a different type of value, say a hexadecimal type value, will have to update the Json parser from the one that is being currently used
	- keys for an instance required to be unique
	- One attribute needs to have values of same type, across all instances
	- Last key-value pair needs to be "Weight"-Value, also only "Weight" attribute is allowed to be specified anywhere in middle also. Value must be int
	- missing values are [not] allowed
*/

#ifndef JSON_ADAPTER_H
#define JSON_ADAPTER_H

#include <map>

#include <DataAdapter.h>
#include "3rdParty/rapidjson/document.h"

using namespace std;
using namespace rapidjson;

class JSONReader : public FileReader {
private:
	Document document;
	Value DataObjects;
	Value::ConstValueIterator NextObjectToRead;
	map<string, int> AttributeIndex;
	char * c_file;
public:
	int totalAttributes;
	JSONReader(const string inputFile) : FileReader(inputFile) {
		readFileToString();
		document.Parse(c_file);	//Creates a DOM/SAX model of given file
		totalAttributes = 0;
		validateAndInit();
	}

	void readFileToString();
	void validateAndInit();
	bool hasNextObject();
	bool readNextObject(DataObject&);

	~JSONReader() {
		delete [] c_file;
	}
};

// AttributeIndex

class JSONWriter : public FileWriter {
public:

	JSONWriter(const string outputFile) : FileWriter(outputFile) {
		outfile << "{ \"objects\": [" << endl;
	}

	void writeKeyValuePair(const string key, const double value) {
		outfile << "\t\t\t\"" << key << "\": " << value << "," << endl;
	}

	void writeNextInstance(const DataInstance& I) {
		outfile << "\t\t{" << endl;
		const vector<string>& nameStore = I.getNameStore();
		const vector<double>& dataStore = I.getDataStore();
		for(int i=0; i<dataStore.size(); ++i) {
			writeKeyValuePair(nameStore[i], dataStore[i]);
		}
		outfile << "\t\t}," << endl;
	}

	void writeNextObject(const DataObject& O) {
		outfile << "\t{ \"instances\": [" << endl;
		const vector<DataInstance>& instances = O.getDataInstances();
		for(auto itr = instances.cbegin(); itr != instances.cend(); ++itr) {
			writeNextInstance(*itr);
		}
		outfile << "\t] }," << endl;
	}

	~JSONWriter() {
		outfile << "] }\n";
	}
};

void JSONReader::readFileToString() {
	string file, newLine;
	while(getline(infile, newLine)) {
		file = file + newLine + '\n';
	}
	c_file = new char[file.length()+1];
	strcpy(c_file, file.c_str());
}

void JSONReader::validateAndInit() {

	if(!document.IsObject()) throw InvalidDataException("Express top most element as an object");
	if(!document.HasMember("objects")) throw InvalidDataException("\"objects\", key value pair expected");

		DataObjects = document["objects"];

	if(!DataObjects.IsArray()) throw InvalidDataException("\"objects\": array value expected");

		NextObjectToRead = DataObjects.Begin();

	for (Value::ConstValueIterator object = DataObjects.Begin(); object != DataObjects.End(); ++object) {

		const Value& DataObject = *object;

			if(!DataObject.IsObject()) throw InvalidDataException("DataObject: object expected");
			if(!DataObject.HasMember("instances")) throw InvalidDataException("\"instances\", key value pair expected");

		const Value& DataInstances = DataObject["instances"];

			if(!DataInstances.IsArray()) throw InvalidDataException("\"instances\": array value expected");

		for (Value::ConstValueIterator instance = DataInstances.Begin(); instance != DataInstances.End(); ++instance) {

			const Value& DataInstance = *instance;
			string attrName;

				if(!DataInstance.IsObject()) throw InvalidDataException("DataInstance: object expected");

			Value::ConstMemberIterator itr;
				for (itr = DataInstance.MemberBegin(); itr+1 != DataInstance.MemberEnd(); ++itr) {

					attrName = itr->name.GetString();

						if(!itr->value.IsNumber()) throw InvalidDataException("Int/Double/String(not now) values expected");

					if(AttributeIndex.find(attrName)==AttributeIndex.end())
						AttributeIndex[attrName] = totalAttributes++;
					/*else Check for type, and if not same as previously recorded type
						throw InvalidDataException("Conflicting DataTypes for values of same attribute");
						*/
				}
				if(attrName!="Weight" && !itr->value.IsInt()) throw InvalidDataException("Weight-Value expected as last pair");
		}
	}
}


bool JSONReader::hasNextObject() {
	return NextObjectToRead!=DataObjects.End();
}

bool JSONReader::readNextObject(DataObject& object) {

	const Value& currentObject = *NextObjectToRead;
	const Value& instances = currentObject["instances"];

	for (Value::ConstValueIterator instanceItr = instances.Begin(); instanceItr != instances.End(); ++instanceItr) {

		DataInstance newInstance(object);
		const Value& instance = *instanceItr;

		vector<string> newNameStore;
		vector<double> newDataStore;

		newDataStore.resize(AttributeIndex.size());
		newNameStore.resize(AttributeIndex.size());

		int insertCount = 0;
		Value::ConstMemberIterator itr;
		for (itr = instance.MemberBegin(); itr+1 != instance.MemberEnd(); ++itr) {

			string attr = itr->name.GetString();
			int index = AttributeIndex[attr];

			newNameStore[index] = attr;
			if(itr->value.IsInt())
				newDataStore[index] = (double)(itr->value.GetInt());
			else
				newDataStore[index] = itr->value.GetDouble();

			insertCount++;
		}
		if(insertCount<AttributeIndex.size()) throw InvalidDataException("Missing values");

		newInstance.setWeight(itr->value.GetInt());
		newInstance.updateStoreTo(newNameStore, newDataStore);
		object.addInstance(newInstance);
	}
	NextObjectToRead++;

	return true;
}


#endif