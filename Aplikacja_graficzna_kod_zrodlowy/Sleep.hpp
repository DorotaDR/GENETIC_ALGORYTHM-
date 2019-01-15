/*
 * Sleep.hpp
 *
 *  Created on: 16 lis 2018
 *      Author: Dorotka
 */

#ifndef SLEEP_HPP_
#define SLEEP_HPP_
#include "Action.hpp"
#include "DayActions.hpp"
#include <istream>


class Sleep :public Action{
private:
    static int MIN_SLEEP; //jednorazowo minimum drzemki to 20min
    static int MIN_DAY_SLEEP; //180 minut = 3h
    static double A_MULTIPLIER;
    Factors factorsAfter(); //funkcja pomocnicza, uzywana w Sleep::update(...)

    /* ALPH(int m)
	 *	 funkcja oblicza wspó³czynnik (alfa) spadku aktywnoœci
	 * 	w zale¿noœci od snu w minutach (m).
	 */
	static double alph(int m);

	static int sumSleeps(std::vector<Action*>* collection);	//sumuje ilosc snu w wektorze


public:


// --- KONSTRUKTORY
	Sleep();
	Sleep (TimeRange _begin_end);
	Sleep (int _begin, int _end);

// --- ACTION'S VIRTUAL FUNCTIONS

	// --- checkRestrictionAndRetake(std::vector<Action*>* collection) ---
	// ! Wektor Collection MUSI!!! byc posegregowany CHRONOLOGICZNIE !
	// funkcja również łączy dwa przylegające sny w jeden.
	static void checkRestrictionAndRetake(std::vector<Action*>* collection, DayActions* dayActions);
    static void deleteToShortSleeps(std::vector<Action*>* collection, DayActions* dayActions);
    static void completionSleepAtDayAction(std::vector<Action*>* collection, DayActions* dayActions);
    static void connectNearest(std::vector<Action*>* collection, DayActions* dayActions);

    static void setA_Multiplier(double);
    static void setMinSleep(int);
    static void setMinTotalSleep(int);


	// --- toString()
	// 	do wyswietlania Sleep
	string toString() const;

	// --- update()
	// 	Przeszukuje wektor i uaktualnia we wszystkich Sleep składową factors_after
	void update(std::vector<Action*>* collection, Factors& start_factors);

	// --- randomAction ---
	// Zwraca wskaznik (stworzony przez new) na losowa Sleep w podanym TimeRange.
	Action* randomAction(TimeRange range) const;

	// --- clone() ---
	// 	kopiuje this do nowego wskaznika na Sleep utworzinego przez new
	Action* clone() const;

	// --- dividinByRange(...) ---
	//	jezeli range zawiera sie w czasie trwania Sleep to dzieli go na dwa snu:
	//		- jeden to this ze zmienionym czasem trwania,
	//		- a drugi, ten pozniejszy jest tworzony przez new i zwracany przez funkcje
	//	jak range nie zawiera sie w Sleep zwraca: nullptr
	Action* divideByRange(TimeRange& range);

	std::vector<Point> getActivityDuring() const;

	static std::shared_ptr<Action> createFromStream(std::istream& in);

	~Sleep(){}
};


#endif /* SLEEP_HPP_ */
