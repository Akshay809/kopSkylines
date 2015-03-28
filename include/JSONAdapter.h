#ifndef JSON_ADAPTER_H
#define JSON_ADAPTER_H

#include <DataAdapter.h>
using namespace std;

class JSONReader : public FileReader {
public:
	JSONReader(const string &inputFile) : FileReader(inputFile) {}
	bool hasNextLine();
	void readNextLine(DataInstance &instance);
	~JSONReader();
};

class JSONWriter : public FileWriter {
public:
	JSONWriter(const string &outputFile) : FileWriter(outputFile) {}
	void writeNextLine(DataInstance &instance);
	~JSONWriter();
};

#endif