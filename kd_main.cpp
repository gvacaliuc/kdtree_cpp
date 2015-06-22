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
	
	cnpy::NpyArray arr = cnpy::npy_load("kd_data.npy");
	double* arr_data = reinterpret_cast<double*>(arr.data);
	int i, j;
/*	std::vector<unsigned int> shape;	//If data stored correctly
	shape = arr.shape;
	int a = shape[0], b = shape[1];
	const int dim = a, numSamples = b;*/
	const int dim = 2, numSamples = 6;
	std::vector<double> temp (dim);   // used to put values in nodes
	kdnode node_data[numSamples];

//  Initializes arrayOfNodes
	for (i = 0; i < numSamples; i++){
		for (j = 0; j < dim; j++){
			temp[j] = arr_data[i*dim + j];
		}
		node_data[i] = kdnode(temp, i, dim);
	}
//  Printing node vectors for debugging
	for (i = 0; i < numSamples; i++){
		for(j = 0; j < dim; j++){
//			printf("Entry(%u,%u): %2.0f\n", j+1, i+1, node_data[i].getPointVal(j) );
		}
	} 
	kdtree tree;
	tree = kdtree(node_data, numSamples, node_data[0].getdim());
	
//  Printing tree node vectors for debugging
	for (i = 0; i < numSamples; i++){
		for(j = 0; j < dim; j++){
	//		printf("Node: %u, Entry(%u): %2.0f\n", i+1, j+1, tree.getNode(i).getPointVal(j) );
		}
	}
	tree.build(); 
	tree.printTree();
	std::vector<double> point (2,5.0);
	std::vector<int> nn_indices = tree.getNN(4, point, 2);
	for (i = 0; i < 4; i++){
//		tree.getNode(nn_indices[i]).printPointVal();
		printf("Idx %u: %u\n", i+1, nn_indices[i]); 	
	}
	return 0;
}
