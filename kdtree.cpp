#include <stdio.h>
#include "kdnode.h"
#include "kdtree.h"
#include <cmath>
#include <vector>
#include <string>
#include <cstring>
#include "assert.h"
#include "neighbor.h"

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

	- Gabriel Vacaliuc - edited - 06/22/15
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
		quickSort(arr, 0, numSamples - 1, numSamples);	// Modified quicksorter to do argument sorting

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
		this->nodes[idx].setLevel(level);
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

// Private recursive tree printing 
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
	assert(this->dim == dim && point.size() == dim && numNeigh < this->nodes.size());
	int i;	
	neighborArray neighbors;

//	Calls private NN function
	neighbors = this->nearestNeighbors(numNeigh, point );
	std::vector<int> indices (numNeigh);

	for (i = 0; i < numNeigh; i++){
		indices[i] = neighbors[i].getIdx();
	}

	return indices;
};

// Public nearest neighbor function
std::vector<int> kdtree::getNN(int numNeigh, int idx){
	assert(idx >= 0 && idx < this->nodes.size() && numNeigh < this->nodes.size());
	neighborArray neighbors;
	int i;
	printf("1\n");
//	Calls private NN function
	neighbors = this->nearestNeighbors(numNeigh, this->nodes[idx].getPointVal() );
	std::vector<int> indices (numNeigh);
	printf("2\n");
	for (i = 0; i < numNeigh; i++){
		indices[i] = neighbors[i].getIdx();
	}

	return indices;
};

// Private neighbor function
neighborArray kdtree::nearestNeighbors(int numNeigh, std::vector<double> point){
	neighborArray neighbors;
	neighbors = neighborArray(numNeigh);
	int i;
	printf("3\n");
	//	Gets a 'best-guess' leaf
	int leaf = this->getNearestLeaf(point, this->getRootNode());
	printf("4\n");
	//	Initializes the nearest neighbors array as the first N neighbors in tree
	neighbors = this->initNeighbors(neighbors, numNeigh, point);
	printf("5\n");
	//	Calls recursive function and passes a reference to neighbors
	this->traverseUp(point, &neighbors, leaf, -1, this->getRootNode() );
	printf("6\n");
	return neighbors;
};

//	Returns if is leaf, otherwise finds the correct side of splitting Hyper-
//	Plane (favoring the right).
int kdtree::getNearestLeaf(std::vector<double> point, int idx){
	if (this->nodes[idx].isLeaf()){	
		return idx;
	}
	else{
		bool greaterThan = point[this->nodes[idx].getLevel()] >= this->nodes[idx].getPointVal(this->nodes[idx].getLevel());
		if ( greaterThan && this->nodes[idx].getChild(1) != -1){ return getNearestLeaf(point, this->nodes[idx].getChild(1)); }
		else{ return getNearestLeaf(point, this->nodes[idx].getChild(0)); }
	}
};

//	Returns the distance squared between two points (so we don't have to do root finding)
double kdtree::distsquared(std::vector<double> a, std::vector<double> b){
	assert(a.size() == b.size());	
	double distsquared = 0;
	int i;
	for (i = 0; i < a.size(); i++){
		distsquared += std::pow( (a[i] - b[i]), 2.0);
	}
	return distsquared;
};

//	Returns the idx of a parent's other child given the parent and one child
int kdtree::getOtherChild(int parent, int child){
	int leftChild = this->nodes[parent].getChild(0);
	int rightChild = this->nodes[parent].getChild(1);
	assert( child == leftChild || child == rightChild );
	if (child == leftChild){ return rightChild; }
	else{ return leftChild; }
};

//	Recursive function to find nearest neighbors, neighbors points to the memory allocated for the NN
void kdtree::traverseUp(std::vector<double> point, neighborArray *neighbors, int now_idx, int from_idx, int root_idx){
	int node = now_idx,i;
	double dist;
	printf("node: %u\n", node);
	printf("root: %u\n", root_idx);
	printf("7\n");
//	Checks to make sure node passed is not the local root
	if (node != root_idx){
		int level = this->nodes[node].getLevel();
		printf("level: %u\n", level);
		dist = kdtree::distsquared(this->nodes[node].getPointVal(), point);	
		printf("max dist: %f\n", neighbors->getMaxDist() );
		printf("8\n");
	//	Adds node to neighbors if the the distance from node to point is lower than the max distance in neighbors
		if ( dist < neighbors->getMaxDist() ){
			//printf("1\n");
			neighbor n;
			n.setIdx(node);
			n.setDist(dist);
			n.setPoint(this->nodes[node].getPointVal());
			neighbors->insert(n);
			printf("9\n");
		}
		double distToHP = std::pow( (this->nodes[node].getPointVal(level) - point[level]), 2.0 );
		printf("disttohp2: %f\n", distToHP );

	//	Checks the other subtree of node if the largest distance is greater than the distance to the hyperplane
		if ( distToHP < neighbors->getMaxDist() && from_idx != -1 && this->getOtherChild(node, from_idx) != -1){
			//printf("2\n");
			printf("10\n");
			traverseUp(point, neighbors, this->getNearestLeaf( point, this->getOtherChild(node, from_idx) ), -1, node);
		}

	//	If node is not global root and not local root move up a level
		if ( node != this->getRootNode() ){
		//printf("3\n");
		printf("11\n");
		traverseUp(point, neighbors, this->nodes[node].getParent(), node, root_idx);
		}
	}
//	If node is not global root but is local root then move up a level and set root node as global root
	else if (node != this->getRootNode()){
		//printf("4\n");
		printf("12\n");
		traverseUp(point, neighbors, this->nodes[node].getParent(), node, this->getRootNode() ); 
	}
//	If node is global root
	else if (node == this->getRootNode()){
		printf("13\n");
		dist = kdtree::distsquared(this->nodes[node].getPointVal(), point);
		//printf("max dist: %f\n", neighbors->getMaxDist() );
		//printf("dist: %f\n", dist );

	//	If root is closer than the furthest neighbor		
		if ( dist < neighbors->getMaxDist() ){
			//printf("1\n");
			neighbor n;
			n.setIdx(node);
			n.setDist(dist);
			n.setPoint(this->nodes[node].getPointVal());
			neighbors->insert(n);
		}
	}
};

//	Initializes a neighborArray of numNeigh length with the first k-NN 
neighborArray kdtree::initNeighbors(neighborArray neighbors, int numNeigh, std::vector<double> point){
	int i;
	double dist;
	for (i = 0; i < numNeigh; i++){
		neighbor n;
		n.setIdx(i);
		dist = kdtree::distsquared(this->nodes[i].getPointVal(), point);
		n.setDist(dist);
		n.setPoint(this->nodes[i].getPointVal());
		//printf("idx %u: %u\n", i, n.getIdx());
		//printf("dist %u: %f\n", i, n.getDist());
		neighbors.insert(n);
	}
	return neighbors;
};
