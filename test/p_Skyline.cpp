#include <iostream>
#include <time.h>
#include <string>

#include <Skyline.h>

using namespace std;

int main(int argc, char const *argv[])
{
	string resultOut = "./result/", Fname;

	double p;
	cin >> p >> Fname;

	while(p>=0 && p<=1) {
		// clock_t tStart = clock();
			Skyline S(Fname, resultOut);
			S.findSkyline(p);
		// double executionTime = (double)(clock() - tStart)/CLOCKS_PER_SEC;
		cin >> p >> Fname;
	}

	return 0;
}