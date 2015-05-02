#include <Data.h>
#include <algorithm>
#include <unordered_set>

using namespace std;

struct lower {
	vector<DataInstance>& points;
	int dim;
	lower(vector<DataInstance>& points, int dim): points(points), dim(dim) {}
	bool operator()(const int i1, const int i2) {
		return points[i1].getDataStore()[dim]<points[i2].getDataStore()[dim];
	}
};

class kdTree {
private:
	vector<DataInstance>& points;
	vector<int> indices;
	int numAttr;
public:
	class TreeNode;
	TreeNode *root;
	struct Query;
	kdTree(vector<DataInstance>&);
	TreeNode* buildTree(int, int, int);
	unordered_set<DataObject*>* searchR(Rectangle&);
	unordered_set<DataObject*>* searchR(DataInstance& lowerEnd, DataInstance& upperEnd) {
		Rectangle *R = new Rectangle(lowerEnd, upperEnd);
		return searchR(*R);
	}
	void printTree();
	// Rectangle& region(TreeNode*);
	~kdTree() {}
};

class kdTree::TreeNode {
public:
	TreeNode *left, *right;
	int splitIndex;

	/*Stores the region covered by this node*/
	DataInstance MBBmin, MBBmax;

	TreeNode(int splitIndex): splitIndex(splitIndex), left(NULL), right(NULL), MBBmin(DataObject::Origin), MBBmax(DataObject::Origin) {}
	bool isLeaf() {
		/*splitIndex represents the index of the point*/
		return left==NULL && right==NULL;
	}
};


kdTree::kdTree(vector<DataInstance>& points): points(points), root(NULL) {
	cout << "Initializing the tree ...." << endl;
	root = NULL;
	if(points.size()!=0) {
		for(int i=0;i<points.size();i++)
			indices.push_back(i);
		numAttr = points[0].getDataStore().size();
			cout << "  number of attributes: " << numAttr << endl;
			cout << "  number of points: " << points.size() << endl;
		cout << "Building kd-tree...." << endl;
		root = buildTree(0, indices.size()-1, 0);
	}
}

kdTree::TreeNode* kdTree::buildTree(int startIndex, int endIndex, int depth) {
	// cout << "Building kd-tree node...." << endl;

	int midIndex = startIndex+(endIndex-startIndex)/2;
/*	cout << "\n\n";
	cout << "    StartIndex: " << startIndex << endl;
	cout << "    MidIndex: " << midIndex << endl;
	cout << "    EndIndex: " << endIndex << endl;
	cout << "\n\n";
*/
	if(startIndex==endIndex) {
		TreeNode* root = new TreeNode(indices[startIndex]);
			root->MBBmin = points[indices[startIndex]];
			root->MBBmax = points[indices[startIndex]];
		return root;
	}
	int dim = depth % numAttr;
	/*split on 'dim'*/

	vector<int>::iterator startItr = indices.begin();
	/*change points array such that nth element is in the correct position*/
	std::nth_element(startItr+startIndex, startItr+midIndex, startItr+endIndex+1, lower(points,dim));
	TreeNode* root = new TreeNode(indices[midIndex]);

/*	if(endIndex==startIndex+1)
		midIndex = startIndex;
*/
	root->left = buildTree(startIndex, midIndex, depth+1);
	root->right = buildTree(midIndex+1, endIndex, depth+1);
		root->MBBmin = root->left->MBBmin;
		root->MBBmin.minimizeWRT(root->right->MBBmin);
		root->MBBmax = root->left->MBBmax;
		root->MBBmax.maximizeWRT(root->right->MBBmax);
	return root;
}

Rectangle& region(kdTree::TreeNode* root) {
	Rectangle *R = new Rectangle(root->MBBmin, root->MBBmax);
	return *R;
}

