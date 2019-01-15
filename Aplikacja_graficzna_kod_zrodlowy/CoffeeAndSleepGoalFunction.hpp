#ifndef COFFEEANDSLEEPGOALFUNCTION_HPP
#define COFFEEANDSLEEPGOALFUNCTION_HPP
#include"GoalFunction.hpp"
#include "Coffee.hpp"
#include "Sleep.hpp"
#include <iostream>

class CoffeeAndSleepGoalFunction : public GoalFunction
    {
    private:
        double multiplier_integral;
        double multiplier_coffee;
        mutable double max_possible;

        void countMaxPossible() const;
        static double countLocalIntegral(Factors& previous_end_factors, int previous_end_time, int current_start_time) ;
        static Factors factorsAt(int time,const Factors& previous_factors,int previous_end);

    public:
        CoffeeAndSleepGoalFunction(double multi_integral = 1.0, double multi_coffee = 1.0) : multiplier_integral(multi_integral),multiplier_coffee(multi_coffee){void countMaxPossible();}

        double goalFunction(const std::vector<Action*>* collection, const Factors& start_factors) const;
        Factors getFactorsAt(int time, const Factors& start_factors_, const std::vector<Action*>* collection) const;

        void setIntegral_Multi(double);
        void setCoffee_Multi(double);
    };

#endif // COFFEEANDSLEEPGOALFUNCTION_HPP
