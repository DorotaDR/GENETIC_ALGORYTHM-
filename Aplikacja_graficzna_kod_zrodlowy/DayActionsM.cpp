#include "DayActions.hpp"

void DayActions::sort() //setting and removing flags
    {
    if(collection.empty())
        return;

    bool flag = false;

    for(int j = collection.size()-1; j > 0; --j)
        {
        for(int i = 0; i < j; i++) //i-te pozycje to od 0 do j-otej
            {
            int z = collection[i]->isEqual(collection[i+1]);

            if(z > 0)// jesli aktualna i-ta pozycja jest wieksza od i+1-szej
                {   //zamieniamy je
                Action* temporary = collection[i];
                collection[i] = collection[i+1];
                collection[i+1] = temporary;
                flag = true;    //ustwienie flagi gdy nast¹pi zmiana
                }
            else if(z == 0)
                {
                if(collection[i]->getBegin() >= collection[i+1]->getBegin() && collection[i]->getEnd() <= collection[i+1]->getEnd())
                    {//i is contained in i+1
                    delete collection[i];
                    collection[i] = nullptr;
                    collection.erase(collection.begin()+ i);
                    --i;
                    --j;
                    }
                else if(collection[i]->getBegin() <= collection[i+1]->getBegin() && collection[i]->getEnd() >= collection[i+1]->getEnd())
                    {//i+1 is contained in i
                    delete collection[i+1];
                    collection[i+1] = nullptr;
                    collection.erase(collection.begin()+ i+1);
                    --j;
                    }
                else if(collection[i]->getBegin() >= collection[i+1]->getBegin())//only overlapping
                    {
                    Action* temporary = collection[i];
                    collection[i] = collection[i+1];
                    collection[i+1] = temporary;
                    }
                }
            }

        if(!flag)   //gdy flaga nie ustawiona konczymy sortowanie
            break;

        modified = true;        //setting modified

        flag = false;   //reset flagi, gdy jest ustawiona dla nastêpnej pêtli
        }

    //deleteOverlapping();
    }



DayActions* DayActions::replacePart(const DayActions* other, TimeRange& range) const
    {
    if(this->class_types != other->class_types)     //class types must be the same
        throw std::string("\nObjects are contain different Action class types objects. Replace is impossible!\n");

    if(range.getBegin() == range.getEnd())  //it's pointed range
        return new DayActions(*this);

    DayActions* ans;
    const DayActions* second;
    std::vector<Action*>* part;
    Factors part_start_factors;

    if(generateRandom(1))    //if answer is based on first
        {
        //std::cout<<"Base from rDA1\n";
        ans = new DayActions(*this);
        second = other;
        }
    else            //if answer is based on second
        {
        //std::cout<<"Base from rDA2\n";
        ans = new DayActions(*other);
        second = this;
        }

    part = second->getPart(range);        //getting part from second
    part_start_factors = second->goal_function->getFactorsAt(range.getBegin(),second->start_factors,&(second->collection)); //pasting it to first at the range

    if(!ans->deleteRange(range))      //problems with deleting
        {
        *ans = *this;
        return ans;
        }

    ans->setPart(part, part_start_factors, range);

    ans->sort();
    ans->updateFactors();

    return ans;
    }

Chromosome* DayActions::crossingOver(const Chromosome* other) const
    {
    const DayActions* second = dynamic_cast<const DayActions*>(other);

    if(!second)
        {
        DayActions* ans = new DayActions(*this);
        return ans;
        }

    int max_length_to_cross = end_/3;           //33% of a day
    int min_length_to_cross = 60;           //1 hour
    int length_to_cross = generateRandom(min_length_to_cross, max_length_to_cross);       //random length crossed time range

    TimeRange range = TimeRange::randomTimeRange(start_,end_, length_to_cross);
    //std::cout<<"Range: "<<range<<"\n";
    return replacePart(second,range);      //replace at the range
    }

