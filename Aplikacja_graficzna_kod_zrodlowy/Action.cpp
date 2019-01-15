#include "Action.hpp"

// --- GET FUNCTIONS

int Action::getBegin() const{
	return begin_end.getBegin();
};
int Action::getEnd() const{
	return begin_end.getEnd();
};

TimeRange Action::getTimeRange() const{
    const TimeRange ans = begin_end;
	return ans;
};


Factors Action::getFactorsAfter() const{
	return factors_after;
}


// --- SET FUNCTIONS
bool Action::setBegin(int _begin){
	return begin_end.setBegin(_begin);
};
bool Action::setEnd(int _end){
	return begin_end.setEnd(_end);
};


void Action::setTimeRange(TimeRange range)
    {
    begin_end = range;
    }

void Action::setTimeRange(int begin, int end)
	{
    begin_end = TimeRange(begin,end);
	}

void Action::setBeginEnd(int begin, int end)
    {
    begin_end(begin,end);
    }

// --- ISEQUAL FUNCTION:

int Action::isEqual(const Action* otherAction) const{
    return begin_end.isEqual(otherAction->begin_end);
	/*if (otherAction->getEnd() <= begin_end.getBegin())
		return 1;
	else if(otherAction->getBegin() >= begin_end.getEnd() )
		return -1;
	else
		return 0;*/
};

std::string Action::toString() const
    {
	std::stringstream text;
	text<<begin_end<<"\n"<<factors_after;

	return text.str();
    }

std::ostream& operator<<(std::ostream& out, const Action& obj)
    {
    out<<obj.toString();

	return out;
	}

std::vector<Point> Action::getActivityDuring() const
	{
	std::vector<Point> vec;
	vec.push_back( Point( begin_end.getBegin(), factors_before.getY() ) );
	vec.push_back( Point( begin_end.getEnd(), factors_after.getY() ) );
	return vec;
	}


bool Action::readFromFile(std::istream& in, Factors& st_factors, TimeRange& range)
	{
	TimeRange temp_range;
	Factors temp_st_factors;

	if( !(in>>temp_range) || !(in>>temp_st_factors) )
		return false;

	st_factors = temp_st_factors;
	range = temp_range;

	return  true;
	}











