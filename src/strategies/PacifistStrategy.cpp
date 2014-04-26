#include "../game/Strategy.hpp"
#include <vector>
#include <iostream>
#include <algorithm>	// std::sort
#include <utility>		//std::pair
#include <string>

/**************************************************

            COMPUTER CONTROLLED
    Pacifist
    Seeks to coexist

Rules for claiming regions:
	-Go for continents in order of decreasing size, claiming a "random" region if availiable (in-order works fine for "random").

Rules for placing:
	1) Place on a random owned spot.  You don't care where it goes, you just want to be free.

Rules for attacking:
	1) Don't do it

Rules for fortifying:
	-If a region is surrounded only by friendly regions, move it's troops into a random one of those regions (eventually getting to the battlefront).

**************************************************/

PacifistStrategy::PacifistStrategy()
{
	movesDoneAttacking = 0;
}


int PacifistStrategy::claim(GameState state)
{
	int whichRegion = 0;
	for( ; whichRegion < state.getNumRegions() ; ++whichRegion)
	{
		if(state.getRegionInfo(whichRegion).first == -1)
			break;
	}
	return whichRegion;
}
	
std::vector<std::pair<int,int>> PacifistStrategy::place(GameState state, int numTroops)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
	if (beVerbose)  std::cout << "PacifistStrategy " << myPlayerNumber << " is placing " << numTroops << " troops." << std::endl;

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

std::pair<int,int> PacifistStrategy::attack(GameState state)
{
	if(beVerbose) std::cout<<"PacifistStrategy "<<myPlayerNumber<<": Attacking"<<std::endl;

	return std::pair<int,int>(-1,-1);	//let the turn be over
}

std::vector<std::tuple<int,int,int> > PacifistStrategy::fortify(GameState state)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
	if (beVerbose)  std::cout << "PacifistStrategy "<<myPlayerNumber<<" is fortifying" << std::endl;
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
