#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <cstring>

using namespace std;

void generateData(int objects, int attributes, string fileName) {
	/*m: number of objects; n: number of attributes*/
	ofstream outfile(fileName.c_str());

	outfile << "{ \"objects\": [" << endl;

	srand(time(NULL));
	for(int i=0; i<objects; ++i) {

		outfile << "\t{ \"instances\": [" << endl;
		int instances = rand()%5 + 1;

		for(int j=0; j<instances; ++j) {

			outfile << "\t\t{" << endl;

			for(int k=0; k<attributes; k++) {
				stringstream ss; ss << (k+1);
				string key = "attribute_" + ss.str();
				double value = rand()%10+1;
				outfile << "\t\t\t\"" << key << "\": " << value << "," << endl;
			}

			int weight = rand()%50;
			outfile << "\t\t\t\"Weight\": " << weight << endl;
			
			if(j==instances-1)
				outfile << "\t\t}" << endl;
			else
				outfile << "\t\t}," << endl;
		}

		if(i==objects-1)
			outfile << "\t] }" << endl;
		else
			outfile << "\t] }," << endl;
	}
	outfile << "] }\n";
	outfile.close();
}

int main(int argc, char const *argv[])
{
	if(argc!=2) return -1;
	string fileName = argv[1];

	generateData(10000, 100, fileName);
	return 0;
}