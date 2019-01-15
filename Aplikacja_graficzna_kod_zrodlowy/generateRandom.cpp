/*
 * generateRandom.cpp
 *
 *  Created on: 10 lis 2018
 *      Author: Dorotka
 */

#include "generateRandom.hpp"

GenerateRandom generateRandom = GenerateRandom();

bool GenerateRandom::is_initialized = false;

GenerateRandom::GenerateRandom()
    {
    if(!is_initialized)
        {
        srand(time(NULL));
        is_initialized = true;
        }
    }

int GenerateRandom::operator()(int low, int high)
    {
    /*if(!is_initialized) //jesi nie zainicjowane jakimœ trafem
        {
        srand(time(0));
        is_initialized = true;
        }*/

    if(low > high)
        {
        int aux = low;
        low = high;
        high = aux;
        }
	//if (low>=0)   //przedzial nalezy do R
    return (rand()%(high-low+1)) + low;
    }