Chromosome* DayActions::mutation() const
    {
    DayActions* ans = new DayActions(*this);    //our new object to mutation
    int max_length_to_mutation = 3*60;           //3 hours
    int min_length_to_mutation = 60;           //1 hour
    int length_to_mutation = generateRandom(min_length_to_mutation,max_length_to_mutation);       //random length crossed time range
    std::vector<Action*>* help_vector1, *help_vector2;
    TimeRange t1, t2;

    t1 = TimeRange::randomTimeRange(start_, end_, length_to_mutation);    //random time ranges
    t2 = TimeRange::randomTimeRange(start_, end_, length_to_mutation);

    while(t1.isEqual(t2) == 0)      //first should not contain second
        t2 = TimeRange::randomTimeRange(start_, end_, length_to_mutation);

    Factors factors_1 = goal_function->getFactorsAt(t1.getBegin(),start_factors,&collection);    // taking factors at point at time of begin range
    Factors factors_2 = goal_function->getFactorsAt(t2.getBegin(),start_factors,&collection);

    help_vector1 = getPart(t1);   //taking part from range
    help_vector2 = getPart(t2);

    adjustToNextRange(help_vector1,t1,t2); //setting new times in actions
    adjustToNextRange(help_vector2,t2,t1);

    if(!ans->deleteRange(t1) || !ans->deleteRange(t2)) // error from deleting
        {
        for(Action* x : *help_vector1)
            delete x;
        for(Action* x : *help_vector2)
            delete x;

        *ans = *this;
        return ans;
        }

    ans->setPart(help_vector1,factors_1,t2);    //paste rescaled actions to opposite ranges
    ans->setPart(help_vector2,factors_2,t1);

    if(generateRandom(2))   //probability of adding new action is 0.5
        ans->addRandomAction();

    if(!collection.empty() && generateRandom(2))    //deleting random action
        {
        int nr = generateRandom(collection.size()-1);

        delete ans->collection[nr];
        ans->collection.erase(ans->collection.begin() + nr);
        }

    ans->sort();

    ans->updateFactors();


//std::cout<<"Mutation:\nTR1: "<<t1<<"\nTR2: "<<t2<<"\nis: "<<"\n";
    return ans;
    }

void DayActions::adjustToNextRange(std::vector<Action*>* vec, TimeRange& previous, TimeRange& next)
    {
    int added_time = next.getBegin() - previous.getBegin();

    for(Action* x : *vec)
        x->setBeginEnd(x->getBegin() + added_time, x->getEnd() + added_time);
    }

std::vector<Action*>* DayActions::getPart(TimeRange& range) const
    {
    if(range.getBegin() == range.getEnd())
        return new std::vector<Action*>;

    std::vector<Action*>* part = new std::vector<Action*>;

    for(Action* x : collection)
        {
        if(x->getEnd() < range.getBegin())  //action before the range
            {
            continue;
            }
        else if(x->getBegin() >= range.getBegin() && x->getEnd()< range.getEnd())  //1
            {
            part->push_back(x->clone());
            }
        else if(x->getBegin() <= range.getBegin() && range.getBegin() <= x->getEnd())        //2
            {
            //in part it belongs to range -> left end
            if(range.getEnd() <= x->getEnd())    //x contain whole range with range's ends
                {
                Action* aux = x->clone();
                aux->setBeginEnd(range.getBegin(),range.getEnd());
                part->push_back(aux);
                return part;
                }
            else    //x->getEnd()< range.end   //range contain part with end of x and perhaps something more
                {
                Action* aux = x->clone();
                aux->setBegin(range.getBegin());
                part->push_back(aux);
                continue;   //part from first action go to next
                }
            }
        else if(x->getBegin() < range.getEnd() &&  range.getEnd() <= x->getEnd())        //30
            {
            //in part it belongs to range -> right end
            //range.begin < x->getBegin()   //range contain part with begin of x and perhaps something more

            Action* aux = x->clone();
            aux->setEnd(range.getEnd());
            part->push_back(aux);

            return part;  //it's last possible action part
            }
        else if(range.getEnd() <= x->getBegin())
            break;  //after last possible action
        }

    return part;
    }

