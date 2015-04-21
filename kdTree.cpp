#include <Data.h>
using namespace std;

struct TreeNode {
	TreeNode* left, right;
	int splitIndex;
	DataInstance MBBmin, MBBmax;
	/*Stores the region covered by this node*/
	TreeNode(): splitIndex(-1), left(NULL), right(NULL), MBBmin(DataObject::Origin), MBBmax(DataObject::Origin) {}
	/* -1 split index implies leaf node */
};

class kdTree {
private:
	vector<DataInstance>& points;
	int numAttr;
	/*
		Attributes of DataInstance needed:
			- DataMap dataStore
			- number of attributes
	*/
public:
	kdTree(vector<DataInstance>& points): points(points) {
		if(points.size()!=0) {
			points[0].getDataStore().
			numAttr = 
		}
	}
	void buildTree(int startIndex, int endIndex, int depth) {

	}
	~kdTree();
	
};