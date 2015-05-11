#ifndef DATA_READER_H
#define DATA_READER_H

#include <Data.h>
#include <JSONAdapter.h>
#include <XMLAdapter.h>
#include <string>

typedef vector<DataObject*> objectSet;

void readJSONData(const string inputFile, objectSet& data) {
	JSONReader reader(inputFile);
	importData(reader, data);
}

void readXMLData(const string inputFile, objectSet& data) {}

void readData(const string inputFile, objectSet& data) {
	/*TODO: Identify the type of file and call appropriate reader function*/
		return readJSONData(inputFile, data);
}

#endif