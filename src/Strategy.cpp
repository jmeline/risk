#include "Strategy.hpp"
#include <iostream>


/*************************
* HUMAN CONTROLLED
*************************/

HumanControlledStrategy::HumanControlledStrategy()
{
}

int HumanControlledStrategy::claim(GameState state)
{
	std::cout<<"Player "<<myPlayerNumber<<": Claim a region."<<std::endl;
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
		outOfBounds = whichRegion < 0 || whichRegion > (state.getNumRegions()-1);
		if(!outOfBounds)
			alreadyClaimed = state.getRegionInfo(whichRegion).first != -1;

		if(outOfBounds)
			std::cout << "You have to claim somewhere ON the map." << std::endl;
		if(alreadyClaimed)
			std::cout << "You have to choose unclaimed territory." << std::endl;
	}while(alreadyClaimed || outOfBounds);

	// I think we checked all the dumb mistakes the user can make
	return whichRegion;
}
	
std::vector<std::pair<int,int>> HumanControlledStrategy::place(GameState state, int numTroops)
{
	std::vector<std::pair<int,int>> actions;
	int wherePut = 0;
	int howMany = 0;
	std::cout<<"Player "<<myPlayerNumber<<": Place your troops."<<std::endl;
	std::cout<<"You have " << numTroops << " to place." << std::endl;
	std::cout << "Where would you like to place troops?";
	std::cin >> wherePut;
	// Bounds checking loop
	while(wherePut < 0 || wherePut > (state.getNumRegions()-1) || state.getRegionInfo(wherePut).first != myPlayerNumber)
	{
		std::cout << "You can't put troops there..." << std::endl;
		std::cout << "Where would you like to place troops?";
		std::cin >> wherePut;
	}

	actions.push_back(std::pair<int,int>(wherePut,howMany));
	return actions;
}

std::pair<int,int> HumanControlledStrategy::attack(GameState state)
{
	std::cout<<"Player "<<myPlayerNumber<<": Attack."<<std::endl;
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
			return std::pair<int,int>(-1,-1);	//let the turn be over

		// Set flags (error check)
		outOfBounds = location >= state.getNumRegions();
		if(!outOfBounds)
			attackingSelf = state.getRegionInfo(location).first == myPlayerNumber;

		if(outOfBounds)
			std::cout << "You must select a valid location to attack." << std::endl;
		if(attackingSelf)
			std::cout << "You can't attack yourself, dummy." << std::endl;
	}while(outOfBounds || attackingSelf);


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
/*
ObtainSmallestContinentsFirstStrategy::ObtainSmallestContinentsFirstStrategy()
{

}

int ObtainSmallestContinentsFirstStrategy::claim(GameState state)
{
	
	return 0;
}

std::vector<std::pair<int,int>> ObtainSmallestContinentsFirstStrategy::place(GameState state, int numTroops)
{

	return std::vector<std::pair<int,int>>();
}

std::pair<int,int> ObtainSmallestContinentsFirstStrategy::attack(GameState state)
{
	return std::pair<int,int>();
}
bool ObtainSmallestContinentsFirstStrategy::defend(GameState state, int countryAttacked, int countryAttacking)
{
	return false;
}

std::vector<std::tuple<int,int,int> > ObtainSmallestContinentsFirstStrategy::fortify(GameState state)
{
	return std::vector<std::tuple<int,int,int> >();
{

*/

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
