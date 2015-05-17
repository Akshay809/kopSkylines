#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <cstring>

using namespace std;

void generateData(int objects, int instancesBound, int attributes, string fileName) {
	/*m: number of objects; n: number of attributes*/
	ofstream outfile(fileName.c_str());

	outfile << "{ \"objects\": [" << endl;

	srand(time(NULL));
	for(int i=0; i<objects; ++i) {

		outfile << "\t{ \"instances\": [" << endl;
		int instances = (rand()%instancesBound) + 1;

		for(int j=0; j<instances; ++j) {

			outfile << "\t\t{" << endl;

			for(int k=0; k<attributes; k++) {
				stringstream ss; ss << (k+1);
				string key = "attribute_" + ss.str();
				double value = rand()%10+1;
				outfile << "\t\t\t\"" << key << "\": " << value << "," << endl;
			}

			int weight = (rand()%21) + 20;
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
	// if(argc!=2) return -1;
	// string fileName = argv[1];

	int oCount, iCount, aCount;
	cin >> oCount >> iCount >> aCount;
	/*while loop should go in a make file*/
	/*read first line, build a data file, execute skyline programs and compare results and execution times*/

	string SkylineConfigFile = "./../FindSkyline.config";
	ofstream out(SkylineConfigFile.c_str());

	while(oCount>0 && iCount>0 && aCount>0) {
		string fileName = "./../data/data_" + to_string(oCount) + "_" + to_string(iCount) + "_" + to_string(aCount) + ".json";
		out << fileName << endl;
		generateData(oCount, iCount, aCount, fileName);
		cin >> oCount >> iCount >> aCount;
	}

	out.close();

	return 0;
}

/* ~10^3 objects, ~10^3 instances/object , ~10 attributes/instance*/
/* ~200 bytes/instance, ~2*10^5 bytes/object, ~2*10^8 bytes/data */
