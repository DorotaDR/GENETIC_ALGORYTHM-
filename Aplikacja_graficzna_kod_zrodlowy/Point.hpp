#ifndef POINT_HPP
#define POINT_HPP
#include <ostream>

class Point
	{
	private:
		int x = 0;
		double y = 0;

	public:
		 Point()
			 {}

		 Point(int x_, double y_) : x(x_), y(y_)
			 {}

		 int& getX()
			 {
			 return  x;
			 }

		 double& getY()
			 {
			 return y;
			 }

		 friend std::ostream& operator<<(std::ostream& out, Point& p);
	};


std::ostream& operator<<(std::ostream& out, Point& p);

#endif // POINT_HPP
