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

void importData(FileReader& reader, vector<DataObject*>& data) {
	while(reader.hasNextObject()) {
		DataObject * obj = new DataObject();
		if(reader.readNextObject(*obj))
			data.push_back(obj);
	}
}

void exportData(FileWriter& writer, const vector<DataObject*>& skyline) {
	for(auto itr = skyline.cbegin(); itr != skyline.cend(); ++itr)
		writer.writeNextObject(**itr);
}
