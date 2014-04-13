#include "../game/Strategy.hpp"
#include <iostream>

/*************************
 * HUMAN CONTROLLED
 *************************/

HumanControlledStrategy::HumanControlledStrategy()
{
}

int HumanControlledStrategy::claim(GameState state)
{
    std::cout << "Player " << myPlayerNumber << ": Claim a region." << std::endl;
    state.display();
    int whichRegion = 0;

    bool outOfBounds = false;
    bool alreadyClaimed = false;
    // Validation loop
    do
    {
        outOfBounds = false;
        alreadyClaimed = false;
        std::cout << "Which region would you like to claim? ";
        std::cin >> whichRegion;

        // Set flags (do check)
        outOfBounds = whichRegion < 0 || whichRegion > (state.getNumRegions() - 1);
        if (!outOfBounds)
            alreadyClaimed = state.getRegionInfo(whichRegion).first != -1;

        if (outOfBounds)
            std::cout << "You have to claim somewhere ON the map." << std::endl;
        if (alreadyClaimed)
            std::cout << "You have to choose unclaimed territory." << std::endl;
    }
    while (alreadyClaimed || outOfBounds);

    // I think we checked all the dumb mistakes the user can make
    return whichRegion;
}

std::vector<std::pair<int, int>> HumanControlledStrategy::place(GameState state, int numTroops)
{
    std::vector<std::pair<int, int>> actions;
    int wherePut = 0;
    int howMany = 0;
    bool outOfBounds = false;
    bool notYours = false;
    bool badAmount = false;
    std::cout << "Player " << myPlayerNumber << ": Place your troops." << std::endl;
    do
    {
        outOfBounds = false;
        notYours = false;
        std::cout << "You have " << numTroops << " to place." << std::endl;
        std::cout << "Where would you like to place troops?";
        std::cin >> wherePut;
        // Error Checking
        outOfBounds = wherePut < 0 || wherePut > (state.getNumRegions() - 1);
        if (!outOfBounds)
            notYours = state.getRegionInfo(wherePut).first != myPlayerNumber;

        // Location choice is valid
        if (!outOfBounds && !notYours)
        {
            do
            {
                badAmount = false;
                std::cout << "How many troops will you place here? " << std::endl;
                std::cin >> howMany;

                badAmount = howMany < 0 || howMany > numTroops;
                if (badAmount)
                    std::cout << "You have " << numTroops << " to place.  Choose a valid number." << std::endl;

            }
            while (badAmount);

            //Should be valid placement, let them do it.
            if (howMany == 0) // Gonna let 0 work so they can change their mind, but not gonna add it to the action list
                std::cout << "Cute..." << std::endl;
            else
            {
                // THEY CHOSE CORRECTLY YAY!
                actions.push_back(std::pair<int, int>(wherePut, howMany));
                numTroops -= howMany;
            }
        }


        if (outOfBounds)
            std::cout << "That's not on the map." << std::endl;
        if (notYours)
            std::cout << "You have to add them to YOUR territory." << std::endl;

    }
    while (numTroops > 0 || outOfBounds || notYours);
    return actions;
}

std::pair<int, int> HumanControlledStrategy::attack(GameState state)
{
    std::cout << "Player " << myPlayerNumber << ": Attack." << std::endl;
    state.display();

    int location = 0;
    bool outOfBounds = false;
    bool attackingSelf = false;

    do
    {
        outOfBounds = false;
        attackingSelf = false;
        std::cout << "Which country do you want to attack (-1 to stop this phase)? ";
        std::cin >> location;

        // Check for quit FIRST
        if (location < 0)
            return std::pair<int, int>(-1, -1); //let the turn be over

        // Set flags (error check)
        outOfBounds = location >= state.getNumRegions();
        if (!outOfBounds)
            attackingSelf = state.getRegionInfo(location).first == myPlayerNumber;

        if (outOfBounds)
            std::cout << "You must select a valid location to attack." << std::endl;
        if (attackingSelf)
            std::cout << "You can't attack yourself, dummy." << std::endl;
    }
    while (outOfBounds || attackingSelf);


    return std::pair<int, int>(location, myPlayerNumber);
}

bool HumanControlledStrategy::defend(GameState state, int countryAttacked, int countryAttacking)
{
    //Sufficiently trivial that we don't involve the human.
    if (state.getRegionInfo(countryAttacked).second >= 2)
        return true;
    else
        return false;
}

std::vector<std::tuple<int, int, int> > HumanControlledStrategy::fortify(GameState state)
{
    std::cout << "Player " << myPlayerNumber << ": Fortify." << std::endl;
    std::vector<std::tuple<int, int, int>> actions;
    //int whereFrom = 0;
    //int whereTo = 0;
    //int howMany = 0;
    //actions.push_back(std::tuple<int,int,int>(whereFrom,whereTo,howMany));
    return actions; //for now, let's do nothing
}
