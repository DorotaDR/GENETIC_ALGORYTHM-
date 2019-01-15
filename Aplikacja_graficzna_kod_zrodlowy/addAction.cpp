#include "DayActions.hpp"

bool DayActions::addAction(Action* a)
        {
    try
        {
        for(auto x : collection)
            if(a->isEqual(x) == 0)
                return false;

        collection.push_back(a);

        modified = true;
        onlyUpdate();

        return true;
        }
    catch(std::bad_alloc e)
        {
        return false;
        }
    catch(...)
        {
        return false;
        }
    }

