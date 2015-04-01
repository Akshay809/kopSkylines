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
	- object is an array of instance
	- instance is a dictionary
	- Key is always a string
	- Only standard(int, double, string) type values allowed currently, if required to add a different type of value, say a hexadecimal type value, will have to update the Json parser from the one that is being currently used
	- Last key-value pair needs to be "Weight-Value", so even if Weight attribute is needed to be specified somewhere in middle, it is allowed
	- If keys for an instance are not unique, the value for the last occurence will get saved in the DataStore
*/

#ifndef JSON_ADAPTER_H
#define JSON_ADAPTER_H

#include <DataAdapter.h>
#include "3rdParty/rapidjson/document.h"

using namespace std;
using namespace rapidjson;

class JSONReader : public FileReader {
private:
	Document document;
	const Value& DataObjects, DataInstances;
	Value::ConstValueIterator NextObjectToRead;
public:
	JSONReader(const string inputFile) : FileReader(inputFile) {
		readFileToString();
		document.Parse(c_file);	//Creates a DOM/SAX model of given file
		validateData();
		initAttributeList();
	}
	void validateData();
	void initAttributeList();
	bool hasNextObject();
	void readNextObject(DataObject&);
	void createDummyInstance(DataInstance&);
	// ~JSONReader();
};

class JSONWriter : public FileWriter {
public:
	JSONWriter(const string outputFile) : FileWriter(outputFile) {}
	void writeNextLine(DataInstance& instance);
	// ~JSONWriter();
};


void JSONReader::validateData() {

		if(!document.IsObject()) throw InvalidDataException("Express top most element as an object");
		if(!document.HasMember("objects")) throw InvalidDataException("\"objects\", key value pair expected");

	DataObjects = document["objects"];

		if(!DataObjects.IsArray()) throw InvalidDataException("\"objects\": array value expected");

	NextObjectToRead = DataObjects.Begin();

	for (Value::ConstValueIterator object = DataObjects.Begin(); object != DataObjects.End(); ++object) {

		const Value& DataObject = *object;

			if(!DataObject.IsObject()) throw InvalidDataException("DataObject: object expected");
			if(!DataObject.HasMember("instances")) throw InvalidDataException("\"instances\", key value pair expected");

		DataInstances = DataObject["instances"];

			if(!DataInstances.IsArray()) throw InvalidDataException("\"instances\": array value expected");

		for (Value::ConstValueIterator instance = DataInstances.Begin(); instance != DataInstances.End(); ++instance) {

			const Value& DataInstance = *instance;
			string attrName;

				if(DataInstance.IsObject())	throw InvalidDataException("DataInstance: object expected");

			Value::ConstMemberIterator itr;
				for (itr = DataInstance.MemberBegin(); itr+1 != DataInstance.MemberEnd(); ++itr) {

					attrName = itr->name.GetString();
					AttributeList.insert(attrName);

						if(!itr->value.IsDouble() && !itr->value.IsInt() && !itr->value.IsString())	throw InvalidDataException("Int/Double/String values expected");
				}
				if(attrName!="Weight" && !itr->value.IsInt()) throw InvalidDataException("Weight-Value expected as last pair");
		}
	}
}

void JSONReader::initAttributeList() {
	// Initialised in validation function itself
}

bool JSONReader::hasNextObject() {
	return NextObjectToRead!=DataObjects.End();
}

void JSONReader::createDummyInstance(DataInstance& instance) {
	DataMap& dataStore = instance.getDataStore();
	for (vector<string>::iterator itr = AttributeList.begin(); itr != AttributeList.end(); ++itr)	{
		dataStore[*itr] = NULL;
	}
}

void JSONReader::readNextObject(DataObject& object) {

	const Value& currentObject = *NextObjectToRead;
	const Value& instances = currentObject["instances"];

	for (Value::ConstValueIterator instanceItr = instances.Begin(); instanceItr != instances.End(); ++instanceItr) {

		DataInstance newInstance(object);
		const Value& instance = *instanceItr;

		createDummyInstance(newInstance);
		DataMap& dataStore = newInstance.getDataStore();

		for (Value::ConstMemberIterator itr = instance.MemberBegin(); itr+1 != instance.MemberEnd(); ++itr) {
			string attr = itr->name.GetString();
				if(itr->value.IsInt()) {
					DataValue * val = new IntDataValue(itr->value.GetInt());
				} else if(itr->value.IsDouble()) {
					DataValue * val = new DoubleDataValue(itr->value.GetDouble());
				} else //if(itr->value.IsString()) { 
					DataValue * val = new StringDataValue(itr->value.GetString());
				}
			dataStore[attr] = val;
		}
		newInstance.setWeight(itr->value.GetInt());
		object.addInstance(newInstance);
	}
	NextObjectToRead++;
}


#endif