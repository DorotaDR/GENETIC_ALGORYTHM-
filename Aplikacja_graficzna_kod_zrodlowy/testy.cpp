#include "GeneticAlgorithm.hpp"
#include "DayActions.hpp"
#include "Action.hpp"
#include "Sleep.hpp"
#include "Coffee.hpp"
#include "CoffeeAndSleepGoalFunction.hpp"
#include "GoalFunction.hpp"
#include "Chromosome.hpp"
#include "TimeRange.hpp"
#include "Factors.hpp"
#include "Point.hpp"
#include "generateRandom.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <stdio.h>
#include <ctime>
#include <chrono>

using namespace std;

struct three_ans
	{
	Chromosome* minimum, *maximum;
	double mean;
	unsigned int minimum_i, mean_i, maximum_i;
	vector< double > min_trace, max_trace;
	long min_time,mean_time, max_time;
	};

void saveChromosome(const string& nazwa,const Chromosome* da, unsigned int iter)
	{
	fstream plik;
	plik.open(nazwa,ios_base::out);

	if(plik.good())
		{
		plik<<(*da)<<"\niteracja = "<<iter;

		plik.close();
		}
	}

void saveTrace(const string& nazwa,const vector<double>& trace)
	{
	std::fstream plik;
	plik.open(nazwa, std::ios_base::out);

	if(plik.good())
		{
		for(double x : trace)
			plik<<x<<std::endl;
		}

	plik.close();
	}

void makeTest(shared_ptr<DayActions> dA, vector<Chromosome*>& pop_st ,const unsigned int liczba_itracji, const unsigned int max_populacji, const unsigned int max_najlepszych,	const double min_przyrost, const unsigned int liczba_powtorzen,	const unsigned int try_number,	string nazwa)
	{
	cout<<"\nTest dla :"<<'\t'<<
				"max iteracji: " << liczba_itracji<<'\t'<<
				"max najlepszych: " << max_najlepszych<<'\t'<<
				"max populacji: " << max_populacji<<'\t'<<
				"min przyrost: " << min_przyrost<<'\t'<<
				"liczba powtorzen: " << liczba_powtorzen<<'\t'<<
				"liczba prob: " << try_number<<'\t'<<
				"nazwa pliku: " << nazwa<<endl;
	///temp
	vector<Chromosome*> proby_zb(try_number);
	vector<unsigned int> proby_zb_iter(try_number);
	vector< vector<double> > proby_zb_trace(try_number);
	vector<long> proby_zb_time(try_number);
	///

	for(unsigned int i = 0; i < try_number; ++i)
		{
		GeneticAlgorithm gAlg(cout, dA.get(), min_przyrost, liczba_powtorzen, liczba_itracji, max_populacji, max_najlepszych,pop_st);

		auto start = std::chrono::high_resolution_clock::now();
		proby_zb[i] = gAlg.startAlgorithm();
		auto end = std::chrono::high_resolution_clock::now();

		chrono::duration<long, std::micro> difference = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
		proby_zb_time[i] =  difference.count();

		proby_zb_iter[i] = gAlg.getIteration();
		proby_zb_trace[i] = gAlg.getValueTrace();
		cout<<'.';
		}

	three_ans ans;
	ans.minimum = ans.maximum = proby_zb[0];
	ans.minimum_i = ans.mean_i = ans.maximum_i = proby_zb_iter[0];
	ans.mean = ans.maximum->goalFunction();
	ans.min_time = ans.max_time = ans.mean_time = proby_zb_time[0];

	for(unsigned int i = 1; i < try_number; ++i)
		{
		ans.mean += proby_zb[i]->goalFunction();
		ans.mean_i += proby_zb_iter[i];
		ans.mean_time += proby_zb_time[i];

		if(proby_zb[i]->goalFunction() > ans.maximum->goalFunction())
			{
			ans.maximum = proby_zb[i];
			ans.maximum_i = proby_zb_iter[i];
			ans.max_trace = proby_zb_trace[i];
			ans.max_time = proby_zb_time[i];
			}
		else if(proby_zb[i]->goalFunction() < ans.minimum->goalFunction())
			{
			ans.minimum = proby_zb[i];
			ans.minimum_i = proby_zb_iter[i];
			ans.min_trace = proby_zb_trace[i];
			ans.min_time = proby_zb_time[i];
			}
		}

	ans.mean /= try_number;
	ans.mean_i /= try_number;
	ans.mean_time /= try_number;

	cout<< "\nWyniki : \n" << "           f. celu\t" << "iteracja\t" << "czas wykonania\n" <<
				"minimum    " << ans.minimum->goalFunction() << "\t" << ans.minimum_i << "\t" << ans.min_time << "us\n" <<
				"srednia    " << ans.mean << "\t" << ans.mean_i << "\t" << ans.mean_time << "us\n" <<
				"maksimum   " << ans.maximum->goalFunction() << "\t" << ans.maximum_i << "\t" << ans.max_time << "us\n";


	saveChromosome( nazwa + "_min.da" , ans.minimum, ans.minimum_i );
	saveTrace( nazwa + "_min.trace" , ans.min_trace);
	saveChromosome( nazwa + "_max.da" , ans.maximum, ans.maximum_i );
	saveTrace( nazwa + "_max.trace" , ans.max_trace);

	for(auto x : proby_zb)
		delete x;

	}