bool DayActions::deleteRange(TimeRange& range)  //setting flag modified
    {
    if(range.getBegin() == range.getEnd())
        return true;

    bool ans = true;
    int first = 0, num_to_erase = 0;

    for(unsigned int i = 0; i < collection.size() ; ++i)
        {
        Action* x = collection[i];

        if(x->getEnd() < range.getBegin())
            {
            continue;
            }
        else if(range.getBegin() <= x->getBegin() && x->getEnd()< range.getEnd())  //full x action is containing at range
            {
            delete x;
            if(num_to_erase++ == 0)
                {
                first = i;    //adding index to later delete from collection a pointer value
                }
            }
        else if(x->getBegin() < range.getBegin() && range.getBegin() < x->getEnd())        //2
            {
            //in part it belongs to range -> left end
            if(range.getEnd() < x->getEnd())    //x contain whole range with range's ends => dividing this action for 2 actions
                {
                Action* second = x->divideByRange(range);

                if(second != nullptr)
                    collection.push_back(second);   //adding second part of x

                break;  //end of range
                }
            else    //x->getEnd()<= range.end   //range contain part of x from the interior of x to end of x and can have more actions
                {
                TimeRange tr(range.getBegin(),x->getEnd());
                Action* second = x->divideByRange(tr);

                if(second != nullptr)       //second should be nullptr, but if it's not
                    ans = false;
                }
                //it's not end, there can be some actions.
            }
        else if(x->getBegin() < range.getEnd() && x->getEnd() >= range.getEnd())        //3
            {
            //in part it belongs to range -> right end
            if(x->getBegin() < range.getBegin() )//x contain whole range with range's ends => dividing this action for 2 actions
                {
                Action* second = x->divideByRange(range);

                if(second != nullptr)
                    collection.push_back(second);   //adding second part of x

                break;  //end of range
                }
            else    //range.begin <= x->getBegin()   //range contain part with begin of x and but range end is less than x.end
                {
                TimeRange tr(x->getBegin(),range.getEnd());
                Action* second = x->divideByRange(tr);

                if(second != nullptr)       //second should be nullptr, but if it's not
                    ans = false;
                }
            break;  //it was last possible action part
            }
        else if(range.getEnd() <= x->getBegin())
            break;  //after last possible action
        }

    if(num_to_erase)    //is >0
        {
        collection.erase(collection.begin()+first, collection.begin()+first+num_to_erase);
        }

    modified = true;        // setting and removing flags

    return ans;
    }

void DayActions::setPart(std::vector<Action*>* part, Factors& start_part_factors, TimeRange& range)   //it's delete the memory allocated for part - vector but not it's elements. they are copping to collection. collection is not sorted.
    {   //setting flag modified
    if(range.getBegin() == range.getEnd())
        return ;

    if(range.getBegin() == 0)
        start_factors = start_part_factors;

    for(Action* x : *part)
        {
        collection.push_back(x);
        }

    part->clear();

    delete part;

    modified = true;        //setting and removing flags
    }

Chromosome* DayActions::randomChromosome() const
    {
    return randomDayActions();
    }

void DayActions::updateFactors()   //setting flag
    {
	if(start_factors.getA() > -0.1)
		start_factors.setA(-0.1);

    if(modified)        //if some modifications
        checkRestrictionsAndRetake();

    for(Action* x : collection)
        x->update(&collection, start_factors);

    goal_function_value = goalFunction();
    }

void DayActions::checkRestrictionsAndRetake()   //removing modified
    {
    sort();

    for(checkingFunction x : check_functions)
        if(x != nullptr)
            x(&collection,this);

    modified = false;   //setting flag
    }

bool DayActions::removeAction(Action* action)   //setting flag modified
    {
    for(unsigned int i = 0; i < collection.size(); ++i)
        if(action == collection[i])
            {
            modified = true;    // setting flag
            collection.erase(collection.begin()+i);
            onlyUpdate();    //updating

            return true;
            }

    return false;
    }

double DayActions::goalFunction() const   //return value of goal function
    {
    if(modified)        //if was some modification
        throw(std::string("Call to double \'DayAction::goalFunction() const\' without update."));//updateFactors();

    return goal_function->goalFunction(&collection,start_factors);
    }

void DayActions::deleteAllActionsAndGoalFunction()
    {
    for(Action* x : collection) //deleting all Actions
        delete x;
    collection.clear();

    class_types.clear();

    check_functions.clear();

    goal_function = nullptr;

    modified  = true;       //setting and removing flags
    }

DayActions::~DayActions()
    {
    deleteAllActionsAndGoalFunction();
    }

