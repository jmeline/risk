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
    Prey on Weak
    Kill the easiest target every time

Claim: Randomly
		
Place: On strongest myRegion that borders the weakest enemy region
		
Attack: Always go for best odds of immediate victory (inspects neighbors)
	"The weak" can mean 1) a player almost to be illuminated or 2) a region easily conquered
	There exists a funciton in Game.cpp that will get victory probability for you
		
Fortify: If not touching any enemy, move to random neighbor (code for this exists, recycle it)

**************************************************/

PreyOnWeakStrategy::PreyOnWeakStrategy()
{
	movesDoneAttacking = 0;
}


int PreyOnWeakStrategy::claim(GameState state)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
    if (beVerbose)  std::cout << "PreyOnWeakStrategy "<<myPlayerNumber<<" is claiming" << std::endl;
	if (beVerbose)  state.display();

	std::uniform_int_distribution<int> distrubution(0, state.getNumRegions());
	while (true) {
		int chosen = distrubution(rng);
		if(state.getRegionInfo(chosen).first == -1)
			return chosen;
	}
}

std::vector<std::pair<int,int>> PreyOnWeakStrategy::place(GameState state, int numTroops)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
	if (beVerbose)  std::cout << "PreyOnWeakStrategy " << myPlayerNumber << " is placing " << numTroops << " troops." << std::endl;

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

std::pair<int,int> PreyOnWeakStrategy::attack(GameState state)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
    if (beVerbose)  std::cout << "PreyOnWeakStrategy "<<myPlayerNumber<<" is attacking" << std::endl;
	if (beVerbose)  state.display();

	//sort neighbors by strength
	std::vector<std::pair<int,int>> neighborStrength;		//first is where to attack, second is their strength
	std::vector<int> myNeighbors = state.getAllNeighborsOfPlayer(myPlayerNumber, map);
	for (int i=0; i<myNeighbors.size(); i++)
		neighborStrength[i] = std::pair<int,int>(myNeighbors[i],state.getRegionInfo(myNeighbors[i]).second);
	std::sort(neighborStrength.begin(), neighborStrength.end(),
		[](const std::pair<int,int> &lhs, const std::pair<int,int> &rhs)
			{ return lhs.second < rhs.second; });

	//attack the weakest that we can
	for (int i=0; i<neighborStrength.size(); i++) {
		std::vector<int> theirNeighbors = map->getNeighborsOfRegion(neighborStrength[i].first);
		for (int attackFromIndex = 0; attackFromIndex<theirNeighbors.size(); attackFromIndex++) {
			std::pair<int,int> attackFromInfo = state.getRegionInfo(theirNeighbors[attackFromIndex]);
			if (attackFromInfo.first==myPlayerNumber && attackFromInfo.second>3)
				return std::pair<int,int>(attackFromIndex, neighborStrength[i].first);
		}
	}

	//if we made it this far, there's no good place to attack.  Let the turn be over
	return std::pair<int,int>(-1,-1);
}

std::vector<std::tuple<int,int,int> > PreyOnWeakStrategy::fortify(GameState state)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
	if (beVerbose)  std::cout << "PreyOnWeakStrategy "<<myPlayerNumber<<" is fortifying" << std::endl;
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
