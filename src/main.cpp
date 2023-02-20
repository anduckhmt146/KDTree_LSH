#include "KdTree.cpp"
#include "Hashing.cpp"
using namespace std;

void createDataLog(int arr[], int n) {
	cout << "In option 2, program automatically random 100 query point to test with randomly automatical dataset." << endl;
	cout << "*Note: searchNNP = Total searchNNP time of 100 point continously." << endl;
	for (int i = 0; i < n; i++) {
		int initialize_dataSet = arr[i];
		KdTree tree;
		KdTree balancedTree;
		Hashing table(initialize_dataSet);
		Point3D* dataSet = createDataSet(initialize_dataSet);
		vector<Point3D> dataSet_vect = convertArrayToVector(dataSet, initialize_dataSet);
		auto startCreateTree = high_resolution_clock::now();
		tree.insertDataSet(dataSet, initialize_dataSet);
		auto endCreateTree = high_resolution_clock::now();
		auto durationCreateTree = duration_cast<microseconds>(endCreateTree - startCreateTree);
		double createTree_time = durationCreateTree.count() * pow(10, -6);
		auto startCreateBalancedTree = high_resolution_clock::now();
		balancedTree.insertDataSetToBalancedTree(dataSet_vect);
		auto endCreateBalancedTree = high_resolution_clock::now();
		auto durationCreateBalancedTree = duration_cast<microseconds>(endCreateBalancedTree - startCreateBalancedTree);
		double createBalancedTree_time = durationCreateBalancedTree.count() * pow(10, -6);
		Point3D* queryDataSet = createDataSet(100);
		auto startTreeSearch = high_resolution_clock::now();
		for(int i = 0; i < 100; i++) {
			tree.searchNearestNeighborPoint(queryDataSet[i], false);
		}
		auto endTreeSearch = high_resolution_clock::now();
		auto durationTreeSearch = duration_cast<microseconds>(endTreeSearch - startTreeSearch);
		double searchTree_time = durationTreeSearch.count() * pow(10, -6);
		auto startBalancedTreeSearch = high_resolution_clock::now();
		for(int i = 0; i < 100; i++) {
			balancedTree.searchNearestNeighborPoint(queryDataSet[i], false);
		}
		auto endBalancedTreeSearch = high_resolution_clock::now();
		auto durationBalancedTreeSearch = duration_cast<microseconds>(endBalancedTreeSearch - startBalancedTreeSearch);
		double searchBalancedTree_time = durationBalancedTreeSearch.count() * pow(10, -6);
		auto startCreateHash = high_resolution_clock::now();
		table.insertDataSet(dataSet, initialize_dataSet);
		auto endCreateHash = high_resolution_clock::now();
		auto durationCreateHash = duration_cast<microseconds>(endCreateHash - startCreateHash);
		double createHash_time = durationCreateHash.count() * pow(10, -6);
		auto startHashSearch = high_resolution_clock::now();
		for(int i = 0; i < 100; i++) {
			table.searchNearestNeighborPoint(queryDataSet[i], false);
		}
		auto endHashSearch = high_resolution_clock::now();
		auto durationHashSearch = duration_cast<microseconds>(endHashSearch - startHashSearch);
		double searchHash_time = durationHashSearch.count() * pow(10, -6);
		cout << "\nSize: " << initialize_dataSet << endl;
		cout << "*Kd-Tree: " << "Build - " << createTree_time << ", SearchNNP - " << searchTree_time << endl;
		cout << "*Balanced Kd-Tree: " << "Build - " << createBalancedTree_time << ", SearchNNP - " << searchBalancedTree_time << endl;
		cout << "*Hash: " << "Build - " << createHash_time << ", SearchNNP - " << searchHash_time << endl;
		clear(tree.root);
		clear(balancedTree.root);
		table.clear();
		delete[] dataSet;
		dataSet_vect.clear();
	}
}
void handleQueryPointException(Point3D& query){
	handleException(query.x);
	handleException(query.y);
	handleException(query.z);
}
void linearSearch(Point3D* dataSet, Point3D query, int sizeOfData) {
	Point3D nearestNeighborPoint;
	double min_dist = 99999;
	double dist = 0;
	auto start = high_resolution_clock::now();
	for (int i = 0; i < sizeOfData; i++) {
		dist = distance(dataSet[i], query);
		if (dist < min_dist) {
			min_dist = dist;
			nearestNeighborPoint = dataSet[i];
		}
	}
	auto end = high_resolution_clock::now();
	auto durationSearch = duration_cast<microseconds>(end - start);
	double searchTime = durationSearch.count() * pow(10, -6);
	bool checkEqual = isEqual(nearestNeighborPoint, query);
	cout << "*Linear search O(N): " << endl;
	if (checkEqual) {
		cout << "-Output: Found exacly Query Point ";
		query.print();
		cout << " in dataSet." << endl;
	}
	else {
		cout << "-Output: Nearest Neighbor Point: ";
		nearestNeighborPoint.print();
		cout << " - Distance: " << distance(nearestNeighborPoint, query) << endl;
	}
	cout << "-SearchNNP Time: " << searchTime << " seconds." << endl;
	cout << "-Number of comparisons: " << sizeOfData << endl;
}
int main() {
	cout << "-------------------------------------------------------MENU----------------------------------------------------- \n";
	cout << "1. You can create a dataSet, view and find NNP with your input Query Point. \n";
	cout << "2. Performace testing between KdTree and Hashing with automatic dataset and search 100 random point continuously \n";
	string choice;
	do {
		cout << "Enter your choice! \n";
		cin >> choice;
		if(choice != "1" && choice != "2") {
			cout << "Invalid choice ! Your choice must be 1 or 2 !" << endl;
		}
	} while (choice != "1" && choice != "2");
	if(choice == "1") {
		int size;
		do{
			cout << "Enter the size of dataSet: " << endl;
			cin >> size;
			if(size <= 0){
				cout << "Invalid size ! Size of dataSet is greater than 0 !" << endl;
			}
		}while(size <= 0);
		Point3D* dataSet = createDataSet(size);
		cout << "Randoming a dataSet. Loading..." << endl;
		vector<Point3D> dataSet_vect = convertArrayToVector(dataSet, size);
		do {
			cout << "Do you want to view dataSet ? If yes, please type Y/y/Yes/yes, else type N/n/No/no: " << endl;
			cin >> choice;
			if (choice != "Y" && choice != "y" && choice != "Yes" && choice != "yes"
				&& choice != "n" && choice != "N" && choice != "No" && choice != "no") {
				cout << "Invalid choice ! Your choice must be Y/y/Yes/yes or N/n/No/no !" << endl;
			}
			if (choice == "Y" || choice == "y" || choice == "Yes" || choice == "yes") {
				printDataSet(dataSet, size);
				break;
			}
		} while (choice != "Y" && choice != "y" && choice != "Yes" && choice != "yes"
				&& choice != "n" && choice != "N" && choice != "No" && choice != "no");
		cout << "Creating a KdTree from dataSet above. Loading..." << endl;
		KdTree tree;
		KdTree balancedTree;
		auto startCreateTree = high_resolution_clock::now();
		tree.insertDataSet(dataSet, size);
		auto endCreateTree = high_resolution_clock::now();
		cout << "Insert tree successfully !" << endl;
		cout << "Creating a Balanced KdTree from dataSet above. Loading..." << endl;
		auto startCreateBalancedTree = high_resolution_clock::now();
		balancedTree.insertDataSetToBalancedTree(dataSet_vect);
		auto endCreateBalancedTree = high_resolution_clock::now();
		cout << "Insert balanced tree successfully !" << endl;
		double createTree_time = duration_cast<microseconds>(endCreateTree - startCreateTree).count() * pow(10, -6);
		double createBalancedTree_time = duration_cast<microseconds>(endCreateBalancedTree - startCreateBalancedTree).count() * pow(10, -6);
		cout << "Creating a HashTable from dataSet above. Loading..." << endl;
		Hashing table(size);
		auto startCreateHash = high_resolution_clock::now();
		table.insertDataSet(dataSet, size);
		auto endCreateHash = high_resolution_clock::now();
		cout << "Insert HashTable successfully !" << endl;
		double createHash_time =  duration_cast<microseconds>(endCreateHash - startCreateHash).count() * pow(10, -6);
		string decision;
		float x, y, z;
		cout << "Please input coordinate of Query Point that you want to find in dataSet: " << endl;
		cout << "X_0: ";
		cin >> x;
		cout << "Y_0: ";
		cin >> y;
		cout << "Z_0: ";
		cin >> z;
		Point3D query = Point3D(x, y, z);
		cout << "Query Point is: ";
		query.print();
		cout << endl;
		handleQueryPointException(query);
		cout << "Performace with dataSet size = " << size << ": " << endl;
		linearSearch(dataSet, query, size);
		cout << "\n*Kd-Tree:" << endl;
		cout << "-Build Tree Time: " << createTree_time << " seconds." << endl;
		tree.searchNearestNeighborPoint(query);
		cout << "\n*Balanced Kd-Tree: " << endl;
		cout << "-Build Balanced Tree Time: " << createBalancedTree_time << " seconds." << endl;
		balancedTree.searchNearestNeighborPoint(query);
		cout << "\n*Hashing:" << endl;
		cout << "-Build Hash Time: " << createHash_time << " seconds." << endl;
		table.searchNearestNeighborPoint(query);
		clear(tree.root);
		clear(balancedTree.root);
		table.clear();
		delete[] dataSet;
		dataSet_vect.clear();
	}
	else if(choice == "2") {
		int arr[] = { 100,500,1000,5000,10000,50000,100000,500000,1000000 };
		int n = sizeof(arr) / sizeof(arr[0]);
		createDataLog(arr, n);
	}
	else {
		cout << "A new update will be available soon !";
	}
}