int test()
	{

	fstream plik;

	string obiekt;
	string populacja_startowa;
	unsigned int liczba_prob;

	Factors start_factors(0,-0.1);

	shared_ptr<Sleep> sl = make_shared<Sleep>();
	shared_ptr<Coffee> cof = make_shared<Coffee>();
	shared_ptr<GoalFunction> gF = make_shared<CoffeeAndSleepGoalFunction>();

	vector< shared_ptr<const Action> > class_vector;
	vector<checkingFunction> function_vector;

	class_vector.push_back(sl);
	class_vector.push_back(cof);

	function_vector.push_back(Sleep::checkRestrictionAndRetake);
	function_vector.push_back(Coffee::checkRestrictionAndRetake);

	shared_ptr<DayActions> dA = make_shared<DayActions>(gF,class_vector,function_vector,start_factors);
	vector<Chromosome*> pop_st;
	vector<shared_ptr<DayActions>> populacja_z_pliku;

	//vektor do parsowania plików
	stringAndParseFunction p1("Coffee:", Coffee::createFromStream), p2("Sleep:", Sleep::createFromStream);
	vector<stringAndParseFunction> parse_form;

	parse_form.push_back(p1);
	parse_form.push_back(p2);

	cout << "Plik osobnika startowego: \n";
	cin >> obiekt;
	plik.open(obiekt,std::ios_base::in);

	if(plik.good())
		{
		std::shared_ptr<DayActions> DA = DayActions::createFromFile(plik,parse_form,gF,class_vector,function_vector);

		if(DA !=nullptr)
			dA = DA;
		else
			return -1;

		plik.close();
		}
	else
		return -1;



	cout << "Plik populacji startowej:\n";
	cin >> populacja_startowa;

	plik.open(populacja_startowa,std::ios_base::in);

	if(plik.good())
		{
		populacja_z_pliku = loadFromFile(plik,parse_form,gF,class_vector,function_vector);
		if(populacja_z_pliku.size() != 0)
			{
			for(shared_ptr<DayActions> x : populacja_z_pliku)
				pop_st.push_back( x->clone() );
			}
		else
			return -2;

		plik.close();
		}
	else
		return -2;

	cout<<"Liczba prob:\n";
	cin>>liczba_prob;


	const unsigned int liczba_itracji = 1000;
	const unsigned int max_populacji = 200;
	const unsigned int max_najlepszych = 10;
	const double min_przyrost = 0.0;
	const unsigned int liczba_powtorzen = 10;
	const unsigned int try_number = liczba_prob;
	string nazwa = "zbieznosc_";
	int num_prob = 10;

	/*******próby zbieżności********/
	cout<<"ZBIEZNOSC: \n";
	makeTest(dA, pop_st, liczba_itracji, max_populacji, max_najlepszych, min_przyrost, liczba_powtorzen,	try_number, nazwa );
	/****************************************/

/*******max populacji********/
	cout<<"MAX POPULACJI: \n";
	for(int i = 0; i <= num_prob; ++i)
		{
		int val = 21 + i/(double)num_prob *(double)(800);
		nazwa = "max_populacji_"  + to_string(val);

		makeTest(dA, pop_st, liczba_itracji, val, max_najlepszych, min_przyrost, liczba_powtorzen,	try_number, nazwa );
		}
	/****************************************/

/*******min przyrost********/
	cout<<"MINIMALNY PRZYROST: \n";
	for(int i = 0; i <= num_prob; ++i)
		{
		double val = 0.0 + i/(double)num_prob *1.0;
		nazwa = "min_przyrost_"  + to_string(val);

		makeTest(dA, pop_st, liczba_itracji, max_populacji, max_najlepszych, val, liczba_powtorzen,	try_number, nazwa );
		}
	/****************************************/


/*******max najlepszych********/
	cout<<"MAX NAJLEPSZYCH: \n";
	for(int i = 0; i <= num_prob; ++i)
		{
		int val = 3 + i/(double)num_prob *(double)(17);
		nazwa = "max_najlepszych_"  + to_string(val);

		makeTest(dA, pop_st, liczba_itracji, max_populacji, val, min_przyrost, liczba_powtorzen, try_number, nazwa );
		}
	/****************************************/

/*******liczba powtorzen********/
	cout<<"LICZBA POWTORZEN: \n";
	for(int i = 0; i <= num_prob; ++i)
		{
		int val = 0 + i/(double)num_prob * double(100);
		nazwa = "liczba_powtorzen_"  + to_string(val);

		makeTest(dA, pop_st, liczba_itracji, max_populacji, max_najlepszych, min_przyrost, val,	try_number, nazwa );
		}
	/****************************************/

/*******iteracje********/
	cout<<"MAx iteracji: \n";
	for(int i = 0; i <= num_prob; ++i)
		{
		int val =  5 + i*i*9.95;
		nazwa = "liczba_iteracji_"  + to_string(val);

		makeTest(dA, pop_st, val, max_populacji, max_najlepszych, min_przyrost, liczba_powtorzen,	try_number, nazwa );
		}
	/****************************************/

/*******losowa populacja********/
	shared_ptr<DayActions> ddd = shared_ptr<DayActions>( dynamic_cast<DayActions*>( dA->randomChromosome() ) );
	cout<<"MAx iteracji: \n";
	for(int i = 0; i <= num_prob; ++i)
		{

		nazwa = "losowa_populacja_"  + to_string(i);
		vector<Chromosome*> v;
		makeTest( ddd, v, liczba_itracji, max_populacji, max_najlepszych, min_przyrost, liczba_powtorzen,	try_number, nazwa );
		}
	/****************************************/

	return 1;
	}

int main()
	{
	char abc;

	cout<<test()<<endl;

	cin>>abc;
	return 0;
	}




