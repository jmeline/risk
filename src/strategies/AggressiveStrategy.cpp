#include "../game/Strategy.hpp"
#include <vector>
#include <iostream>
#include <algorithm>	// std::sort
#include <utility>		//std::pair
#include <string>
#include <random>

extern std::default_random_engine rng;

/**************************************************

            COMPUTER CONTROLLED
    Agressive
    Attack any time possible, starting with our strongest army

Claim: Randomly

Place: Evenly distribute, anywhere touching an enemy

Attacking: Pick my region with most troops first, attack anyone.  ALWAYS attack if you can, never hold back

Fortify: If not touching any enemy, move to random neighbor (code for this exists, recycle it)

**************************************************/

AggressiveStrategy::AggressiveStrategy() {}


int AggressiveStrategy::claim(GameState state)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
    if (beVerbose)  std::cout << "AggressiveStrategy "<<myPlayerNumber<<" is claiming" << std::endl;
	if (beVerbose)  state.display();

	std::uniform_int_distribution<int> distrubution(0, state.getNumRegions());
	while (true) {
		int chosen = distrubution(rng);
		if(state.getRegionInfo(chosen).first == -1) {
			if (beVerbose)  std::cout<<"Claiming "<<chosen<<std::endl;
			return chosen;
		}
	}
}


std::vector<std::pair<int,int>> AggressiveStrategy::place(GameState state, int numTroops)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
	if (beVerbose)  std::cout << "AggressiveStrategy " << myPlayerNumber << " is placing " << numTroops << " troops." << std::endl;
	if (beVerbose)  state.display();

	std::vector<std::pair<int, int>> actions;

    // get all battlefront territories
	std::vector<std::pair<int,int>> bordersOwned = state.getAllExposedBorders(myPlayerNumber, map);
	std::shuffle(bordersOwned.begin(), bordersOwned.end(), rng);
	int howManyPerPlace = numTroops / bordersOwned.size();
	int remainder = numTroops % bordersOwned.size();
    for (int i=0; i<bordersOwned.size(); i++) {
		int howManyHere = howManyPerPlace+(i<remainder?1:0);
		if (beVerbose)  std::cout<<"Placing "<<howManyHere<<" troops at "<<bordersOwned[i].first<<std::endl;
		actions.push_back(std::pair<int, int>(bordersOwned[i].first, howManyHere));
	}

	if (beVerbose)  std::cout << "________________________________" << std::endl;
    return actions;
}


std::pair<int,int> AggressiveStrategy::attack(GameState state)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
    if (beVerbose)  std::cout << "AggressiveStrategy "<<myPlayerNumber<<" is attacking" << std::endl;
	if (beVerbose)  state.display();

	// get all battlefront territories, sorted by number of troops
	std::vector<std::pair<int,int>> bordersOwned = state.getAllExposedBorders(myPlayerNumber, map);
	std::sort(bordersOwned.begin(), bordersOwned.end(),
		[](const std::pair<int,int> &lhs, const std::pair<int,int> &rhs)
			{ return lhs.second < rhs.second; });

	//see if we don't have enough troops anywhere to attack
	int attackFrom = bordersOwned[bordersOwned.size()-1].first;
	if (beVerbose)  std::cout<<"Will attack from "<<attackFrom<<std::endl;
	if (state.getRegionInfo(attackFrom).second<2) {
		if (beVerbose)  std::cout<<"Not attacking"<<std::endl;
		return std::pair<int,int>(-1,-1);
	}

	//then pick a place to attack
	std::vector<int> targets = map->getNeighborsOfRegion(attackFrom);
	std::uniform_int_distribution<int> distrubution(0, targets.size());
	while (true) {
		int chosen = distrubution(rng);
		if (state.getRegionInfo(targets[chosen]).first != myPlayerNumber) {
			if (beVerbose)  std::cout<<"Attacking "<<targets[chosen]<<" from "<<attackFrom<<std::endl;
			return std::pair<int,int>(attackFrom, targets[chosen]);
		}
	}
}


std::vector<std::tuple<int,int,int> > AggressiveStrategy::fortify(GameState state)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
	if (beVerbose)  std::cout << "AggressiveStrategy "<<myPlayerNumber<<" is fortifying" << std::endl;
    if (beVerbose)  state.display();

	std::vector<std::tuple<int, int, int>> actions;

	std::vector<int> myRegions = state.getRegionsOwnedByPlayer(myPlayerNumber);
	for (int i=0; i<myRegions.size(); i++)
	{
		std::vector<int> neighbors = map->getNeighborsOfRegion(myRegions[i]);
		bool isOnBattleFront = false;
		for (int j=0; j<neighbors.size(); j++)
		{
			if (state.getRegionInfo(neighbors[j]).first != myPlayerNumber)
			{
				isOnBattleFront = true;
				break;
			}
		}
		if (!isOnBattleFront)
		{
			//move it, randomly, hoping to find the battlefront
			int whereFrom = myRegions[i];
			int whereTo = neighbors[rand() % neighbors.size()];
			int howManyToMove = state.getRegionInfo(whereFrom).second - 1;
			if (howManyToMove > 0)
			{
				if (beVerbose)  std::cout<<"Moving "<<howManyToMove<<" from "<<whereFrom<<" to "<<whereTo<<std::endl;
				actions.push_back(std::tuple<int,int,int>(whereFrom,whereTo,howManyToMove));
			}
		}
	}

	if (beVerbose)  std::cout << "________________________________" << std::endl;
	return actions;
}
