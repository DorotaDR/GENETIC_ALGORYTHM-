#ifndef CHROMOSOME_HPP
#define CHROMOSOME_HPP
#include<string>
#include<ostream>

class Chromosome
    {
    public:
        virtual Chromosome* crossingOver(const Chromosome*) const = 0;
        virtual Chromosome* mutation() const = 0;
        virtual double goalFunction()const = 0;
        virtual Chromosome* randomChromosome() const = 0;
        virtual std::string toString() const = 0;
        virtual Chromosome* clone() const = 0;

        friend std::ostream& operator<<(std::ostream& out, const Chromosome& ch);

        virtual ~Chromosome(){}
    };

#endif // CHROMOSOME_HPP
