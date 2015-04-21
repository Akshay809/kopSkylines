#ifndef DATA_READER_H
#define DATA_READER_H

#include <Data.h>
#include <JSONAdapter.h>
#include <XMLAdapter.h>
#include <string>

typedef vector<DataObject*> objectSet;

objectSet& readJSONData(const string inputFile) {
	objectSet * data = new objectSet;
	JSONReader reader(inputFile);
	importData(reader, *data);
	return *data;
}

objectSet& readData(const string inputFile) {
	/*TODO: Identify the type of file and call appropriate reader function*/
		return readJSONData(inputFile);
}

#endif