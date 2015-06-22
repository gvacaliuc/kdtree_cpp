#ifndef KDTREE_H
#define KDTREE_H

#include "kdnode.h"
#include <vector>

class kdtree{
	public:
		kdtree();
		kdtree(kdnode* arrayOfNodes, int numSamples, int dim);
		int getRootNode();
		//getLeaves(); //needed?
		void build();
		kdnode getNode(int idx);
		void printTree(); //Public function to print out tree (2-Dim REQ!)
		std::vector<int> getNN(int numNeigh, std::vector<double>, int dim);	//Nearest Neighbors of an input vector
		std::vector<int> getNN(int numNeigh, int idx);						//Nearest Neighbors of node in tree
	private:
		int numSamples;
		int dim;
		std::vector<kdnode> nodes;
		int root; //idx for root node
		static void swap(double* a, double* b); //These three do an argsort on the data
		static int partition (double arr[], int l, int h, int nS);
		static void quickSort(double arr[], int l, int h, int nS);
		static std::vector<kdnode> sortNodes(std::vector<kdnode> data, int start, int end, int level); //Calls above static functions
		static int getMedian(int numSamples); //simple helper function
		int treeGen(std::vector<kdnode> nodes, int start, int end, int level, int parent);
		void treeWalk(int idx, int level);
		double getPointVal(int idx, int level);
		std::vector<int> nearestNeighbors(int numNeigh, std::vector<double> point);
		int getNearestLeaf(std::vector<double> point, int node_idx);
		static double distsquared(std::vector<double> a, std::vector<double> b);
		int getOtherChild(int parent, int child);
};
#endif
