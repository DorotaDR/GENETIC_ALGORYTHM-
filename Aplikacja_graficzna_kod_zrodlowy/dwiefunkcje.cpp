#include "dwiefunkcje.hpp"

void DwieFunkcje::update() const
    {
    A = begA;
    B = begB;

    for(bool x : ciag)
        {
        if(x)
            A=2*A-B;
        else
            B=2*B+A;
        }
    goalFunction();
    }

Chromosome* DwieFunkcje::crossingOver(const Chromosome* other) const
    {
    const DwieFunkcje* second =  dynamic_cast<const DwieFunkcje*>(other);
    const DwieFunkcje* first = this;
    DwieFunkcje* ans = new DwieFunkcje();

    if(first->ciag.size() > second->ciag.size())
        {
        *ans = *first;

        for(int i = 0; i < second->ciag.size()/2; ++i)
            {
            int r =generateRandom(0,second->ciag.size()-1);

            ans->ciag[r] = second->ciag[r];
            }
        }
    else
        {
        *ans = *second;

        for(int i = 0; i < first->ciag.size()/3; ++i)
            {
            int r =generateRandom(0,first->ciag.size()-1);

            ans->ciag[r] = first->ciag[r];

            }
        }

    ans->update();

    return ans;
    }

Chromosome* DwieFunkcje::mutation() const
    {
    DwieFunkcje* ans = new DwieFunkcje(*this);

    if(generateRandom(1))
        ans->ciag.push_back(generateRandom(2));
    else
        {
        int first = generateRandom(0,ciag.size()-1);
        int second = generateRandom(0,ciag.size()-1);

        while(first == second)
            second = generateRandom(0,ciag.size()-1);

        ans->ciag[first] = !(ans->ciag[first]);
        ans->ciag[second] = !(ans->ciag[second]);
        }

    ans->update();

    return ans;
    }

double DwieFunkcje::goalFunction()const
    {
    int diffA = abs(endAB-A);
    int diffB = abs(endAB-B);

    int blizsze = std::min(diffA,diffB);

    if(A == endAB || B == endAB)
        {
        value = max_size - ciag.size();
        return value;
        }
    else
        {
        //value = INT_MIN;

        if(ciag.size() == 0)
            return INT_MIN;

        value = -blizsze*ciag.size();

        return value;
        }
    }

Chromosome* DwieFunkcje::randomChromosome() const
    {
    DwieFunkcje* losowe = new DwieFunkcje();
    int a = begA;
    int b = begB;
    int max_iter = 10000000;

    while(a != endAB && b != endAB)
        {
        losowe->ciag.clear();
        a = begA;
        b = begB;

        int size_= generateRandom(2,max_size);


        for(int i = 0; i < size_; ++i)
            {
            int r = generateRandom(1);
            if(r)
                {
                a=2*a-b;
                losowe->ciag.push_back(true);

                }
            else
                {
                b=2*b+a;
                losowe->ciag.push_back(false);
                }
            }

        losowe->A = a;
        losowe->B = b;

        if(--max_iter == 0)
            throw std::string("max_iter");
        }

    losowe->goalFunction();
    std::cout<<"!";
    return losowe;

    }

std::string DwieFunkcje::toString() const
    {
    std::stringstream str;
    str<<"Dwie Funkcje:\n";
    for(bool x : ciag)
        str<<(x? 'A' : 'B')<<" ";
    str<<ciag.size()
    <<"\nA = "<<A
    <<"\nB = "<<B
    <<"\nValue = "<<value
    <<"\n\n";

    return str.str();
    }

DwieFunkcje::~DwieFunkcje()
    {
    ciag.clear();
    }


DwieFunkcje& DwieFunkcje::operator=(const DwieFunkcje& other)
    {
    for(bool x : other.ciag)
        ciag.push_back(x);

    A = other.A;
    B = other.B;

    value = other.value;

    return *this;
    }

DwieFunkcje::DwieFunkcje(const DwieFunkcje& other)
    {
    *this = other;
    }


Chromosome* DwieFunkcje::clone() const
    {
    DwieFunkcje* ans = new DwieFunkcje(*this);

    return ans;
    }
