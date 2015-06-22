#include <stdio.h>
#include <assert.h>
#include "cnpy.h"
#include "kdnode.h"
#include <vector>
#include "kdtree.h"
#include "neighbor.h"

/*	Primitive main class that reads numpy file in, creates and prints tree.
	Eventually this will all be wrapped in C, then fortran, then compiled as a
	shared object that is readable/implementable (words?) in python.

	- Gabriel Vacaliuc - edited - 06/22/15
*/

int main(){
	
	/*cnpy::NpyArray arr = cnpy::npy_load("kd_data.npy");	//	Some computers have trouble using cnpy
	double* arr_data = reinterpret_cast<double*>(arr.data); */
	
	int i, j;
	double arr_data[12] = {2,3,5,4,9,6,4,7,8,1,7,2};
/*	std::vector<unsigned int> shape;	//If data stored correctly
	shape = arr.shape;
	int a = shape[0], b = shape[1];
	const int dim = a, numSamples = b;*/

//	Setup for arrayOfNodes
	const int dim = 2, numSamples = 6;
	std::vector<double> temp (dim);
	kdnode node_data[numSamples];

//  Initializes arrayOfNodes
	for (i = 0; i < numSamples; i++){
		for (j = 0; j < dim; j++){
			temp[j] = arr_data[i*dim + j];
		}
		node_data[i] = kdnode(temp, i, dim);
	}

//	Initializes Tree

	kdtree tree;
	tree = kdtree(node_data, numSamples, node_data[0].getdim());
	
//	Builds Tree
	tree.build();

//	Prints Tree 
	tree.printTree();

//	Instantiates a point to find NN of
	std::vector<double> point (2);
	point[0] = 6.0;
	point[1] = 2.0;

//	Finds indices of tree where the NN are located, nearest first
	std::vector<int> nn_indices = tree.getNN(4, point, 2);

// Prints node's point value and index
	for (i = 0; i < 4; i++){
		tree.getNode(nn_indices[i]).printPointVal();
		printf("Index %u: %2u\n", i+1, nn_indices[i]); 	
	}
	return 0;
}
