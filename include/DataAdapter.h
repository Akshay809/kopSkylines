#ifndef DATA_ADAPTER_H
#define DATA_ADAPTER_H

#include <Data.h>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std;

class FileReader {
protected:
	string inputFile;
	ifstream infile;
public:

	FileReader(const string inputFile) : infile(inputFile.c_str()), inputFile(inputFile) {}

	virtual	void validateAndInit() = 0;

			bool hasNextLine() {}
			void readNextLine(vector<string>&, char);

	virtual bool hasNextObject() { return hasNextLine(); }
	virtual bool readNextObject(DataObject& O) {}

	virtual void createDummyInstance(DataInstance& I) {}

	~FileReader() {
		infile.close();
	}
};

class FileWriter {
protected:
	string outputFile;
	ofstream outfile;
public:
	FileWriter(const string outputFile) : outfile(outputFile.c_str()), outputFile(outputFile) {}

	virtual void writeNextLine(DataInstance& I) {}
	virtual void writeNextObject(DataObject& O) {}

	~FileWriter() {
		outfile.close();
	}
};

void importData(FileReader&, vector<DataObject*>&);

void exportData(FileWriter&, vector<DataObject*>&);

#endif