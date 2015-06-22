#include <stdio.h>
#include "kdnode.h"
#include "kdtree.h"
#include <cmath>
#include <vector>
#include <string>
#include <cstring>
#include "assert.h"

/*	This file defines the class laid out in 'kdtree.h'.  It contains methods
for constructing a kd-tree type structure from a vector of kdnodes.  The
thinking and methodology of these two classes is as follows:

	A vector of kdnodes is created with indices corresponding to the indices
	of the vector and also the columns of the original data matrix of shape 
	(dim, numSamples).

	A 'tree' is created as so: kdtree tree; tree = kdtree(arrayOfNodes, numSamples, dim);

	The 'tree' data is held in the nodes of the tree.  Each node has a parent,
	leftChild, and rightChild integer variable.  That integer variable is set
	to the index of the arrayOfNodes where its corresponding kin is located.

	Please refer to these class member functions for more of the intricacies.

	- Gabriel Vacaliuc - edited - 06/18/15
*/

// kdtree constructor
kdtree::kdtree(){};
kdtree::kdtree(kdnode* arrayOfNodes, int numSamples, int dim){
	int i = 0;
	this->nodes = std::vector<kdnode> (numSamples);
	for (i = 0; i < numSamples; i++){
		this->nodes[i] = arrayOfNodes[i];
	}
	this->numSamples = numSamples;
	this->dim = dim;
};
int kdtree::getRootNode(){
	return this->root;
};

// These functions do an argument sort alongside the quicksort algorithm
// ----------------------------------------------------------------------------------------------------------------
void kdtree::swap(double* a, double* b){
   	double t = *a;
   	*a = *b;
   	*b = t;
};
int kdtree::partition (double arr[], int l, int h, int nS){
    int x = arr[h];    // pivot
    int i = (l - 1);  // Index of smaller element
 
    for (int j = l; j <= h- 1; j++)
    {
        // If current element is smaller than or equal to pivot 
        if (arr[j] <= x)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);  // Swap current element with index
			swap(&arr[i + nS], &arr[j + nS]);  // Swaps indices for argsort      
		}
    }
    swap(&arr[i + 1], &arr[h]);
	swap(&arr[i + 1 + nS], &arr[h + nS]);  
    return (i + 1);
};	 
/* arr[] --> Array to be sorted, l  --> Starting index, h  --> Ending index */
void kdtree::quickSort(double arr[], int l, int h, int nS)
{
    if (l < h)
    {
        int p = partition(arr, l, h, nS); /* Partitioning index */
        quickSort(arr, l, p - 1, nS);
        quickSort(arr, p + 1, h, nS);
    }
};
// ----------------------------------------------------------------------------------------------------------------

//Sorts data correponding to level, original index is preserved inside kdnode class
std::vector<kdnode> kdtree::sortNodes(std::vector<kdnode> data, int start, int end, int level){ 
	int numSamples = end - start + 1;
	if (numSamples > 1){
		std::vector<kdnode> temp (numSamples);		
		double* arr = new double[numSamples*2];
		int i, j,median;
		for(i = 0; i < numSamples; i++){
			arr[i] = data[i+start].getPointVal(level);
			arr[i+numSamples] = (double) i+start;
		}
		quickSort(arr, 0, numSamples - 1, numSamples); // Modified quicksorter to do argument sorting

		for(i = 0; i < numSamples; i++){				//Could be implemented to be more memory efficient
			temp[i] = data[ (int) arr[numSamples + i] ];
		}
		for(i = 0; i < numSamples; i++){
			data[i+start] = temp[i];
		}
		delete[] arr;
		return data;
	}	
};

// Helper function to get the median of numSamples-length data
int kdtree::getMedian(int numSamples){  
	return (int) ceil( (numSamples -1)/2. );
};

// Public building function, splits tree once and does the rest recursively
void kdtree::build(){
	int median, root;

	//Make temp array and sort
	std::vector<kdnode> temp = this->nodes;
	temp = kdtree::sortNodes(temp, 0, this->numSamples-1, 0);
	median = kdtree::getMedian(this->numSamples);

	//Assign root index and level
	root = temp[median].getidx();
	this->root = root;
	this->nodes[root].setLevel(0);

	//Generate rest of tree recursively
	this->nodes[root].assignChild( (*this).treeGen(temp, 0, median - 1, 1, root), 0);
	this->nodes[root].assignChild( (*this).treeGen(temp, median + 1, this->numSamples - 1, 1, root), 1);
	this->nodes[root].assignParent(-1);
};

// Private tree building function, recursively generates all branches
int kdtree::treeGen(std::vector<kdnode> nodes, int start, int end, int level, int parent){
	
	// Resets level iterator
	if (level == this->dim){ level = 0; }

	// Number of samples to consider from nodes
	int numSamples = end - start + 1;

	int median, local_root, idx, i, j;

	//  Generates rest of tree recursively
	if (level < this->dim && numSamples > 1){
		nodes = kdtree::sortNodes(nodes, start, end, level);
		median = kdtree::getMedian(numSamples) + start;
		local_root = nodes[median].getidx();
	
		// Assignments
		this->nodes[local_root].setLevel(level);
		this->nodes[local_root].assignChild( (*this).treeGen(nodes, start, median - 1, level+1, local_root), 0);
		this->nodes[local_root].assignChild( (*this).treeGen(nodes, median + 1, end, level+1, local_root), 1);
		this->nodes[local_root].assignParent(parent);
		return local_root;
	}
	// 	Creates a leaf node
	else if (numSamples == 1){
		idx = nodes[start].getidx();
		this->nodes[idx].assignChild(-1, 0);
		this->nodes[idx].assignChild(-1, 1);
		this->nodes[idx].assignParent(parent);
		return idx;
	}
	else if (numSamples == 0){
		return -1;
	}
};

