#include "pathfinding.h"

NODE aStar(NODE* start, NODE* end){
	NODE n;

	int dX = end->nodeX - start->nodeX;
	int dY = end->nodeY - start->nodeY;
	
	double iY = (double)(dY/dX);
	double iX = (double)(dX/dY);
	
	double nX = (double)start->nodeX + iX;
	double nY = (double)start->nodeY + iY;
	
	n.nodeX = (int)nX;
	n.nodeY = (int)nY;
	
	return n;
}
