#ifndef RANDOM_DATASET_H
#define RANDOM_DATASET_H
#include <iostream>
#include <iomanip>
#include <random>
#include <map>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace std::chrono;
class Point3D {
public:
	float x, y, z;
	map<int, float> myMap;
	Point3D() {
		x = 0;
		y = 0;
		z = 0;
		myMap.insert({ 0, x });
		myMap.insert({ 1, y });
		myMap.insert({ 2, z });
	}
	Point3D(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
		myMap.insert({ 0, x });
		myMap.insert({ 1, y });
		myMap.insert({ 2, z });
	}
	void print() {
		cout << "(" << x << ", " << y << ", " << z << ")";
	}
};
void ranDomData(Point3D*& data, int size){ 
	vector<float> randomVect;
	int curr_index = 0;
	srand(time(nullptr));
	for(int i = 0; i < 3 * size && curr_index < size; i++) {
		float temp = ((float)rand() / RAND_MAX) * 99.999;
		randomVect.push_back(temp);
		if(randomVect.size() == 3){
			float x_rand = round(randomVect[0] * 1000) / 1000;
			float y_rand = round(randomVect[1] * 1000) / 1000;
			float z_rand = round(randomVect[2] * 1000) / 1000;
			Point3D randomPoint(x_rand, y_rand, z_rand);
			data[curr_index] = randomPoint;
			randomVect.clear();
			curr_index++;
		}
	}
}
Point3D* createDataSet(int size) {
	Point3D* data = new Point3D[size];
	ranDomData(data, size);
	return data;
}
vector<Point3D> convertArrayToVector(Point3D* data, int size) {
	vector<Point3D> res;
	for(int i = 0; i < size; i++) {
		res.push_back(data[i]);
	}
	return res;
}
void printDataSet(Point3D* dataSet, int size) {
	cout << "Random dataset with " << size << " 3D-Points, accuracy = 0.001: " << endl;
	for (int i = 0; i < size; i++) {
		dataSet[i].print();
		cout << endl;
	}
}
void printDataSetForBalancedTree(vector<Point3D>dataSet, int size) {
	cout << "Random dataset with " << size << " 3D-Points, accuracy = 0.001: " << endl;
	for (int i = 0; i < size; i++) {
		dataSet[i].print();
		cout << endl;
	}
}
void handleException(float& x){
	if(x < 0) x = 0;
	if(x >= 100) x = 99.999;
}
bool isEqual(Point3D point1, Point3D point2) {
	for (int i = 0; i < 3; i++) {
		if (point1.myMap[i] != point2.myMap[i]) {
			return false;
		}
	}
	return true;
}
double distance(Point3D point1, Point3D point2) {
	return sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2) + pow(point1.z - point2.z, 2));
}
#endif