DayActions::DayActions(std::shared_ptr<GoalFunction> goalFunction_, const std::vector<shared_ptr_Action>& cl_types, std::vector<checkingFunction> checkFun, const Factors start_factors_) : start_factors(start_factors_)
    {
    if(cl_types.empty())
        throw std::string("\ncl_types is empty\n");

    if(!goalFunction_)
        throw std::string("\ngoalFunction is nullptr\n");

    for(shared_ptr_Action x : cl_types)   //checking input vector for nullptr elements
        if(!x) //if object or fun is nullptr
            throw std::string("\ncl_types contain nullptr Action*\n");

    //class_types_and_check_functions = cl_types_checkFun;
    for(shared_ptr_Action x : cl_types)
		class_types.push_back(x);

    check_functions = checkFun;

    goal_function = goalFunction_;

    modified = true;

    goal_function_value = 0.0;
    }

DayActions& DayActions::operator=(const DayActions& other)
    {
    deleteAllActionsAndGoalFunction(); //clearing the vector for new actions

    for(shared_ptr_Action x : other.class_types)
        class_types.push_back(x);

    check_functions = other.check_functions;

    modified = other.modified;
    start_factors  = other.start_factors;

    goal_function = other.goal_function;
    goal_function_value = other.goal_function_value;

    for(Action* x : other.collection)   //cloning actions in the same order
        collection.push_back(x->clone());

    return *this;
    }

DayActions::DayActions(const DayActions& other)
    {
    *this = other;
    }

DayActions* DayActions::randomDayActions() const
    {
	Factors random_start(generateRandom(0,100),generateRandom(-10000,-100)/1000.0); //random start_factors
	DayActions* answer = new DayActions(this->goal_function, this->class_types, this->check_functions,random_start);    //returned object

    if(answer == nullptr)
        throw "Random DayAction is equal nullptr.\n";

    int number_of_actions = generateRandom(3,10);   //number of action added to day

    for(int i = 0; i <number_of_actions ; i++)
		answer->addRandomAction();  //adding random Action

    answer->modified = true;;


    answer->updateFactors();

    return answer;
    }

TimeRange findMaxFreeTimeRange(const std::vector<Action*>& collection, int start, int end)  //must be sorted and overlapping must be deleted
    {                                                                                                   //finding first the biggest range
    if(collection.empty())
        return TimeRange(start,end);

    TimeRange current_max_range(start,collection[0]->getBegin());  //for start to saving the best range
    int previous_end = collection[0]->getEnd();     //ending of previous action

    for(unsigned int i = 1; i < collection.size(); ++i)
        {
        Action* x = collection[i];

        if(x->getBegin() - previous_end > current_max_range.length())   //if this is larger than previous
            {
            current_max_range(previous_end,x->getBegin());
            }

            previous_end = x->getEnd();
        }

    if(end - previous_end > current_max_range.length())   //if this is larger than previous
        {
        current_max_range(previous_end,end);
        previous_end = end;
        }

    return current_max_range;
    }

TimeRange DayActions::getMaxFreeTimeRange()
    {
    if(modified)
        sort();             //sorting and deleting overlapping, because it's outside interface

    return findMaxFreeTimeRange(collection, start_,end_);
    }

void DayActions::addRandomAction()
    {
    TimeRange range = getMaxFreeTimeRange();    //the largest free range
    TimeRange random_part_of_range = TimeRange::randomTimeRange(range.getBegin(),range.getEnd());   // randomizing a part from the largest range
    int random_type_num = generateRandom(0,class_types.size()-1); //choosing type of action

    Action* addedAction = (class_types[random_type_num])->randomAction(random_part_of_range); //creating random action

    modified = true;

    collection.push_back(addedAction);  //adding
    }

Chromosome* DayActions::clone() const
    {
    DayActions* ans = new DayActions(*this);

    return ans;
    }


void DayActions::setStartFactors(Factors st_factors)
    {
	start_factors = st_factors;

    onlyUpdate();
    }

void DayActions::removeAllActions()
    {
    for(Action* x : collection)
        delete x;

    collection.clear();

    onlyUpdate();
    }


void DayActions::onlyUpdate()
    {
	if(start_factors.getA() > 0.1)
		start_factors.setA(0.1);

    sort();
    for(Action* x : collection)
        x->update(&collection, start_factors);

    modified = false;
    goal_function_value = goalFunction();
    modified = true;
    }

void DayActions::removeFromRange(TimeRange& tr)
    {
    deleteRange(tr);
    modified = true;

    onlyUpdate();
    }