struct kdTree::Query {
	Rectangle& R;
	unordered_set<DataObject*> ans;
	vector<DataInstance>& points;
	Query(Rectangle& R, vector<DataInstance>& points): R(R), points(points) {}
	bool containsPoint(DataInstance& point) {
		/* Condition for point-p not belonging to Rectangle-R                  */
		/* p is better than the lower bound (p dominates the best-of-R), or,   */
		/* p is worser than the upper bound (p is dominated by the worst-of-R) */
		// return R.lowerEnd.isDominatedBy(point) || point.isDominatedBy(R.upperEnd));
		return point.isDominatedBy(R.lowerEnd) && R.upperEnd.isDominatedBy(point);
	}
	bool containsRegion(Rectangle& region) {
		return containsPoint(region.lowerEnd) && containsPoint(region.upperEnd);
		// return !(R.lowerEnd.isDominatedBy(region.lowerEnd) || region.upperEnd.isDominatedBy(R.upperEnd));
	}
	bool intersects(Rectangle& region) {
		return containsPoint(region.lowerEnd) || containsPoint(region.upperEnd) || (R.lowerEnd.isDominatedBy(region.lowerEnd) && region.upperEnd.isDominatedBy(R.lowerEnd)) ||
		(R.upperEnd.isDominatedBy(region.lowerEnd) && region.upperEnd.isDominatedBy(R.upperEnd));
/*		return !(
			( R.lowerEnd.isDominatedBy(region.lowerEnd) || region.lowerEnd.isDominatedBy(R.upperEnd) )
			&&
			( R.lowerEnd.isDominatedBy(region.upperEnd) || region.upperEnd.isDominatedBy(R.upperEnd) )
			&&
			( region.lowerEnd.isDominatedBy(R.lowerEnd) || R.lowerEnd.isDominatedBy(region.upperEnd) )
			&&
			( region.lowerEnd.isDominatedBy(R.upperEnd) || R.upperEnd.isDominatedBy(region.upperEnd) )
			);
*/
	}
	void report(TreeNode* root) {
		// cout << "Reporting " << root->splitIndex << endl;
		if(root->isLeaf()) {
			DataInstance& point = points[root->splitIndex];
			ans.insert(&(point.getObjectRef()));
			return;
		}
		report(root->left);
		report(root->right);
	}
	void searchIn(TreeNode* root) {
		if(root->isLeaf()) {
			// cout << "Searching in Leaf " << root->splitIndex << endl;
			DataInstance& point = points[root->splitIndex];
			if(containsPoint(point)) {
				// cout << "  Contains Point " << root->splitIndex << endl;
				ans.insert(&(point.getObjectRef()));
			}
		}
		else {
			// cout << "Searching in Non-Leaf " << root->left->splitIndex << endl;
			if(containsRegion(region(root->left))) {
				// cout << "Contains Region " << root->left->splitIndex << endl;
				report(root->left);
			}
			else if(intersects(region(root->left))) {
				// cout << "Intersects Region " << root->left->splitIndex << endl;
				searchIn(root->left);
			}
			else {
				// cout << "No-Intersect Region " << root->left->splitIndex << endl;
			}
			if(containsRegion(region(root->right))) {
				// cout << "Contains Region " << root->right->splitIndex << endl;
				report(root->right);
			}
			else if(intersects(region(root->right))) {
				// cout << "Intersects Region " << root->right->splitIndex << endl;
				searchIn(root->right);
			}
			else {
				// cout << "No-Intersect Region " << root->right->splitIndex << endl;
			}
		}
	}
};

unordered_set<DataObject*>* kdTree::searchR(Rectangle& R) {
	Query object(R, points);
	cout << "Searching in kd-tree...." << endl;
	object.searchIn(root);
	unordered_set<DataObject*> * ans = new unordered_set<DataObject*>();
	*ans = object.ans;
	return ans;
}

void _printTree(kdTree::TreeNode *root, vector<DataInstance>& points) {
	if(root->isLeaf()) {
		DataInstance& point = points[root->splitIndex];
		cout << "Leaf: " << root->splitIndex << endl;
		point.printDataInstance();
		return;
	}
	cout << "Non-Leaf: " << root->splitIndex << endl;
		root->MBBmin.printDataInstance();
		root->MBBmax.printDataInstance();
	_printTree(root->left, points);
	_printTree(root->right, points);
}

void kdTree::printTree() {
	_printTree(root, points);
}
