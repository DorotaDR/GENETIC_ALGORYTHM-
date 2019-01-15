#ifndef GOALFUNCTION_HPP
#define GOALFUNCTION_HPP
#include<vector>
#include "Action.hpp"
#include "Factors.hpp"

class GoalFunction
    {
    public:
        virtual double goalFunction(const std::vector<Action*>* collection, const Factors& start_factors) const  = 0;
        virtual Factors getFactorsAt(int time, const Factors& start_factors_, const std::vector<Action*>* collection) const = 0;
    };

#endif // GOALFUNCTION_HPP
