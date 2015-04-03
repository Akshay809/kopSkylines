#include <Data.h>
#include <JSONAdapter.h>
#include <string>

objectSet& readData(const string inputFile) {
	objectSet * data = new objectSet;
	JSONReader reader(inputFile);
	importData(reader, *data);
	return *data;
}