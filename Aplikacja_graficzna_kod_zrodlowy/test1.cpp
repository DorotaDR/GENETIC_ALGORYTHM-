 #include<iostream>
#include "Ciag.hpp"
#include "GeneticAlgorithm.hpp"
#include <time.h>
#include "generateRandom.hpp"
#include<vector>
#include "Sleep.hpp"
#include "Coffee.hpp"
#include "TimeRange.hpp"
#include "CoffeeAndSleepGoalFunction.hpp"
#include<memory>
#include "DayActions.hpp"
#include "Chromosome.hpp"
#include "dwiefunkcje.hpp"
#include "Point.hpp"
#include <fstream>

using namespace std;

int main()
    {
    char abc;

    try
        {
        std::shared_ptr<Sleep> sleep = make_shared<Sleep>(90,270);
        std::shared_ptr<Coffee> coffee =  make_shared<Coffee>(20*60,70);
        Sleep* a1 =new Sleep(90,270);
        Coffee* a2 = new Coffee(1200,90);

        TimeRange range;
        Factors st_factors(100,-0.05);
        vector<shared_ptr_Action> vec;
        vec.push_back(shared_ptr_Action(sleep));
        vec.push_back(shared_ptr_Action(coffee));

        std::vector<checkingFunction> fun;
        fun.push_back(Sleep::checkRestrictionAndRetake);
        fun.push_back(Coffee::checkRestrictionAndRetake);

        std::shared_ptr<GoalFunction> gF = make_shared<CoffeeAndSleepGoalFunction>();

        DayActions ch(gF,vec,fun,st_factors);
        ch.addAction(a1);
        ch.addAction(a2);
				ch.updateFactors();
        clock_t start_time, end_time;

        //(wyswietlanie ,chrom. , roznica, liczba powtorzen wyniku, max iter.,max. populacja, max liczba wybieranych najlepszych)
        //GeneticAlgorithm genAlg = GeneticAlgorithm(cout,&ch,0.1,15,100,300,15);

        start_time = clock();
        /*Chromosome* ans = genAlg.startAlgorithm(true,1);

        while(abc != 'q')
            {
            delete ans;

            ans = genAlg.resume(true,1);

            cin>>abc;
            }
*/
        end_time = clock();
/*        cout<<genAlg<<endl<<endl;
        genAlg.status(cout);
        std::cout<<endl<<(*ans);*/

				//genAlg.savePopulation(cout);

				Factors f(13,-90);
				TimeRange tr(2,5);
				Sleep s(tr);
				Coffee c(tr,70);

				DayActions da(gF,vec,fun,f);
				da.addAction(&s);
				da.addAction(&c);

				std::fstream plik;

				plik.open("plik.txt", std::ios::out);

				plik<<da;
				plik<<ch;

				plik.close();

				cin>>abc;

				plik.open("plik.txt", std::ios::in);
				std::vector<stringAndParseFunction> parse_form_and_function;

				stringAndParseFunction p1("Coffee:", Coffee::createFromStream), p2("Sleep:", Sleep::createFromStream);

				parse_form_and_function.push_back(p1);
				parse_form_and_function.push_back(p2);

				std::vector< std::shared_ptr<DayActions>> ptr = loadFromFile(plik,parse_form_and_function,gF,vec,fun );
				if( ptr.size() != 0 )
					for(auto x : ptr)
						cout<<x->toString()<<"\n";
				else
					cout<<"bad\n";

				plik.close();
        cout<<"Execution time : "<<end_time-start_time<<endl;

        //delete ans;
        //start_time = clock();
        //genAlg.restart(&ch,0,10,1000,2000,3);
        //end_time = clock();
        //cout<<"RESTART : \n\n"<<genAlg<<endl<<endl<<"Execution time : "<<end_time-start_time<<endl;

  /*  try{
    std::shared_ptr<Sleep> sleep = make_shared<Sleep>();
    std::shared_ptr<Coffee> coffee =  make_shared<Coffee>();
    TimeRange range;
    Factors st_factors(0,0);

    std::vector<Pair_shared_ptr_Action_checkingFunction> vec;
    vec.push_back(Pair_shared_ptr_Action_checkingFunction(sleep,Sleep::checkRestrictionAndRetake));
    vec.push_back(Pair_shared_ptr_Action_checkingFunction(coffee,Coffee::checkRestrictionAndRetake));

    std::shared_ptr<GoalFunction> gF = make_shared<CoffeeAndSleepGoalFunction>();

    DayActions dA(gF,vec,st_factors);
    //gF = nullptr;
    //vec.clear();
    //sleep = nullptr;
    //coffee = nullptr;
    //dA.addAction(new Sleep(60,23*60));

    DayActions* rDA1 = dA.randomDayActions();
    DayActions* rDA2 = dA.randomDayActions();

    cout<<"\nFirst DayAction:\n\n"<<rDA1->toString()<<"\n_____________________________________________________________\n";
    cout<<"\nSecond DayAction:\n\n"<<rDA2->toString()<<"\n_____________________________________________________________\n";

    for(int i = 0; i < 1; ++i)
        {
        cout<<"Mutated DayAction nr:"<<i+1<<"\n";
        Chromosome* rDA = rDA1->mutation();
        cout<<rDA->toString()<<"\n_____________________________________________________________\n";
        delete rDA;
        cout<<"Crossed DayAction\n";
        rDA = rDA1->crossingOver(rDA2);
        cout<<rDA->toString()<<"\n_____________________________________________________________\n";
        delete rDA;
        }
*/


/* dzielenie
    cout<<"vec : \n";
    for(Action* x : vec)
        {
        cout<<x->toString();
        range = TimeRange::randomTimeRange(x->getBegin(),x->getEnd());
        cout<<range<<endl;
        vec1.push_back(x->dividingByRange(range));
        }

    cout<<"after dividing : \n";
    for(int i = 0; i< vec.size(); ++i)
        {
        cout<<vec[i]->toString();

        if(vec1[i] == nullptr)
            {
            cout<<"nullptr\n\n";
            continue;
            }

        cout<<vec1[i]->toString()<<endl;
        }*/


    //delete rDA1;
    //delete rDA2;
    }
    catch(std::string s)
        {
        cout<<s<<endl;
        }
    catch(std::exception& e)
        {
        cout<<e.what()<<endl;
        }
    catch(char* s)
        {
        cout<<s<<endl;
        }
    catch(...)
        {
        cout<<"nieznane"<<endl;
        }

    cin>>abc;

    return 0;
    }
