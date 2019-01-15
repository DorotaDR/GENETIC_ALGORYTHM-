#ifndef FACTORS_HPP
#define FACTORS_HPP
#include "generateRandom.hpp"
#include <cmath>
#include <ostream>
#include<iomanip>


/* ---------- FACTORS -----------*/

class Factors {
private:
	double y;
	double a;

public:

    const static double MAX_Y;		// max watosc wspólczynnika y


	Factors();											//domyslnie y =0 oraz a =0
	Factors(const Factors&);									// konstruktor kopiujacy
	Factors(double _y, double _alpha);					// ustawia wartosci przy tworzeniu obiektu

	//GET FUNCTIONS:
	double getY() const;
	double getA() const;


	//SET FUNCTIONS:
    void setY (double _y);
    void setA (double _a);

	Factors& operator=(const Factors&);
    friend std::ostream& operator<<(std::ostream& out, const Factors& obj);
	friend bool operator>>(std::istream& in, Factors& factors);
};

std::ostream& operator<<(std::ostream& out, const Factors& obj);
bool operator>>(std::istream& in, Factors& factors);


#endif // FACTORS_HPP
