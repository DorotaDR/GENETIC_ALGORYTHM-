#ifndef DWIEFUNKCJE_HPP
#define DWIEFUNKCJE_HPP

#include"Chromosome.hpp"
#include "generateRandom.hpp"
#include <ostream>
#include <sstream>
#include <vector>
#include <iostream>
#include <climits>
#include <cmath>
#define TYP_MIN INT_MIN
#define TYP int

 class DwieFunkcje : public Chromosome
    {
    private:
        static const TYP begA = 1;
        static const TYP begB = 0;        static const TYP endAB = 13;
        static const int max_size = 120;
        mutable TYP A = begA;
        mutable TYP B = begB;

        mutable TYP value = 0;

        std::vector<bool> ciag;

        void update() const;

    public:
        DwieFunkcje(){}
        DwieFunkcje(const DwieFunkcje& other);

        Chromosome* crossingOver(const Chromosome*) const;
        Chromosome* mutation() const;
        double goalFunction()const;
        Chromosome* randomChromosome() const;
        std::string toString() const;
        Chromosome* clone() const;

        DwieFunkcje& operator=(const DwieFunkcje& other);

        ~DwieFunkcje();
    };



#endif // DWIEFUNKCJE_HPP
