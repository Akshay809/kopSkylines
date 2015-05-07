#ifndef DATA_WRITER_H
#define DATA_WRITER_H

#include <Data.h>
#include <JSONAdapter.h>
#include <XMLAdapter.h>
#include <string>

typedef vector<DataObject*> objectSet;

void writeJSONData(const string outputFile, objectSet& Skyline) {
	JSONWriter writer(outputFile);
	exportData(writer, Skyline);
	return;
}

void writeData(const string outputFile, objectSet& Skyline) {
	/*TODO: Identify the type of file by its name and call appropriate writer function*/
		return writeJSONData(outputFile, Skyline);
}

#endif