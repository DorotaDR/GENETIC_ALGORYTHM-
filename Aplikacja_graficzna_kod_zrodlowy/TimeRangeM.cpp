#include "TimeRange.hpp"

int TimeRange::isEqual(const TimeRange& second) const
    {
    if(begin == second.begin && end == second.end)
        return 0;
    else if(end <=second.begin)//this is earlier
        return -1;
    else if(second.end <= begin)
        return 1;
    else
        return 0;
    }

TimeRange& TimeRange::operator=(const TimeRange& second)
    {
    begin = second.begin;
    end = second.end;

    return *this;
    }

std::ostream& operator<<(std::ostream& out, const TimeRange& range)
    {
    out<<"START:  "<<minToString(range.getBegin())<<"\tSTOP: "<<minToString(range.getEnd());

    return out;
    }
void TimeRange::operator()(int begin_, int end_)
    {
    if(begin_ > end_)
        {
        begin = end_;
        end = begin_;
        }
    else
        {
        begin = begin_;
        end = end_;
        }
    }

bool TimeRange::contain(const TimeRange& range) const
    {
    if(range.begin >= begin)
        {
        if(range.end <= end)
            {//i is contained
            return true;
            }
        }
    return false;
    }

TimeRange TimeRange::randomTimeRange(const TimeRange& range, int length_)
    {
    return randomTimeRange(range.begin,range.end, length_);
    }


bool operator>>(std::istream& in, TimeRange& range)
	{
	int beg;
	int end;
	std::string read;

	if( !(in.good()) )
		return false;

	if( !(in>>read) || read != "START:")
		return false;

	if( ! stringTimeToInt(in,beg))
		return false;

	//stop
	if( !(in>>read) || read != "STOP:")
		return false;

	if( !stringTimeToInt(in, end))
		return false;

	range(beg, end);

	return true;
	}

bool stringTimeToInt(std::istream& in, int& min)
	{
	int vh = -1;
	int vm = -1;
	std::string read;

	if( in.bad() )
		return false;

	if( !( in>>vh ) || vh < 0 || 24 < vh )
		return false;

	if( !( in>>read ) || read != "h" )
		return false;

	if( !( in>>vm ) || vm < 0 || 59 < vm || (vh == 24 && vm !=0) )
		return  false;

	if( !(in>>read) || read != "min" )
		return false;

	min = vh*60 + vm;

	return true;
	}





