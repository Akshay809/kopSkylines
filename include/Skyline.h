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
void full_(const objectSet&, vector<const DataObject*>&); /*naive-version*/

/* k-Skyline Algorithms */
void k_(const objectSet&, int, vector<const DataObject*>&); /*naive-version*/

/* p-Skyline Algorithms */
void p_(const objectSet&, double, vector<const DataObject*>&); /*naive-version*/
void p_TopDown(const objectSet&, double, vector<const DataObject*>&);
void p_BottomUp(const objectSet&, double, vector<const DataObject*>&);

/* kop-Skyline Algorithms */
void kop_(const objectSet&, double, int, vector<const DataObject*>&); /*naive-version*/


class Skyline {
private:
	vector<const DataObject*> * SkylineSet;
	objectSet * data;
	/*Purposefully creating these data ojects here, as heap allocation is required and I do not want a memory leak*/
	string outputName;
public:
	const string Fname;

	Skyline(string file_name): Fname(file_name), data(new objectSet), SkylineSet(new vector<const DataObject*>) {
		readData(file_name, *data);
		outputName = fileBaseName(Fname);
		/*TODO: Can make this a friend function*/
	}

	/*TODO: Add preference relation specification*/

	/*Certain Data <=> Single instance instanceSet for every object*/
	/*Locationally Uncertain Data <=> Multi-instance instanceSet for atleast one object*/
	/*Existentially Uncertain Data <=> TODO: need to modfiy current weight allocation schema*/

	void findSkyline();					//full-Skyline
	void findSkyline(int k);			//k-Skyline
	void findSkyline(double p);			//p-Skyline
	void findSkyline(double p, int k);	//kop-Skyline

	const vector<const DataObject*>& getSkyline() const { return *SkylineSet; }
	const objectSet& getData() const { return *data; }

	~Skyline() {
		delete data;
		delete SkylineSet;
	}
};

void Skyline::findSkyline() {
	outputName = "./../data/skyline_data/result_" + outputName;
}

void Skyline::findSkyline(int k) {
	outputName = "./../data/skyline_data/result_K_" + outputName;
}

void Skyline::findSkyline(double p) {
	outputName = "./../data/skyline_data/result_P_" + outputName;

	p_(getData(), p, *SkylineSet);
	// p_BottomUp(getData(), p, *SkylineSet);

	cout << "Skyline found." << endl;
	writeData(outputName, getSkyline());
}

void Skyline::findSkyline(double p, int k) {
	outputName = "./../data/skyline_data/result_KoP_" + outputName;
}

#endif