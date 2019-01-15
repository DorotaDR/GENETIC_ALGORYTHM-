/*
 * Action.hpp
 */

#ifndef ACTION_HPP_
#define ACTION_HPP_


#include <string>
#include <vector>
#include <sstream>
#include <istream>

#include "TimeRange.hpp"
#include "Factors.hpp"
#include "DayActions.hpp"
#include "Point.hpp"

using std::string;

class Action {

protected:
	TimeRange begin_end;   			// czas trwania czynnosci
	Factors factors_before;
	Factors factors_after;			// Wspolczynniki po wykonaniu czynnosci

public:

	//---- GET FUNCTION ----
	int getBegin() const;
	int getEnd() const;
	TimeRange getTimeRange() const;
	Factors getFactorsAfter() const;

	//---- SET FUNCTION ----
	bool setBegin(int _begin);
	bool setEnd(int _end);
	void setBeginEnd(int begin, int end);
	void setTimeRange(TimeRange range);
	void setTimeRange(int, int);

	// ---- isEqual(Action&) ----
	//compare Actions, return answer from TimeRange::isEqual():
	// <0 if this is earlier than other;
	//  >0 if this is later than other;
	//  0 if are some complications or ranges are equal
	int isEqual(const Action* otherAction) const;

	// ---- VIRTUAL FUNCTIONS ----

	virtual void update(std::vector<Action*>* collection, Factors& start_factors) = 0;
	virtual Action* divideByRange(TimeRange& range) = 0;
	virtual Action* randomAction(TimeRange range)const  = 0;
	virtual Action* clone() const = 0;
	virtual string toString() const ;
	virtual std::vector<Point> getActivityDuring() const ;

	friend std::ostream& operator<<(std::ostream& out, const Action& obj);

	static bool readFromFile(std::istream& in, Factors& st_factors, TimeRange& range);

	virtual ~Action()
        {}
};

#endif /* ACTION_HPP_ */
