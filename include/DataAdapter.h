#ifndef READ_AND_WRITE_DATA_H
#define READ_AND_WRITE_DATA_H

	#include <objects.h>
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