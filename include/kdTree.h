#ifndef KD_TREE_H
#define KD_TREE_H

/*Refer this: http://www.cs.uu.nl/docs/vakken/ga/slides5.pdf , for algorithm related details*/

#include <iostream>
#include <algorithm>
#include <unordered_set>

#include <Data.h>

using namespace std;

class kdTree {
private:
	const vector<DataInstance>& points;
	vector<int> indices;
	int numAttr;
public:
	class TreeNode;
	struct Query;
	struct lower;

	TreeNode *root;

	kdTree(const vector<DataInstance>&);

	TreeNode* buildTree(int, int, int);

	const vector<int>& searchR(const Rectangle&);
	const vector<int>& searchR(const DataInstance& lowerEnd, const DataInstance& upperEnd) {
		Rectangle R(lowerEnd, upperEnd);
		return searchR(R);
	}

	void printTree();
	void _printTree(TreeNode *, const vector<DataInstance>&);
	// Rectangle& region(TreeNode*);

	~kdTree();
};

#endif