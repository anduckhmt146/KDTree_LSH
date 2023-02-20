#include "randomDataSet.h"

class Hashing {
public:
	vector <vector<Point3D>> hashTable;
	int binwidth;
	int sizeOfTable;
	Hashing(int sizeOfDataSet) {
		if (sizeOfDataSet >= 0 && sizeOfDataSet < 1000) {
			this->binwidth = 5;
			this->sizeOfTable = 200000;
			hashTable.resize(sizeOfTable);
		}
		else if (sizeOfDataSet >= 1000 && sizeOfDataSet < 100000) {
			this->binwidth = 2;
			this->sizeOfTable = 500000;
			hashTable.resize(sizeOfTable);
		}
		else  {
			this->binwidth = 1;
			this->sizeOfTable = 1000000;
			hashTable.resize(sizeOfTable);
		}
	}
	int hashFunct(Point3D point) {
		int x_hash = (int)point.x;
		int y_hash = (int)point.y;
		int z_hash = (int)point.z;
		return (int)(x_hash * 10000 / binwidth) + (int)(y_hash * 100 / binwidth) + (int)(z_hash * 1 / binwidth);
	}
	void insertPoint(Point3D point) {
		int index = hashFunct(point);
		hashTable[index].push_back(point);
	}
	void insertDataSet(Point3D* dataSet, int size) {
		for (int i = 0; i < size; i++) {
			insertPoint(dataSet[i]);
		}
	}
	void insertDataSetForVector(vector<Point3D>dataSet, int size){
		for (int i = 0; i < size; i++) {
			insertPoint(dataSet[i]);
		}
	}
	bool isInvalidPoint(Point3D point){
		return point.x < 0 || point.y < 0 || point.z < 0 || point.x > 100 || point.y > 100 || point.z > 100;
	}
	void localitySearch(Point3D query, double& min_dist, Point3D& nearestNeighborPoint, int& numberOfComparison) {
		int i = binwidth;
		bool flag = false;
		while (true) {
			for (int dx = -i; dx <= i; dx++) {
				for (int dy = -i; dy <= i; dy++) {
					for (int dz = -i; dz <= i; dz++) {
						Point3D temp = Point3D(query.x + dx, query.y + dy, query.z + dz);
						if(isInvalidPoint(temp)) continue;
						int temp_pos = hashFunct(temp);
						if (hashTable[temp_pos].size() == 0) continue;
						flag = true;
						for (auto it = hashTable[temp_pos].begin(); it != hashTable[temp_pos].end(); it++) {
							numberOfComparison++;
							double dist = distance(query, *it);
							if (dist < min_dist) {
								min_dist = dist;
								nearestNeighborPoint = *it;
							}
						}
					}
				}
			}
			if (flag) return;
			i += binwidth;
		}
	}
	void searchNearestNeighborPoint(Point3D query, bool flag = true) {
		double min_dist = 99999;
		Point3D nearestNeighborPoint;
		auto startHashSearch = high_resolution_clock::now();
		int query_index = hashFunct(query);
		int numberOfComparison = 0;
		if (hashTable[query_index].size() == 0) {
			localitySearch(query, min_dist, nearestNeighborPoint, numberOfComparison);
		}
		else {
			for (auto it = hashTable[query_index].begin(); it != hashTable[query_index].end(); it++) {
				double dist = distance(query, *it);
				numberOfComparison++;
				if (dist < min_dist) {
					min_dist = dist;
					nearestNeighborPoint = *it;
				}
			}
			localitySearch(query, min_dist, nearestNeighborPoint, numberOfComparison);
		}
		auto endHashSearch = high_resolution_clock::now();
		double searchHash_time = duration_cast<microseconds>(endHashSearch - startHashSearch).count() * pow(10, -6);
		if(!flag) return;
		bool checkEqual = isEqual(nearestNeighborPoint, query);
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
		cout << "-SearchNNP Time: " << searchHash_time << " seconds." << endl;
		cout << "-Number of comparisons: " << numberOfComparison << endl;
	}
	void clear(){
		for(int i = 0; i < sizeOfTable; i++) {
			if(hashTable[i].size() == 0) continue;
			hashTable[i].clear();
		}
		hashTable.clear();
		this->sizeOfTable = 0;
		this->binwidth = 0;
	}
};