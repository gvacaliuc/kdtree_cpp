#include <vector>

neighbor::neighbor(){};

neighbor::setData(int idx){
	this->idx = idx;
};

neighbor::setDist(double dist){
	this->dist = dist;
};

neighbor::setPoint(std::vector<double> point){
	this->point = point;
};

neighbor::getIndex(){
	return this->idx;
};

neighbor::getDist(){
	return this->dist;
};

neighbor::getPoint(){
	return this->point;
};

