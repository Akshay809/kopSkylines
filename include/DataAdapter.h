#ifndef DATA_ADAPTER_H
#define DATA_ADAPTER_H

#include <Data.h>
using namespace std;

class FileReader {
protected:
	string inputFile;
public:
	FileReader(const string &inputFile) : inputFile(inputFile) {}
	virtual bool hasNextLine() = 0;
	virtual void readNextLine(DataInstance &instance) = 0;
	~FileReader();
};

class FileWriter {
protected:
	string outputFile;
public:
	FileWriter(const string &outputFile) : outputFile(outputFile) {}
	virtual void writeNextLine(DataInstance &instance) = 0;
	~FileWriter();
};

void importData(FileReader &reader, vector<DataObject> &data);

void exportData(FileWriter &writer, const vector<DataObject> &data);

#endif