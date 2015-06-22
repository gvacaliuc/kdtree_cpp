#include <vector>
#include "assert.h"
#include "kdtree.h"
#include <stdio.h>

neighbor::neighbor(){
	this->dist = -1.0; // Identifies neighbor as not set up yet
};

void neighbor::setIdx(int idx){
	this->idx = idx;
};

void neighbor::setDist(double dist){
	this->dist = dist;
};

void neighbor::setPoint(std::vector<double> point){
	this->point = point;
};

int neighbor::getIdx(){
	return this->idx;
};

double neighbor::getDist(){
	return this->dist;
};

std::vector<double> neighbor::getPoint(){
	return this->point;
};

neighborArray::neighborArray(){};

neighborArray::neighborArray(int n){
	std::vector<neighbor> neigh (n);
	int i;
	for (i = 0; i < n; i++){
		neigh[i].setDist(-1.0);
	}
	neighbors = neigh;
	for (i = 0; i < n; i++){
//		printf("dist %f\n", neighbors[i].getDist());
	}
};

neighbor& neighborArray::operator[](int i){
	assert( i < this->neighbors.size() );
	return this->neighbors[i];
};

int neighborArray::getMinIdx(){
	int idx = 0, i;
	for (i = 0; i < this->neighbors.size(); i++){
	//	printf("mindex: %u\n", idx);
		if (this->neighbors[i].getDist() == -1.0){ idx++; }
		else{ break; return idx; }
	}
	//printf("returned idx: %u\n", idx);
	return idx;
};

double neighborArray::getMaxDist(){
	this->sortNeighbors();
	int size = this->neighbors.size();
	return this->neighbors[size - 1].getDist();
};

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
	//	printf("sorted index: %u\n", (int) arr[i+numneigh]);
	}
	for (i = 0; i < numneigh; i++){
		temp[i] = this->neighbors[ (int) arr[i+numneigh] ];
	}
	for (i = 0; i < numneigh; i++){
		
		this->neighbors[i] = temp[i];
	}
	delete[] arr;
};

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

int neighborArray::size(){
	return this->neighbors.size();
};