void DayActions::setFlagModified()
    {
    modified = true;
    }

std::vector<Point> DayActions::getActivityPoints() const
	{
	std::vector<Point> vec;

	vec.push_back(Point(0, start_factors.getY())); //start point

	int previous_time = start_;
	Factors previous_factors = start_factors;//start previous values

	for(Action* x : collection)
		{
		int TimeBetween = x->getBegin() - previous_time;//times differences
		double action_beg_y = previous_factors.getY() + previous_factors.getA() * TimeBetween;//y at the bginning of action

		if(action_beg_y < 0 && previous_factors.getA() != 0.0)//y<0 at beggining
			{
			double x = (previous_factors.getA()*previous_time-previous_factors.getY())/previous_factors.getA(); //search the time when y = 0
			vec.push_back(Point(x,0));
			action_beg_y = 0.0;
			}

		//vector z action
		std::vector<Point> help = x->getActivityDuring();
		for(Point& x : help)
			vec.push_back(x);

		previous_time = x->getEnd();
		previous_factors = x->getFactorsAfter();
		}



	if(previous_time < end_ )
		{
		int TimeBetween = end_ - previous_time;
		double previous_y = previous_factors.getY() + previous_factors.getA() * TimeBetween;

		if(previous_y < 0  && previous_factors.getA() != 0.0)
			{
			double x = (previous_factors.getA()*previous_time-previous_factors.getY() )/previous_factors.getA(); ;
			vec.push_back(Point(x,0));
			previous_y = 0;
			}

		vec.push_back(Point(end_,previous_y));
		vec.push_back(Point(end_,0));
		}

	return  vec;
	}

std::string DayActions::toString() const
	{
	std::stringstream str;
	str<<"DayActions: \n"<<"Start factors:\n"<<start_factors<<"\nCollection:\n";

	for(Action* x : collection)
		str<<x->toString();

	str<<"goal function value: "<<goal_function_value<<"\n";

	return str.str();
	}

std::shared_ptr<DayActions> DayActions::createFromFile(std::istream& in, std::vector<stringAndParseFunction> parse_form_and_function, std::shared_ptr<GoalFunction> goalFunction_, const std::vector<shared_ptr_Action>& cl_types, std::vector<checkingFunction>& checkFun )
	{
	std::shared_ptr<DayActions> ptr = nullptr;
	Factors st_factors;
	std::string read;
	double val = -1;

	if( in.bad() )
		return nullptr;

	if( !(in>>read) || read != "DayActions:"
		|| !(in>>read) || read != "Start"  || !(in>>read) || read != "factors:"
		|| !(in>>st_factors) || !(in>>read) || read != "Collection:" )
		return nullptr;

	ptr = std::make_shared<DayActions>(goalFunction_,cl_types,checkFun,st_factors);

	while ( in>>read && read != "goal")
		{
		shared_ptr_Action ac = nullptr;

		for(unsigned int i = 0; i < parse_form_and_function.size(); ++i )//searching in possible option
			{
			if( parse_form_and_function[i].first == read )
				{
				ac = parse_form_and_function[i].second(in);

				if( ac != nullptr )
					{
					ptr->addAction(ac->clone());

					break;
					}
				else
					return nullptr;
				}
			}

		if(ac == nullptr)
			return nullptr;
		}

	if(read != "goal" || !(in>>read) || read != "function"
	   || !(in>>read) || read != "value:"
	   || !(in>>val) )
		{
		return nullptr;
		}

	ptr->goal_function_value = val;

	ptr->updateFactors();

	return ptr;
	}



std::vector< std::shared_ptr<DayActions>>  loadFromFile(std::istream& in, std::vector<stringAndParseFunction> parse_form_and_function, std::shared_ptr<GoalFunction> goalFunction_, const std::vector<shared_ptr_Action>& cl_types, std::vector<checkingFunction> checkFun )
	{
	std::vector< std::shared_ptr<DayActions>> vec;
	std::shared_ptr<DayActions> ptr;

	while ( ( ptr = DayActions::createFromFile(in,parse_form_and_function,goalFunction_,cl_types,checkFun) ) != nullptr  )
		{
		vec.push_back(ptr);
		}

	return vec;
	}

void DayActions::addRandAction()
	{
	addRandomAction();
	onlyUpdate();
	}
