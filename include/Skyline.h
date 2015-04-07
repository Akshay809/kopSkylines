#ifndef SKYLIN_H
#define SKYLIN_H

#include <Data.h>
#include <DataReader.h>
#include <Rtree.h>
#include <Heap.h>
#include <unordered_set>

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
	objectSet& data, Skyline;
	string Fname;
public:
	Skyline(string file_name): Fname(file_name) {
		objectSet& data = readData(Fname);
	}

	/*TODO: Add preference relation specification*/

	/*Certain Data <=> Single instance instanceSet for every object*/
	/*Locationally Uncertain Data <=> Multi-instance instanceSet for atleast one object*/
	/*Existentially Uncertain Data <=> TODO: need to modfiy current weight allocation schema*/

	virtual void findSkyline() {}				//full-Skyline
	virtual void findSkyline(int k) {}			//k-Skyline
	virtual void findSkyline(double p) {}		//p-Skyline
	virtual void findSkyline(double p, int k) {}	//kop-Skyline

	~Skyline();	
};
