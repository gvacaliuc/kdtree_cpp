#include <vector>
#include "assert.h"
#include "kdtree.h"
#include <stdio.h>

//	Identifies the neighbor as un-initialized
neighbor::neighbor(){
	this->dist = -1.0;
};

//	Sets index
void neighbor::setIdx(int idx){
	this->idx = idx;
};

//	Sets distance
void neighbor::setDist(double dist){
	this->dist = dist;
};

//	Sets point
void neighbor::setPoint(std::vector<double> point){
	this->point = point;
};

//	Gets index
int neighbor::getIdx(){
	return this->idx;
};

//	Gets distance
double neighbor::getDist(){
	return this->dist;
};

//	Gets point
std::vector<double> neighbor::getPoint(){
	return this->point;
};

//	Blank constructor
neighborArray::neighborArray(){};

//	Constructor, sets distance to -1 to id as uninitialized
neighborArray::neighborArray(int n){
	std::vector<neighbor> neigh (n);
	int i;
	for (i = 0; i < n; i++){
		neigh[i].setDist(-1.0);
	}
	neighbors = neigh;
};

//	operator[] to ease use of neighborArray
neighbor& neighborArray::operator[](int i){
	assert( i >= 0 && i < this->neighbors.size() );
	return this->neighbors[i];
};

//	returns where the first actual neighbor lies -- (only used in neighbor initializer)
int neighborArray::getMinIdx(){
	int idx = 0, i;
	for (i = 0; i < this->neighbors.size(); i++){
		if (this->neighbors[i].getDist() == -1.0){ idx++; }
		else{ break; return idx; }
	}
	return idx;
};

//	returns the largest distance in a neighborArray
double neighborArray::getMaxDist(){
	this->sortNeighbors();
	int size = this->neighbors.size();
	return this->neighbors[size - 1].getDist();
};

//	sorts the neighbors based on dist^2
void neighborArray::sortNeighbors(){
	int numneigh = this->neighbors.size();
	double* arr = new double[numneigh*2];
	int i;
	for(i = 0; i < numneigh; i++){
		arr[i] = this->neighbors[i].getDist();
		arr[i+numneigh] = i;
	}
	kdtree::quickSort(arr, 0, numneigh - 1, numneigh);
	neighborArray temp;
	temp = neighborArray(numneigh);
	for (i = 0; i < numneigh; i++){
		temp[i] = this->neighbors[ (int) arr[i+numneigh] ];
	}
	for (i = 0; i < numneigh; i++){
		
		this->neighbors[i] = temp[i];
	}
	delete[] arr;
};

//	inserts a value into the neighborArray, rejects duplicates
void neighborArray::insert(neighbor n){
	//printf("mindex: %u\n", this->getMinIdx());	
	if (this->getMinIdx() != 0){
		this->neighbors[this->getMinIdx() - 1] = n;
		this->sortNeighbors();
	}
	else{
		int i, numneigh = this->neighbors.size();
		for (i = 0; i < numneigh; i++){
			if ( n.getIdx() == this->neighbors[i].getIdx() ){ break; }
			else if ( n.getDist() < this->neighbors[i].getDist() ){
				this->neighbors.insert( (this->neighbors.begin() + i), n);
				this->neighbors.pop_back();
				break;
			}
		}
	}
};

//	returns the size of the neighbor array
int neighborArray::size(){
	return this->neighbors.size();
};
