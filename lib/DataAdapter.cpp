#include <DataAdapter.h>
#include <iostream>

using namespace std;

void FileReader::readNextLine(vector<string> &fields, char delimiter) {
	string newLine, item;
	if(getline(infile, newLine)) {
		stringstream ss(newLine);
		while(getline(ss, item, delimiter)) {
			if(!item.empty())
				fields.push_back(item);
		}
	}
}

void FileReader::readFileToString() {
	string file, newLine;
	while(getline(infile, newLine)) {
		file = file + newLine + '\n';
	}
	c_file = new char[file.length()+1];
	strcpy(c_file, file.c_str());
}


void importData(FileReader& reader, vector<DataObject*>& data) {
	while(reader.hasNextObject()) {
		DataObject * obj = new DataObject();
		if(reader.readNextObject(*obj))
			data.push_back(obj);
	}
}

void exportData(FileWriter& writer, vector<DataObject*>& skyline) {
	for(vector<DataObject*>::iterator itr = skyline.begin(); itr != skyline.end(); ++itr)
		writer.writeNextObject(**itr);
}
