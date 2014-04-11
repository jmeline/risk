#include "Strategy.hpp"
#include <iostream>


/*************************
* HUMAN CONTROLLED
*************************/

HumanControlledStrategy::HumanControlledStrategy()
{
	movesDoneAttacking = 0;
}

int HumanControlledStrategy::claim(GameState state)
{
	std::cout<<"Player "<<myPlayerNumber<<": Claim a region."<<std::endl;
	state.display();
	int whichRegion = 0;
	std::cout << "Which region would you like to claim? ";
	std::cin >> whichRegion;
	// Bounds checking loop
	while(whichRegion < 0 || whichRegion > (state.getNumRegions()-1))
	{
		std::cout << "You can't claim that..." << std::endl;
		std::cout << "Which region would you like to claim? ";
		std::cin >> whichRegion;
	}
	// Disallow claiming already claimed regions
	while(state.getRegionInfo(whichRegion).first != -1)
	{
		std::cout << "That location is already occupied, choose another location: ";
		std::cin >> whichRegion;
	}
	// I think we checked all the dumb mistakes the user can make
	return whichRegion;
}
	
std::vector<std::pair<int,int>> HumanControlledStrategy::place(GameState state, int numTroops)
{
	std::cout<<"Player "<<myPlayerNumber<<": Place your troops."<<std::endl;
	std::vector<std::pair<int,int>> actions;
	int wherePut = 0;
	int howMany = numTroops;
	actions.push_back(std::pair<int,int>(wherePut,howMany));
	return actions;
}

std::pair<int,int> HumanControlledStrategy::attack(GameState state)
{
	std::cout<<"Player "<<myPlayerNumber<<": Attack."<<std::endl;
	state.display();
	std::cout << "Which country do you want to attack (-1 to stop this phase)? ";
	int location = 0;
	std::cin >> location;

	// Check for quit FIRST  (cuz bounds check on this can get hairy)
	if (location < 0)
		return std::pair<int,int>(-1,-1);	//let the turn be over

	// Bounds check
	while(location >= state.getNumRegions())
	{
		std::cout << "You must select a valid location to attack." << std::endl;
		std::cout << "Which country do you want to attack (-1 to stop this phase)? ";
		std::cin >> location;
	}
	while(state.getRegionInfo(location).first == myPlayerNumber)
	{
		std::cout << "You can't attack yourself, dummy." << std::endl;
		std::cout << "Which country do you want to attack (-1 to stop this phase)? ";
		std::cin >> location;
	}


	return std::pair<int,int>(location,myPlayerNumber);
}

bool HumanControlledStrategy::defend(GameState state, int countryAttacked, int countryAttacking)
{
	//Sufficiently trivial that we don't involve the human.
	if (state.getRegionInfo(countryAttacked).second >= 2)
		return true;
	else
		return false;
}

std::vector<std::tuple<int,int,int> > HumanControlledStrategy::fortify(GameState state)
{
	std::cout<<"Player "<<myPlayerNumber<<": Fortify."<<std::endl;
	std::vector<std::tuple<int,int,int>> actions;
	//int whereFrom = 0;
	//int whereTo = 0;
	//int howMany = 0;
	//actions.push_back(std::tuple<int,int,int>(whereFrom,whereTo,howMany));
	return actions;	//for now, let's do nothing
}

/**************************************************
			COMPUTER CONTROLLED
	ObtainSmallestContinentsFirstStrategy
**************************************************/

ObtainSmallestContinentsFirstStrategy::ObtainSmallestContinentsFirstStrategy()

ObtainSmallestContinentsFirstStrategy::claim(GameState state)
{
	
}

/*************************
* BAD STRATEGY
*************************/

BadStrategy::BadStrategy()
{
	movesDoneAttacking = 0;
}


int BadStrategy::claim(GameState state)
{
	int whichRegion = 0;
	for( ; whichRegion < state.getNumRegions() ; ++whichRegion)
	{
		if(state.getRegionInfo(whichRegion).first == -1)
			break;
	}
	return whichRegion;
}
	
std::vector<std::pair<int,int>> BadStrategy::place(GameState state, int numTroops)
{
	std::cout<<"Player "<<myPlayerNumber<<": Place your troops."<<std::endl;
	std::vector<std::pair<int,int>> actions;
	int wherePut = 0;
	int howMany = numTroops;
	actions.push_back(std::pair<int,int>(wherePut,howMany));
	return actions;
}

std::pair<int,int> BadStrategy::attack(GameState state)
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

bool BadStrategy::defend(GameState state, int countryAttacked, int countryAttacking)
{
	//Trivial
	if (state.getRegionInfo(countryAttacked).second >= 2)
		return true;
	else
		return false;
}

std::vector<std::tuple<int,int,int> > BadStrategy::fortify(GameState state)
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
