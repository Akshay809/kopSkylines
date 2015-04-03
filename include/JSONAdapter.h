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
	- keys for an instance required to be unique
	- One attribute needs to have values of same type, across all instances
	- Last key-value pair needs to be "Weight"-Value, also only "Weight" attribute is allowed to be specified anywhere in middle also. Value must be int
	- missing values are allowed
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
	Value DataObjects;
	Value::ConstValueIterator NextObjectToRead;
public:
	JSONReader(const string inputFile) : FileReader(inputFile) {
		readFileToString();
		document.Parse(c_file);	//Creates a DOM/SAX model of given file
		validateDataAndInitAttrList();
	}
	void validateDataAndInitAttrList();
	bool hasNextObject();
	void readNextObject(DataObject&);
	void createDummyInstance(DataInstance&);
	// ~JSONReader();
};

// map<string, int> AttributeList; 

class JSONWriter : public FileWriter {
public:
	JSONWriter(const string outputFile) : FileWriter(outputFile) {}
	void writeNextLine(DataInstance& instance);
	// ~JSONWriter();
};

void JSONReader::validateDataAndInitAttrList() {

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

				if(!DataInstance.IsObject())	throw InvalidDataException("DataInstance: object expected");

			Value::ConstMemberIterator itr;
				for (itr = DataInstance.MemberBegin(); itr+1 != DataInstance.MemberEnd(); ++itr) {

					attrName = itr->name.GetString();
					int CurrentValueType;
						if(itr->value.IsDouble())
							CurrentValueType = DoubleDataValue::type;
						else if(itr->value.IsInt())
							CurrentValueType = IntDataValue::type;
						else if(itr->value.IsString())
							CurrentValueType = StringDataValue::type;
						else throw InvalidDataException("Int/Double/String values expected");
					if(AttributeList.find(attrName)==AttributeList.end())
						AttributeList[attrName] = CurrentValueType;
					else if(AttributeList[attrName] != CurrentValueType)
						throw InvalidDataException("Conflicting DataTypes for values of same attribute");

				}
				if(attrName!="Weight" && !itr->value.IsInt()) throw InvalidDataException("Weight-Value expected as last pair");
		}
	}
}


bool JSONReader::hasNextObject() {
	return NextObjectToRead!=DataObjects.End();
}

void JSONReader::createDummyInstance(DataInstance& instance) {
	DataMap& dataStore = instance.getDataStore();
	for (map<string, int>::iterator itr = AttributeList.begin(); itr != AttributeList.end(); ++itr)	{
		dataStore[itr->first] = NULL;
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

		Value::ConstMemberIterator itr;
		for (itr = instance.MemberBegin(); itr+1 != instance.MemberEnd(); ++itr) {
			string attr = itr->name.GetString();
			DataValue * val;
			if(itr->value.IsInt()) {
				val = new IntDataValue(itr->value.GetInt());
			} else if(itr->value.IsDouble()) {
				val = new DoubleDataValue(itr->value.GetDouble());
			} else /*if(itr->value.IsString())*/ { 
				val = new StringDataValue(itr->value.GetString());
			}
			dataStore[attr] = val;
		}
		newInstance.setWeight(itr->value.GetInt());
		object.addInstance(newInstance);
	}
	NextObjectToRead++;
}


#endif