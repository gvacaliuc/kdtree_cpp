#ifndef KDNODE_H
#define KDNODE_H

#include <vector>

class kdnode{
	public:
		kdnode();
		kdnode(std::vector<double> pt, int id, int d);
		int getParent();
		int getChild(int direction); // 0 for left child, 1 for right
		void assignParent(int p);
		void assignChild(int c, int direction); // 0 for left child, 1 for right
		int getidx();
		double getPointVal(int level);
		std::vector<double> getPointVal();
		void printPointVal();
		int getdim();
		void setLevel(int level);
		bool isLeaf();
		int getLevel();
	private:
		std::vector<double> point;
		int parent;
		int left;
		int right;
		int idx;
		int dim;
		int level;
};
#endif
