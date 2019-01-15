cls
g++ -c -Wall -std=c++11 Point.cpp -o Point.o
g++ -c -Wall -std=c++11 generateRandom.cpp -o generateRandom.o
g++ -c -Wall -std=c++11 Factors.cpp -o Factors.o
g++ -c -Wall -std=c++11 TimeRange.cpp -o TimeRange.o
g++ -c -Wall -std=c++11 TimeRangeM.cpp -o TimeRangeM.o
g++ -c -Wall -std=c++11 Action.cpp -o Action.o
g++ -c -Wall -std=c++11 Sleep.cpp -o Sleep.o
g++ -c -Wall -std=c++11 Coffee.cpp -o Coffee.o
g++ -c -Wall -std=c++11 CoffeeAndSleepGoalFunction.cpp -o CoffeeAndSleepGoalFunction.o
g++ -c -Wall -std=c++11 goalFunction.cpp -o goalFunction.o
g++ -c -Wall -std=c++11 addAction.cpp -o addAction.o
g++ -c -Wall -std=c++11 DayActionsM.cpp -o DayActionsM.o
g++ -c -Wall -std=c++11 Chromosome.cpp -o Chromosome.o
g++ -c -Wall -std=c++11 GeneticAlgorithm.cpp -o GeneticAlgorithm.o
g++ -c -Wall -std=c++11 test1.cpp -o test1.o
g++ Point.o generateRandom.o Factors.o TimeRange.o TimeRangeM.o Action.o Sleep.o Coffee.o CoffeeAndSleepGoalFunction.o goalFunction.o addAction.o DayActionsM.o Chromosome.o GeneticAlgorithm.o test1.o -o algorytm_genetyczny_1.exe
algorytm_genetyczny_1.exe
