/*
 * TimeRangeFactor.hpp
 *
 * Zawiera:
 *  - klasy:
 * 		- TimeRange,
 * 		- Factors
 * 	- funkcje pomocnicze do konwersji min na godziny:
 * 		- minToHours(int mins)
 * 		- minToString (int mins)
 *
 */

#ifndef TIMERANGE_HPP_
#define TIMERANGE_HPP_

#include <array>
#include <string>
#include <cmath>

#include "generateRandom.hpp" // zawiera funkcje generateRandom(int, int)
#include <ostream>
#include <istream>
#include <sstream>

/* --------- STALE ----------*/
const int MIN_IN_DAY=24*60;		//ile jest min w dobie


/* --------- TIME RANGE ---------*/
class TimeRange{
private:
	int begin;
	int end;

public:


	TimeRange();						//Domyslnie begin=0, end=1
	TimeRange(const TimeRange& _timeRange);	//Zwykly konstruktor kopiujacy
	TimeRange(int _begin, int _end);	//Ustawia podane watosci begin i end


	//GET FUNCTIONS:
	int getBegin() const;
	int getEnd() const;

	//SET FUNCTIONS:
	bool setBegin(int _beg);
	bool setEnd(int _end);


	int length() const;   					// oblicza dlugosc trwania TimeRange

	/* Funkcja isEqual zaimplementowana przez Michala*/
	//bool isEqual(TimeRange& second);

	static TimeRange randomTimeRange();									// losowy przedzial czasu w jednej dobie
	static TimeRange randomTimeRange(int _begin,int _end);					// losowy przedzial czasu miedzy podanymi wartosciami czasu
	static TimeRange randomTimeRange(const TimeRange& range, int length_); //losowy przedział o okreeślonej długości w przedziale range
	static TimeRange randomTimeRange(int _begin,int _end,int _length);		// losowy przedzial czasu miedzy podanymi wartosciami czasu
																		// ale o okreslonej dlugosci
    	// ---- isEqual(Action&) ----
	//compare Actions, return answer from TimeRange::isEqual():
	// this->end - second.begin < 0 if this is earlier than other;
	//  this->begin - second.end > 0 if this is later than other;
	//  0 if are some complications or ranges are equal
    int isEqual(const TimeRange&) const;
    bool contain(const TimeRange& range) const;


    TimeRange& operator=(const TimeRange&);
    friend std::ostream& operator<<(std::ostream& out, const TimeRange& range);
	friend bool operator>>(std::istream& in, TimeRange& range);

	void operator()(int begin_, int end_);
};

std::ostream& operator<<(std::ostream& out, const TimeRange& range);
bool operator>>(std::istream& in, TimeRange& range);

std::array<int,2> minToHours (int mins); 		//Zamiana minut na godziny. Zwraca tablicę.
												//element [0] to liczba minut, element [1] to ilosc godzin
std::string minToString (int mins);				// do wyswietlania min jako licze godzin i minut

bool stringTimeToInt(std::istream& in, int& min);


#endif /* TIMERANGEFACTOR_HPP_ */
