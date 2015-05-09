#ifndef SKYLINE_H
#define SKYLINE_H

#include <Data.h>
#include <DataReader.h>
#include <DataWriter.h>
#include <Helper.h>

#include <sstream>
#include <string>

using namespace std;

/* full-skyline Algorithms*/
objectSet& full_(objectSet&); /*naive-version*/

/* k-Skyline Algorithms */
objectSet& k_(objectSet&, int); /*naive-version*/

/* p-Skyline Algorithms */
objectSet& p_(objectSet&, double); /*naive-version*/
objectSet& p_TopDown(objectSet&, double);
objectSet& p_BottomUp(objectSet&, double);

/* kop-Skyline Algorithms */
objectSet& kop_(objectSet&, double, int); /*naive-version*/


class Skyline {
private:
	objectSet& Skyline;
public:
	const string Fname;
	const objectSet &data;

	Skyline(string file_name): Fname(file_name), data(readData(file_name)) {}

	/*TODO: Add preference relation specification*/

	/*Certain Data <=> Single instance instanceSet for every object*/
	/*Locationally Uncertain Data <=> Multi-instance instanceSet for atleast one object*/
	/*Existentially Uncertain Data <=> TODO: need to modfiy current weight allocation schema*/

	void findSkyline();					//full-Skyline
	void findSkyline(int k);			//k-Skyline
	void findSkyline(double p);			//p-Skyline
	void findSkyline(double p, int k);	//kop-Skyline

	const objectSet& getSkyline() const { return Skyline; }

	~Skyline() {
	}
};

void Skyline::findSkyline() {}

void Skyline::findSkyline(int k) {}

void Skyline::findSkyline(double p) {
	string outputName = fileBaseName(Fname);
	outputName = "./../data/skyline_data/result_" + outputName;
	Skyline = p_BottomUp(data, p);
		cout << Skyline.size() << endl;
	writeData(outputName, getSkyline());
}

void Skyline::findSkyline(double p, int k) {}

#endif