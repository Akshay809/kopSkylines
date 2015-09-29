#ifndef SKYLINE_H
#define SKYLINE_H

#include <Data.h>
#include <DataReader.h>
#include <DataWriter.h>

#include <sstream>
#include <string>

using namespace std;

/* full-skyline Algorithms*/
void fullSkyline(const objectSet&, vector<const DataObject*>&); /*naive-version*/

/* k-Skyline Algorithms */
void kSkyline(const objectSet&, int, vector<const DataObject*>&); /*naive-version*/

/* p-Skyline Algorithms */
void pSkyline(const objectSet&, double, vector<const DataObject*>&); /*naive-version*/
// void p_TopDown(const objectSet&, double, vector<const DataObject*>&);
// void p_BottomUp(const objectSet&, double, vector<const DataObject*>&);

/* kop-Skyline Algorithms */
void kopSkyline(const objectSet&, double, int, vector<const DataObject*>&); /*naive-version*/
// void kop_BPR(const objectSet&, double, int, vector<const DataObject*>&); /*Bounding_Pruning_Refining-version*/


class Skyline {
private:
	vector<const DataObject*> * SkylineSet;
	objectSet * data;
	/*Purposefully creating these data ojects here, as heap allocation is required and I do not want a memory leak*/
	string outputName;
public:
	const string Fname, outDir;

	Skyline(string file_name, string outDir): Fname(file_name), data(new objectSet), SkylineSet(new vector<const DataObject*>), outDir(outDir) {
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
	outputName = outDir + "result_" + outputName;

	fullSkyline(getData(), *SkylineSet); /*naive-version*/
		cout << "Skyline found." << endl;
	writeData(outputName, getSkyline());
}

void Skyline::findSkyline(int k) {
	outputName = outDir + "result_K_" + outputName;

	kSkyline(getData(), k, *SkylineSet); /*naive-version*/
		cout << "Skyline found." << endl;
	writeData(outputName, getSkyline());
}

void Skyline::findSkyline(double p) {
	outputName = outDir + "result_P_" + outputName;

	pSkyline(getData(), p, *SkylineSet);
		cout << "Skyline found." << endl;
	writeData(outputName, getSkyline());
}

void Skyline::findSkyline(double p, int k) {
	outputName = outDir + "result_KoP_" + outputName;

	kopSkyline(getData(), p, k, *SkylineSet);
		cout << "Skyline found." << endl;
	writeData(outputName, getSkyline());
}

#endif