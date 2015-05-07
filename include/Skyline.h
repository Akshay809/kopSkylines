#ifndef SKYLINE_H
#define SKYLINE_H

#include <Data.h>
#include <DataReader.h>
#include <DataWriter.h>

using namespace std;

/* full-skyline Algorithms*/
objectSet& full_(objectSet&);

/* k-Skyline Algorithms */
objectSet& k_(objectSet&, int);

/* p-Skyline Algorithms */
objectSet& p_TopDown(objectSet&, double);
objectSet& p_BottomUp(objectSet&, double);

/* kop-Skyline Algorithms */
objectSet& kop_(objectSet&, double, int);


class Skyline {
private:
public:
	string Fname;
	objectSet& data;
	Skyline(string file_name): Fname(file_name), data(readData(file_name)) {}

	/*TODO: Add preference relation specification*/

	/*Certain Data <=> Single instance instanceSet for every object*/
	/*Locationally Uncertain Data <=> Multi-instance instanceSet for atleast one object*/
	/*Existentially Uncertain Data <=> TODO: need to modfiy current weight allocation schema*/

	void findSkyline();					//full-Skyline
	void findSkyline(int k);			//k-Skyline
	void findSkyline(double p);			//p-Skyline
	void findSkyline(double p, int k);	//kop-Skyline

	~Skyline() {
	}
};

void Skyline::findSkyline() {}

void Skyline::findSkyline(int k) {}

void Skyline::findSkyline(double p) {
	string outFileName = "./../data/skyline_data/result.json";
	objectSet& Skyline = p_BottomUp(data, p);
	cout << Skyline.size() << endl;
	writeData(outFileName, Skyline);
}

void Skyline::findSkyline(double p, int k) {}

#endif