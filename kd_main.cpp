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

	The code proved far too slow to be useful, so this is the final step with this code.
*/

int main(){
	
	cnpy::NpyArray arr = cnpy::npy_load("icacoffskbh_30.npy");	//	Some computers have trouble using cnpy
	double* arr_data = reinterpret_cast<double*>(arr.data); 
	
	std::vector<unsigned int> shape;	//If data stored correctly
	shape = arr.shape;
	int a = shape[0], b = shape[1], i, j;
	const int dim = a;
	const int numSamples = b;
	
//	Setup for arrayOfNodes
//	const int dim = 2, numSamples = 6;
	std::vector<double> temp (dim);
	kdnode node_data[numSamples];

//  Initializes arrayOfNodes
	for (i = 0; i < numSamples; i++){
		for (j = 0; j < dim; j++){
			temp[j] = arr_data[i + j*numSamples];
		}
		node_data[i] = kdnode(temp, i, dim);
	}

//	Initializes Tree

	kdtree tree;
	tree = kdtree(node_data, numSamples, node_data[0].getdim());
	
//	Builds Tree
	tree.build();
	printf("----- Built Tree -----\n");
//	Prints Tree 
	//tree.printTree();

//	Instantiates a point to find NN of
	int numneigh = 31;
//	Finds indices of tree where the NN are located, nearest first
	neighborArray nn = tree.getNN(numneigh, 24);

// Prints node's point value and index
	for (i = 0; i < numSamples; i++){
		neighborArray nn = tree.getNN(numneigh,i);
		for (j = 0; j < numneigh; j++){
			printf("%f\n", nn[j].getDist() );
			printf("%i\n", nn[j].getIdx() );
		}
	}
	
	return 0;
}
