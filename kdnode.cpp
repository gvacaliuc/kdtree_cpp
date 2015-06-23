#include <assert.h>
#include <cstddef>
#include "kdnode.h"
#include <stdio.h>
#include <vector>

/*	This file defines the class laid out in 'kdnode.h'.

	A kdnode is an object housing for a std::vector<double>, along
	with other relevant attributes and methods.
	
	A set of nodes is created as so:
		kdnode node[numSamples];
		for i = 1:numSamples:
			node[i] = kdnode(data, i, dim)
	where: 
		numSamples	= the number of samples in original data matrix,
		dim 		= the numer of dimesnsions in original data matrix,
		i			= the node's lifelong index.
		 	
	The array node is now ready for use in functions of kdtree.cpp.

	Please refer to these class member functions for more of the intricacies.

	- Gabriel Vacaliuc - edited - 06/22/15
*/

//	kdnode constructor 
kdnode::kdnode(){};

kdnode::kdnode(std::vector<double> pt, int id, int d){
	point = pt;
	idx = id;
	dim = d;
	int i = 0;
};

//	Returns index of parent node
int kdnode::getParent(){
	if (this->parent != -1){
		return (this->parent);
	}
	else return -1; // He's an orphan!	
};

//	Returns index of child node after given direction ( 0 left, 1 right )
int kdnode::getChild(int direction){
	if (direction == 0){
		if (this->left != -1){
			return (this->left);
		}
		else return -1; //No child on left
	}
	else{
		if (this->right != -1){
			return (this->right);
		}
		else return -1; //No child on right
	}
};

//	Assigns child node's parent as the given index
void kdnode::assignParent(int p){
	this->parent = p;
};

//	Assigns parent nodes' child as the given index
void kdnode::assignChild(int c, int direction){
	if (direction == 0){
		this->left = c;
	}
	else{ this->right = c; }
};

//	Returns a node's index
int kdnode::getidx(){ return this->idx; };

//	Returns the node's point value of a given level
double kdnode::getPointVal(int level){
	assert( level >= 0 && level < this->dim );
	return this->point[level];
};

//	Returns the node's point value in a std::vector
std::vector<double> kdnode::getPointVal(){
	return this->point;
};

//	Prints the node's point out -- debugging
void kdnode::printPointVal(){
	int i = 0;
	printf("Point: (%2.8f,",point[0]);
	for( i = 1; i < dim-1; i++){
		printf("%2.8f,", point[i]);
	}
	printf("%2.8f)\n", point[dim-1]);
};

//	Returns the node's dimension
int kdnode::getdim(){
	return this->dim;
};

//	Sets 'level' (axis, really) the hyperplane splits at (for this node)
void kdnode::setLevel(int level){
	this->level = level;
};

//	Returns whether node is leaf
bool kdnode::isLeaf(){
	return (this->left == -1 && this->right == -1);
};

//	Gets 'level' (axis, really) the hyperplane splits at (for this node)
int kdnode::getLevel(){
	return this->level;
};
