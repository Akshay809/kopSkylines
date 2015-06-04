/*Don't use erase*/
#include <iostream>
#include <string>
#include <Skyline.h>
#include <time.h>
#include <cstring>

using namespace std;

int main(int argc, char const *argv[])
{
	// if(argc!=2) return -1;
	// string Fname = argv[1];

	string resultFName = "./../results";
	ofstream outfile(resultFName.c_str());

	double p;
	int k;
	cin >> p >> k;
	while(p>=0 && p<=1) {
		string filesname = "./../FindSkyline.config";
		ifstream infile(filesname);
		string Fname;
		while(getline(infile, Fname)) {
			clock_t tStart = clock();
				Skyline S(Fname);
				S.findSkyline(p, k);
			double executionTime = (double)(clock() - tStart)/CLOCKS_PER_SEC;
			int SkylineSetSize = S.getSkyline().size();
			outfile << Fname << " " << p << " " << SkylineSetSize << " " << executionTime << endl;
		}

		infile.close();
		cin >> p >> k;
	}

	outfile.close();
	return 0;
}