#include "Chromosome.hpp"

std::ostream& operator<<(std::ostream& out, const Chromosome& ch)
    {
    out<<(ch.toString());
    return out;
    }
