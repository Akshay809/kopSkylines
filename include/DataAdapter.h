#ifndef DATA_ADAPTER_H
#define DATA_ADAPTER_H

#include <Data.h>
#include <Exceptions.h>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std;

class FileReader {
protected:
	string inputFile;
	ifstream infile;
	map<string, int> AttributeIndex;
	char * c_file;
public:

	FileReader(const string inputFile) : infile(inputFile.c_str()), inputFile(inputFile) {}

	virtual	void validateDataAndInitAttrList() {}

			void readFileToString();
	virtual bool hasNextLine() {}
			void readNextLine(vector<string>&, char);
	virtual bool hasNextObject() { return hasNextLine(); }
	virtual bool readNextObject(DataObject&) {}

	virtual void createDummyInstance(DataInstance&) {}

	~FileReader() {
		infile.close();
	}
};

class FileWriter {
protected:
	string outputFile;
public:
	FileWriter(const string outputFile) : outputFile(outputFile) {}
	virtual void writeNextLine(DataInstance&) = 0;
	// ~FileWriter();
};

void importData(FileReader&, vector<DataObject*>&);

void exportData(FileWriter&, const vector<DataObject*>&);

#endif