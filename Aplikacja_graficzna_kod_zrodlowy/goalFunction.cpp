#include "CoffeeAndSleepGoalFunction.hpp"

double CoffeeAndSleepGoalFunction::goalFunction(const std::vector<Action*>* collection, const Factors& start_factors) const
    {
                                                                                                             //double previous_end_y = start_factors.y;
    Factors previous_end_factors(start_factors);   //zamiast dwoch zmiennych jest do tego klasa z mysla o tym stworzona        //double previous_end_alpha = start_factors.alpha;//ALPHA tez potrzebne bo na poczatku snu/kawy nie znamy wartosci y i musimy go obliczac
    double previous_end_time = 0;       //czas poczatku dnia
    double sum = 0;
    int sum_of_coffee = Coffee::getMaxTotalPerDay();
    const int END_TIME_OF_DAY = 24*60;
    double answer = 0.0;
    countMaxPossible();

    for(Action* x : *collection)
        {
        const Coffee* coffee = dynamic_cast<const Coffee*>(x);
        if(coffee != nullptr)
            sum_of_coffee -= coffee->getCoffeeQuantity();    // wartość funkcji celu pochodząca od kawy


        double part = countLocalIntegral(previous_end_factors, previous_end_time, x->getBegin());
        sum = sum + part; //to pozosta³o twoje :)

        previous_end_factors = x->getFactorsAfter(); //ustawiamy wartosci z konca aktualnej czynnosci jako poczatek nowego przedzialu nic nie ronienia ;)
        previous_end_time = x->getEnd();
        }

    if(previous_end_time < END_TIME_OF_DAY) //after the last action (if is not fill bye other actions)
        {
        double part = countLocalIntegral(previous_end_factors, previous_end_time, END_TIME_OF_DAY);
        sum = sum + part;
        }

	answer = multiplier_integral*sum + multiplier_coffee*sum_of_coffee; // counting the answer
	answer = answer / max_possible*100;
    return answer;
    }
