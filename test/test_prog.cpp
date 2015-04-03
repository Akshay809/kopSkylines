#include <iostream>
#include <string>
#include <JSONReader.h>

using namespace std;

int main(int argc, char const *argv[])
{
	if(argc!=2) return -1;
	string Fname = argv[1];
	objectSet& DataObjects = readData(Fname);
	cout << "Number of objects read: " << DataObjects.size() << endl;
	for (objectSet::iterator itr = DataObjects.begin(); itr != DataObjects.end(); ++itr) {
		cout << " Object Id: " << (*itr)->getID() << endl;
		cout << " Object Weight: " << (*itr)->getObjectWeight() << endl;		
	}
	return 0;
}