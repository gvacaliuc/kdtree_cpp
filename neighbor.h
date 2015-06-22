#ifndef NEIGHBOR_H
#define NEIGHBOR_H

#include <vector>
#include "kdtree.h"

class neighbor{
	public:
		neighbor();
		void setIdx(int idx);
		void setDist(double dist);
		void setPoint(std::vector<double> point);
		int getIdx();
		double getDist();
		std::vector<double> getPoint();
	private:
		int idx;
		double dist;
		std::vector<double> point;
};
class neighborArray{
	public:
		neighborArray();
		neighborArray(int n);
		void insert(neighbor n);
		double getMaxDist();
		int getMinIdx();
		neighbor &operator[](int i);
		int size();
	private:
		std::vector<neighbor> neighbors;
		void sortNeighbors();
};

#endif
