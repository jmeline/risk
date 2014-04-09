#include "HumanControlledStrategy.hpp"
#include <iostream>

HumanControlledStrategy::HumanControlledStrategy()
{
	movesDoneAttacking = 0;
}

int HumanControlledStrategy::claim(GameState state)
{
	std::cout<<"Player "<<myPlayerNumber<<": Claim a region."<<std::endl;
	int whichRegion = 0;
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
