#include <kdTree.h>

using namespace std;

struct kdTree::lower {
	const vector<DataInstance>& points;
	int dim;

	lower(const vector<DataInstance>& points, int dim): points(points), dim(dim) {}

	bool operator()(const int i1, const int i2) {
		return points[i1].getDataStore()[dim]<points[i2].getDataStore()[dim];
	}
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

	~TreeNode() {
		delete left;
		delete right;
	}
};

kdTree::kdTree(const vector<DataInstance>& points): points(points), root(NULL) {
	// cout << "Initializing the tree ...." << endl;
	// root = NULL;
	if(points.size()!=0) {
		for(int i=0;i<points.size();i++)
			indices.push_back(i);
		numAttr = points[0].getDataStore().size();
		// 	cout << "  number of attributes: " << numAttr << endl;
		// 	cout << "  number of points: " << points.size() << endl;
		// cout << "Building kd-tree...." << endl;
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
	std::nth_element(startItr+startIndex, startItr+midIndex, startItr+endIndex+1, kdTree::lower(points,dim));
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

const Rectangle& region(kdTree::TreeNode* root) {
	/*Some memory leak here, very small though*/
	Rectangle *R = new Rectangle(root->MBBmin, root->MBBmax);
	return *R;
}

struct kdTree::Query {

	const Rectangle& R;
	const vector<DataInstance>& points;
	vector<int> ans;

	Query(const Rectangle& R, const vector<DataInstance>& points): R(R), points(points) {}

	bool containsPoint(const DataInstance& point) {
		if(R.lowerEnd.isDominatedBy(R.upperEnd)) return false;
		/* Condition for point-p not belonging to Rectangle-R                  */
		/* p is better than the lower bound (p dominates the best-of-R), or,   */
		/* p is worser than the upper bound (p is dominated by the worst-of-R) */
		// return R.lowerEnd.isDominatedBy(point) || point.isDominatedBy(R.upperEnd));
		return point.isDominatedBy(R.lowerEnd) && R.upperEnd.isDominatedBy(point);
	}

	bool containsRegion(const Rectangle& region) {
		if(R.lowerEnd.isDominatedBy(R.upperEnd) || region.lowerEnd.isDominatedBy(region.upperEnd)) return false;
		return containsPoint(region.lowerEnd) && containsPoint(region.upperEnd);
		// return !(R.lowerEnd.isDominatedBy(region.lowerEnd) || region.upperEnd.isDominatedBy(R.upperEnd));
	}

	bool intersects(const Rectangle& region) {
		if(R.lowerEnd.isDominatedBy(R.upperEnd) || region.lowerEnd.isDominatedBy(region.upperEnd)) return false;
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
			// DataInstance& point = points[root->splitIndex];
			ans.push_back(root->splitIndex);
			return;
		}
		report(root->left);
		report(root->right);
	}

	void searchIn(TreeNode* root) {
		if(root==NULL) return;
		if(root->isLeaf()) {
			// cout << "Searching in Leaf " << root->splitIndex << endl;
			const DataInstance& point = points[root->splitIndex];
			if(containsPoint(point)) {
				// cout << "  Contains Point " << root->splitIndex << endl;
				// cout << &point << endl;
				ans.push_back(root->splitIndex);
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

const vector<int>& kdTree::searchR(const Rectangle& R) {
	// cout << "Searching in kd-tree...." << endl;
	Query object(R, points);
	object.searchIn(root);

	/*Some memory leak here also.*/
	vector<int>* ans = new vector<int>;
	*ans = object.ans;
	return *ans;
}

void kdTree::_printTree(kdTree::TreeNode *root, const vector<DataInstance>& points) {
	if(root==NULL) return;
	if(root->isLeaf()) {
		const DataInstance& point = points[root->splitIndex];
//		cout << "Leaf: " << root->splitIndex << endl;
		point.printDataInstance();
		return;
	}
//	cout << "Non-Leaf: " << root->splitIndex << endl;
		root->MBBmin.printDataInstance();
		root->MBBmax.printDataInstance();
	_printTree(root->left, points);
	_printTree(root->right, points);
}

void kdTree::printTree() {
	_printTree(root, points);
}

kdTree::~kdTree() {
	delete root;
}
