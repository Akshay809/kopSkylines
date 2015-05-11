/*Don't use erase*/
#include <iostream>
#include <string>
#include <Skyline.h>

using namespace std;

int main(int argc, char const *argv[])
{
	if(argc!=2) return -1;
	string Fname = argv[1];

	Skyline S(Fname);
	S.findSkyline(0.26);

	return 0;
}