#include "TimeRange.hpp"

// DODAC FUN TimeRange::isEqual od Michala!!!!!

/*------ METHODES FROM CLASS TIMERANGE -------*/


TimeRange::TimeRange(){
		begin = 0;
		end = 24*60;    //bo teraz by³by problem z setter'ami gdyby chcieæ zmieniæ pocz¹tek najpierw na >1, a end bylby 1 to niezmieni
};

TimeRange::TimeRange(const TimeRange& _timeRange){
	begin = _timeRange.begin;
	end = _timeRange.end;
};

TimeRange::TimeRange(int _begin, int _end){
	if (_begin> _end){
		begin =_end ;
		end = _begin;
	}
	else{
		begin = _begin;
		end = _end;
	};
};


int TimeRange::getBegin() const{
	return begin;
};

int TimeRange::getEnd() const{
	return end;
};


bool TimeRange::setBegin(int _beg){
	if(_beg  > end)
        return false;

    begin = _beg;
    return true;
};

bool TimeRange::setEnd(int _end){
	if(_end < begin)
        return false;

    end = _end;
    return true;
};


int TimeRange::length() const{
	return end-begin;
};

TimeRange TimeRange::randomTimeRange(){
	int rndBegin = generateRandom(0,  MIN_IN_DAY);
	int rndEnd = generateRandom(rndBegin,  MIN_IN_DAY);
	TimeRange rndTimeRange (rndBegin, rndEnd);
	return rndTimeRange;
};

TimeRange TimeRange::randomTimeRange(int _begin,int _end){
	int rndLength = generateRandom(0,  _end-_begin);
	TimeRange rndTimeRange;
	rndTimeRange = TimeRange::randomTimeRange(_begin, _end, rndLength);
	return rndTimeRange;
}

TimeRange TimeRange::randomTimeRange(int _begin,int _end,int _length){
    if(_length > _end - _begin)
        return TimeRange(_begin,_end);
    else
        {
        int rndBegin = generateRandom(_begin,  _end-_length);
        int rndEnd = rndBegin+_length;
        TimeRange rndTimeRange (rndBegin, rndEnd);
        return rndTimeRange;
        }
};


/* -------------------------------------------------------------------
 * Pozostale funkcje przydatne do konwersji min na godziny i w napis
 * -------------------------------------------------------------------
 */

std::array<int,2> minToHours (int mins){
	std::array<int,2> hoursAndMins;
	int hours = mins / 60;
	int min = mins % 60;
	if (hours*60+min == mins){
		hoursAndMins[1] = hours;
		hoursAndMins[0] = min;
	}
	return hoursAndMins;
};

std::string minToString (int mins){
	std::array<int,2> minAndHours= minToHours(mins);
	int h = minAndHours[1];
	int m = minAndHours[0];
	std::stringstream text;

	text.width(2);
	text.fill(' ');
	text<<h<<"h ";
    text.width(2);
	text.fill(' ');
	text<<m<<"min";
	return text.str();
};

