#include "Point.hpp"

std::ostream& operator<<(std::ostream& out, Point& p)
	{
	out<<"x = "<<p.getX()<<"\t y = "<<p.getY()<<"\n";

	return  out;
	}
