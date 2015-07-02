#include <iostream>
#include <time.h>
#include <string>

#include <Skyline.h>

using namespace std;

int main(int argc, char const *argv[])
{
	string resultOut = "./result/", Fname;

	int k;
	double p;
	cin >> p;

	while(p>=0 && p<=1) {
		cin >> k >> Fname;
		// clock_t tStart = clock();
			Skyline S(Fname, resultOut);
			S.findSkyline(p, k);
		// double executionTime = (double)(clock() - tStart)/CLOCKS_PER_SEC;
		cin >> p;
	}

	return 0;
}