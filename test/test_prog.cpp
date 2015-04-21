#include <iostream>
#include <string>
#include <DataReader.h>

using namespace std;

int main(int argc, char const *argv[])
{
	if(argc!=2) return -1;
	string Fname = argv[1];
	vector<DataObject*>& DataObjects = readData(Fname);
	cout << "Number of objects read: " << DataObjects.size() << endl;
	for (vector<DataObject*>::iterator itr = DataObjects.begin(); itr != DataObjects.end(); ++itr) {
		(*itr)->printDataObject();
	}
	return 0;
}