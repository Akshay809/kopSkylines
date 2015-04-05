#include <Data.h>
#include <Rtree.h>
#include <Heap.h>
#include <unordered_set>

using namespace std;
/*
struct ObjectInfo {
	vector<int>& DominatingObjectIDs;
	double high, low;
	ObjectInfo(int low, int high, vector<int> &v): low(low), high(high), DominatingObjectIDs(v) {}
};
*/

class Skyline {
public:
	Skyline();
	~Skyline();
};

class TopDown : public Skyline {
public:
	TopDown();
	~TopDown();
	
};