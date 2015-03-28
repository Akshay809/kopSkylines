#ifndef READ_AND_WRITE_DATA_JSON_H
#define READ_AND_WRITE_DATA_JSON_H

	#include <read_and_write_data.h>
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