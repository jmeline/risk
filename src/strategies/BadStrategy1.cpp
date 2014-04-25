#include "../game/Strategy.hpp"

/*************************
* BAD STRATEGY
*************************/

BadStrategy1::BadStrategy1()
{
	movesDoneAttacking = 0;
}


int BadStrategy1::claim(GameState state)
{
	int whichRegion = 0;
	for( ; whichRegion < state.getNumRegions() ; ++whichRegion)
	{
		if(state.getRegionInfo(whichRegion).first == -1)
			break;
	}
	return whichRegion;
}
	
std::vector<std::pair<int,int>> BadStrategy1::place(GameState state, int numTroops)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
	if (beVerbose)  std::cout << "BadStrategy " << myPlayerNumber << " is placing " << numTroops << " troops." << std::endl;

	std::vector<std::pair<int, int>> actions;

    // obtain a vector<int> of the regions owned by player
    std::vector<int> regionsOwned = state.getRegionsOwnedByPlayer(myPlayerNumber);
    
    int index = 0;
    while (numTroops > 0)
    {
        actions.push_back(std::pair<int, int>(regionsOwned[index], 1));
        index = (index + 1) % regionsOwned.size();
        --numTroops;
    }

	if (beVerbose)  std::cout << "________________________________" << std::endl;
    return actions;
}

std::pair<int,int> BadStrategy1::attack(GameState state)
{
	std::cout<<"Player "<<myPlayerNumber<<": Attack."<<std::endl;
	if (movesDoneAttacking<1)
	{
		int attacking = myPlayerNumber;
		int attacked = 0;
		//Find first location that isn't occupied by yourself and go keel it.
		for(;attacked < state.getNumRegions() ; ++attacked)
		{
			if(state.getRegionInfo(attacked).first != attacking)
				break;
		}
		movesDoneAttacking++;
		return std::pair<int,int>(attacked,attacking);
	}
	else
	{
		movesDoneAttacking = 0;
		return std::pair<int,int>(-1,-1);	//let the turn be over
	}
}

bool BadStrategy1::defend(GameState state, int countryAttacked, int countryAttacking)
{
	//Trivial
	if (state.getRegionInfo(countryAttacked).second >= 2)
		return true;
	else
		return false;
}

std::vector<std::tuple<int,int,int> > BadStrategy1::fortify(GameState state)
{
	std::cout<<"Player "<<myPlayerNumber<<": Fortify."<<std::endl;
	state.display();
	std::vector<std::tuple<int,int,int>> actions;
	//int whereFrom = 0;
	//int whereTo = 0;
	//int howMany = 0;
	//actions.push_back(std::tuple<int,int,int>(whereFrom,whereTo,howMany));
	return actions;	//for now, let's do nothing
}
