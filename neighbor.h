#ifndef NEIGHBOR_H
#define NEIGHBOR_H

#include <vector>

class neighbor{
	public:
		neighbor();
		void setIndex(int idx);
		void setDist(double dist);
		void setPoint(std::vector<double> point);
		int getIndex();
		double getDist();
		std::vector<double> getPoint();
	private:
		int idx;
		double dist;
		std::vector<double> point;
};
