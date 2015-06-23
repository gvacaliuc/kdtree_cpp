#ifndef KDTREE_H
#define KDTREE_H

#include "kdnode.h"
#include <vector>
#include "neighbor.h"

/*	These functions and variables are better explained in "kdtree.cpp" */

class kdtree{
	public:
		kdtree();
		kdtree(kdnode* arrayOfNodes, int numSamples, int dim);
		int getRootNode();
		void build();
		kdnode getNode(int idx);
		void printTree();
		neighborArray getNN(int numNeigh, std::vector<double>, int dim);
		neighborArray getNN(int numNeigh, int idx);
		static void quickSort(double arr[], int l, int h, int nS);
	private:
		int numSamples;
		int dim;
		std::vector<kdnode> nodes;
		int root; 														
		static void swap(double* a, double* b);
		static int partition (double arr[], int l, int h, int nS);		
		static std::vector<kdnode> sortNodes(std::vector<kdnode> data, int start, int end, int level);
		static int getMedian(int numSamples);
		int treeGen(std::vector<kdnode> nodes, int start, int end, int level, int parent);
		void treeWalk(int idx, int level);
		double getPointVal(int idx, int level);
		int getNearestLeaf(std::vector<double> point, int node_idx);
		static double distsquared(std::vector<double> a, std::vector<double> b);
		int getOtherChild(int parent, int child);
		void traverseUp(std::vector<double> point, neighborArray *neighbors, int now_idx, int from_idx, std::vector<int> roots);
		neighborArray nearestNeighbors(int numNeigh, std::vector<double> point);
		neighborArray initNeighbors(neighborArray neighbors, int numNeigh, std::vector<double> point);
		static bool vector_contains(std::vector<int> roots, int node);
		std::vector<int> vector_remove(std::vector<int> roots, int node);
		static std::vector<int> vector_insert(std::vector<int> roots, int node);
};
#endif
