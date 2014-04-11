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
	for( ; whichRegion < state.getNumRegions() ; ++whichRegion)
	{
		if(state.getRegionInfo(whichRegion).first == -1)
			break;
	}
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
	if (movesDoneAttacking<5)
	{
		int attacking = 0;
		int attacked = 0;
		movesDoneAttacking++;
		return std::pair<int,int>(attacked,attacking);
	}
	else
	{
		movesDoneAttacking = 0;
		return std::pair<int,int>(-1,-1);	//let the turn be over
	}
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
