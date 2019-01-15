#ifndef DAYACTIONS_HPP
#define DAYACTIONS_HPP

class Action;
class DayActions;
class GoalFunction;


#include<stdio.h>
#include<vector>
#include<string>
#include<utility>
#include<memory>
#include <typeinfo>
#include <sstream>
#include <iostream>

#include "Chromosome.hpp"
#include "Action.hpp"
#include "TimeRange.hpp"
#include "GoalFunction.hpp"
#include "Factors.hpp"
#include "generateRandom.hpp"
#include "Point.hpp"


typedef void (*checkingFunction)(std::vector<Action*>*,DayActions*);
typedef std::shared_ptr<const Action> shared_ptr_Action;
typedef std::shared_ptr<Action> shared_Action;
typedef std::pair<std::string, shared_Action (*)(std::istream&) > stringAndParseFunction;


class DayActions : public Chromosome
    {
    protected:
        static const int start_ = 0;
        static const int end_ = 60*24;
        bool modified;
        Factors start_factors;
        std::vector<Action*> collection;
        std::shared_ptr<GoalFunction> goal_function;
        std::vector<shared_ptr_Action> class_types;
        std::vector<checkingFunction> check_functions;
        double goal_function_value;

        void sort();
        static void adjustToNextRange(std::vector<Action*>* vec, TimeRange& previous, TimeRange& next);


    private:
        std::vector<Action*>* getPart(TimeRange&) const;
        void setPart(std::vector<Action*>*,Factors&, TimeRange&);
        bool deleteRange(TimeRange&);

        void checkRestrictionsAndRetake();
        void deleteAllActionsAndGoalFunction();
        void addRandomAction();

    public:
        DayActions(std::shared_ptr<GoalFunction> goalFunction_, const std::vector<shared_ptr_Action>& cl_types, std::vector<checkingFunction> checkFun, const Factors start_factors_);
        explicit DayActions(const DayActions& other);

        void setStartFactors(Factors st_factors = Factors(0,0));

        bool addAction(Action*);
        bool removeAction(Action*);
        void removeAllActions();
        void updateFactors(); //it's also checking restrictions
        void onlyUpdate();
        void setFlagModified();
        void removeFromRange(TimeRange&);
		void addRandAction();//not checking restrictions

        DayActions* replacePart(const DayActions*, TimeRange&) const;
        TimeRange getMaxFreeTimeRange();
		std::vector<Point> getActivityPoints() const;

        DayActions& operator=(const DayActions& other);

        Chromosome* crossingOver(const Chromosome*) const;
        Chromosome* mutation() const;
        double goalFunction()const;
        Chromosome* randomChromosome() const;
        Chromosome* clone() const;
        std::string toString() const;

        virtual DayActions* randomDayActions() const;//v

		static std::shared_ptr<DayActions> createFromFile(std::istream& in, std::vector<stringAndParseFunction> parse_form_and_function, std::shared_ptr<GoalFunction> goalFunction_, const std::vector<shared_ptr_Action>& cl_types, std::vector<checkingFunction>& checkFun );

        virtual ~DayActions();
};

TimeRange findMaxFreeTimeRange(const std::vector<Action*>&, int, int);  //must be sorted and overlapping must be deleted

std::vector< std::shared_ptr<DayActions>> loadFromFile(std::istream& in, std::vector<stringAndParseFunction> parse_form_and_function, std::shared_ptr<GoalFunction> goalFunction_, const std::vector<shared_ptr_Action>& cl_types, std::vector<checkingFunction> checkFun );

#endif
