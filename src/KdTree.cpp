#include "randomDataSet.h"

class KdNode {
public:
	Point3D val;
	KdNode* left;
	KdNode* right;
	KdNode() : val(Point3D()), left(nullptr), right(nullptr){}
	KdNode(Point3D val) {
		this->val = val;
		this->left = this->right = nullptr;
	}
};
class KdTree {
public:
	KdNode* root;
	KdTree() {
		this->root = nullptr;
	}
	KdNode* insertRec(KdNode* root, Point3D val, int depth) {
		if (!root) {
			return new KdNode(val);
		}
		int axis = depth % 3;
		if (val.myMap[axis] < root->val.myMap[axis]) {
			root->left = insertRec(root->left, val, depth + 1);
		}
		else {
			root->right = insertRec(root->right, val, depth + 1);
		}
		return root;
	}
	void insert(Point3D val) {
		this->root = insertRec(root, val, 0);
	}
	void insertDataSet(Point3D* val, int size) {
		for (int i = 0; i < size; i++) {
			insert(val[i]);
		}
	}
	struct ComparatorX {
		bool operator() (const Point3D point1, const Point3D point2) const {
			return point1.x < point2.x;
		}
	};
	struct ComparatorY {
		bool operator() (const Point3D point1, const Point3D point2) const {
			return point1.y < point2.y;
		}
	};
	struct ComparatorZ {
		bool operator() (const Point3D point1, const Point3D point2) const {
			return point1.z < point2.z;
		}
	};
	KdNode* insertBalancedTreeRec(vector<Point3D> dataSet, int depth) {
		if (dataSet.size() == 0) return nullptr;
		int axis = depth % 3;
		if (axis == 0) {
			sort(dataSet.begin(), dataSet.end(), ComparatorX());
		}
		else if (axis == 1) {
			sort(dataSet.begin(), dataSet.end(), ComparatorY());
		}
		else {
			sort(dataSet.begin(), dataSet.end(), ComparatorZ());
		}
		int median = dataSet.size() / 2;
		KdNode* node = new KdNode(dataSet[median]);
		vector<Point3D>left;
		vector<Point3D>right;
		for (int i = 0; i < dataSet.size(); i++) {
			if (i == median) continue;
			if (i < median) left.push_back(dataSet[i]);
			else if (i > median) right.push_back(dataSet[i]);
		}
		node->left = insertBalancedTreeRec(left, depth + 1);
		node->right = insertBalancedTreeRec(right, depth + 1);
		return node;
	}
	void insertDataSetToBalancedTree(vector<Point3D> dataSet) {
		this->root = insertBalancedTreeRec(dataSet, 0);
	}
	KdNode* closer(KdNode* n0, KdNode* n1, KdNode* query){
		if(!n0) return n1;
		if(!n1) return n0;
		double d1 = distance(n0->val, query->val);
		double d2 = distance(n1->val, query->val);
		if(d1 < d2) {
			return n0;
		}
		return n1;
	}
	KdNode* nearestNeighborRec(KdNode* root, KdNode* query, int depth, int& numberOfComparison) {
		if(!root) return nullptr;
		KdNode* nextBranch = nullptr;
		KdNode* otherBranch = nullptr;
		int axis = depth % 3;
		if (query->val.myMap[axis] < root->val.myMap[axis]) {
			nextBranch = root->left;
			otherBranch = root->right;
		}
		else {
			nextBranch = root->right;
			otherBranch = root->left;
		}
		numberOfComparison += 1;
		KdNode* temp = nearestNeighborRec(nextBranch, query, depth + 1, numberOfComparison);
		KdNode* best = closer(temp, root, query);
		double radius = distance(query->val, best->val);
		double dist = abs(query->val.myMap[axis] - root->val.myMap[axis]);
		if(dist <= radius) {
			temp = nearestNeighborRec(otherBranch, query, depth + 1, numberOfComparison);
			best = closer(temp, best, query);
		}
		return best;
	}
	KdNode* nearestNeighbor(KdNode* query, int& numberOfComparison) {
		return nearestNeighborRec(root, query, 0, numberOfComparison);
	}
	void searchNearestNeighborPoint(Point3D queryVal, bool flag = true) {
		KdNode* query = new KdNode(queryVal);
		int numberOfComparison = 0;
		auto startTreeSearch = high_resolution_clock::now();
		KdNode* nearestNeighborPoint = nearestNeighbor(query, numberOfComparison);
		auto endTreeSearch = high_resolution_clock::now();
		double searchTree_time = duration_cast<microseconds>(endTreeSearch - startTreeSearch).count() * pow(10, -6);
		if(!nearestNeighborPoint) {
			if(flag) cout << "-KdTree is empty !" << endl;
			return;
		}
		bool checkEqual = isEqual(nearestNeighborPoint->val, query->val);
		if(!flag) return;
		if(checkEqual) {
			cout << "-Output: Found exacly Query Point ";
			query->val.print();
			cout << " in dataSet." << endl;
		}
		else {
			cout << "-Output: Nearest Neighbor Point: ";
			nearestNeighborPoint->val.print();
			cout << " - Distance: " << distance(nearestNeighborPoint->val, query->val) << endl;
		}
		cout << "-SearchNNP Time: " << searchTree_time << " seconds." << endl;
		cout << "-Number of comparisons: " << numberOfComparison << endl;
	}
	int getHeightOfTree(KdNode* root){
		if(!root) return 0;
		return 1 + max(getHeightOfTree(root->left), getHeightOfTree(root->right));
	}
};
void clear(KdNode*& root) {
	if (root) {
		clear(root->left);
		clear(root->right);
		delete root;
		root = nullptr;
	}
}