// Returns a node
kdnode kdtree::getNode(int idx){
	return this->nodes[idx];
};
// Returns the value of a node in tree
double kdtree::getPointVal(int idx, int level){
	return this->nodes[idx].getPointVal(level);
};
// Recursive tree printing 
void kdtree::treeWalk(int idx, int level){
	assert(this->dim == 2);
	int i = 0;
	std::string tab = "";
	std::string str = "    ";
	for(i = 0; i < level; i++){
		tab.append(str);
	}
	if (this->nodes[idx].getChild(0) != -1){
		int leftChild = this->nodes[idx].getChild(0);
		printf("%s(%1.2f,%1.2f):\n", tab.c_str(), (*this).getPointVal(idx,0), (*this).getPointVal(idx,1));
		printf("%shas LeftChild: (%1.2f,%1.2f)\n", tab.c_str(), (*this).getPointVal(leftChild,0), (*this).getPointVal(leftChild,1));
		treeWalk(leftChild, level+1);
	}
	else{
		printf("%s(%1.2f,%1.2f):\n", tab.c_str(), (*this).getPointVal(idx,0), (*this).getPointVal(idx,1));
		printf("%shas NO LeftChild.\n", tab.c_str());
	}
	if (this->nodes[idx].getChild(1) != -1){
		int rightChild = this->nodes[idx].getChild(1);
		printf("%s(%1.2f,%1.2f):\n", tab.c_str(), (*this).getPointVal(idx,0), (*this).getPointVal(idx,1));
		printf("%shas rightChild: (%1.2f,%1.2f)\n", tab.c_str(), (*this).getPointVal(rightChild,0), (*this).getPointVal(rightChild,1));
		treeWalk(rightChild, level+1);
	}
	else{
		printf("%s(%1.2f,%1.2f):\n", tab.c_str(), (*this).getPointVal(idx,0), (*this).getPointVal(idx,1));
		printf("%shas NO RightChild.\n", tab.c_str());
	}
	
};

// Public tree printing function 
void kdtree::printTree(){
	printf("Root Node: (%1.2f,%1.2f)\n\n", (*this).getPointVal(this->root,0), (*this).getPointVal(this->root,1));
	int level = 0;
	treeWalk(this->root, level+1); 
};

// Public nearest neighbor function
std::vector<int> kdtree::getNN(int numNeigh, std::vector<double> point, int dim){
	assert(this->dim == dim);
	return kdtree::nearestNeighbors(numNeigh, point);
};

// Public nearest neighbor function
std::vector<int> kdtree::getNN(int numNeigh, int idx){
	assert(idx > 0 && idx < this->nodes.size());
	return kdtree::nearestNeighbors(numNeigh, this->nodes[idx].getPointVal());
};

// Private neighbor function
std::vector<neighbor> kdtree::nearestNeighbors(int numNeigh, std::vector<double> point){
	std::vector<neighbor> neighbors[numNeigh];
	
};

int getNearestLeaf(std::vector<double> point, int idx){
	if (this->nodes[idx].isLeaf()){
		return idx;
	}
	bool greaterThan = point[this->nodes[idx].getLevel()] >= this->nodes[idx].getPointVal(this->nodes[idx].getLevel());
	else{
		if ( greaterThan ){ return this->nodes[idx].getChild(1); }
		else{ return this->nodes[idx].getChild(0); }
	}
};

double kdtree::distsquared(std::vector<double> a, std::vector<double> b){
	assert(a.size() == b.size());	
	double distsquared = 0;
	int i;
	for (i = 0; i < a.size(); i++){
		distsquared += std::pow( (a[i] - b[i]), 2.0);
	}
	return distsquared;
};

int kdtree::getOtherChild(int parent, int child){
	int leftChild = this->nodes[parent].getChild(0);
	int rightChild = this->nodes[parent].getChild(1);
	assert( child == leftChild || child == rightChild );
	if (child == leftChild){ return rightChild; }
	else{ return leftChild; }
};

void kdtree::traverseUp(std::vector<double> point, std::vector<int> neighbors, int node_idx){
	int parent = this->nodes[node_idx].getParent();
	if (parent != -1){
		int level = this->nodes[parent].getLevel();
		if ( kdtree::distsquared(this->nodes[parent].getPointVal(), point) < max(neighbors) ){
			insert(neighbors, parent);
		}
		distToHP = std::abs( this->nodes[parent].getPointVal(level) - point[level] );
		if ( distToHP < max(neighbors) ){
			traverseUp(point, neighbors, this->getNearestLeaf( point, this->getOtherChild(parent, node_idx) ));
		}
		traverseUp(point, neighbors, parent);
	}
	return neighbors;
};
