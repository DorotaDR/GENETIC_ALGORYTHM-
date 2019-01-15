#ifndef CIAG_HPP
#define CIAG_HP
#include<string>
#include<vector>
#include<sstream>
#include<stdio.h>
#include "Chromosome.hpp"

class Ciag : public Chromosome
    {
    private:
        int tab[10];
        static const int length = 10;

    public:
        Ciag(){;}
        Ciag(const Ciag&);

        Ciag& operator=(const Ciag& other);

        Chromosome* crossingOver(const Chromosome*) const;
        Chromosome* mutation() const;
        double goalFunction() const;
        Chromosome* randomChromosome() const;
        std::string toString() const;
        Chromosome* clone() const;

        virtual ~Ciag(){;}
    };

#endif // CIAG_HPP
