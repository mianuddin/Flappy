#include "dice.h"
// implementation of dice class
// written Jan 31, 1994, modified 5/10/94 to use RandGen class

Dice::Dice(int sides)
// postcondition: all private fields initialized     
{
    myRollCount = 0;
    mySides = sides;
}

int Dice::Roll()
// postcondition: number of rolls updated
//                random 'die' roll returned     
{
    myRollCount = myRollCount + 1;          // update # of times die rolled
    return myGenerator.RandInt(1,mySides);  // in range [1..mySides]
}

int Dice::NumSides()
// postcondition: return # of sides of die     
{
    return mySides;
}

int Dice::NumRolls()
// postcondition: return # of times die has been rolled     
{
    return myRollCount;
}
