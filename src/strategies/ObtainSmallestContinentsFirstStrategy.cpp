#include "../game/Strategy.hpp"
#include <vector>
#include <iostream>
#include <algorithm> // std::sort
#include <utility> //std::pair
#include <string>

/**************************************************

            COMPUTER CONTROLLED
    ObtainSmallestContinentsFirstStrategy
    Seeks to get the smallest Continent first

**************************************************/

ObtainSmallestContinentsFirstStrategy::ObtainSmallestContinentsFirstStrategy() {}


/* 
bool ObtainSmallestContinentsFirstStrategy::sortByNumberOfRegions(
    const std::pair<int,int> &lhs, const std::pair<int,int> &rhs)
{
    return lhs.second < rhs.second;
}
*/

int ObtainSmallestContinentsFirstStrategy::claim(GameState state)
{
	std::cout << "ObtainSmallestContinentsFirstStrategy Computer is claiming his land" << std::endl;

    // get list of continents
    std::vector<Continent> continentList = this->map->getContinentList();

    std::cout << "Printing continentList" << std::endl;
    for each (Continent c in continentList)
    {
        std::cout << "value: " << c.getValue() << std::endl;
        std::cout << "name: " << c.getContinentName() << std::endl;
        std::cout << "regions: " << std::endl;
        for each (std::pair<int, std::string> r in c.getRegionList())
        {
            std::cout << r.first << "->" << r.second << std::endl;
        }

    }

    std::cout << std::endl;

    // sort the list from the lowest region count to the largest region count
    // std::vector<index> == index from lowest region count to largest region count
    // (1) region index
    // (2) region count
    std::vector<std::pair<int, int> > lowRegioncountList;
    std::cout << "continentList.size(): " << continentList.size() << std::endl;
    for ( int i = 0; i < continentList.size(); i++)
    {
        lowRegioncountList.push_back(std::make_pair(i, continentList[i].getRegionList().size()));
    }

    std::cout << "Printing lowRegioncountList (" << lowRegioncountList.size() << ") " << std::endl;
    for each (std::pair<int,int> p in lowRegioncountList)
    {
        std::cout << p.first << " size=" << p.second << std::endl;
    }
    std::cout << std::endl;
    

    // sort lowRegioncountList by the second element in the pair: region count. End result is a sorted list of the smallest number of regions 
    // to the largest number of regions. its original index is preserved as established in the map->continentList
    //std::sort(lowRegioncountList.begin(), lowRegioncountList.end(), sortByNumberOfRegions);
    std::sort(lowRegioncountList.begin(), lowRegioncountList.end(), 
    	[](const std::pair<int,int> &lhs, const std::pair<int,int> &rhs)
    	{ return lhs.second < rhs.second; } );

    
    std::cout << "Printing Sorted lowRegioncountList(" << lowRegioncountList.size() << ") " << std::endl;
    for each (std::pair<int,int> p in lowRegioncountList)
    {
        std::cout << p.first << " size=" << p.second << std::endl;
    }
    std::cout << std::endl;

    bool alreadyClaimed = false;
    int index = 0;
    int pickedRegion = -1;

    // select the first continent unless it is already taken.
    // If the country has been taken, grab the country with the next smallest region count
    while (index < lowRegioncountList.size() )
    {
        std::cout << "Index: " << index << std::endl;
    	// grab the index of the continent with the lowest region count
    	// countries with the lowest region count have been sorted from lowest to highest
    	pickedRegion = lowRegioncountList[index].first;
        std::cout << "pickedRegion: " << pickedRegion << std::endl;

    	// check if that 
        alreadyClaimed = state.getRegionInfo(pickedRegion).first != -1 &&
            state.getRegionInfo(pickedRegion).first != myPlayerNumber;

        std::cout << "Claimed? " << (alreadyClaimed ? "True" : "False") << std::endl;
        // the region is already claimed
        if (alreadyClaimed)
        {
        	// don't select this one
        }
        else
        {
            // select this continent (Found a winner)
            break;
        }

        index++;
    }

    if (pickedRegion == -1)
    {
    	std::cout << "ERROR! PickedRegion is still -1" << std::endl;
    }
    else{
        std::cout << "Computer picked: " << pickedRegion << std::endl;
    }
	
	// return the available country with the lowest region count
    return pickedRegion;

}

std::vector<std::pair<int, int>> ObtainSmallestContinentsFirstStrategy::place(GameState state, int numTroops)
{
	std::cout << "ObtainSmallestContinentsFirstStrategy computer is placing his troops" << std::endl;

    // obtain a vector<int> of the regions owned by player
    std::vector<int> regionsOwned = state.getRegionsOwnedByPlayer(myPlayerNumber);

    std::cout << "regionsOwned size: " << regionsOwned.size() << std::endl;
    for each (int i in regionsOwned)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

	// place troops in each location
	while( numTroops > 0 )
	{
		// 
	}


    return std::vector<std::pair<int, int>>();
}

std::pair<int, int> ObtainSmallestContinentsFirstStrategy::attack(GameState state)
{
    return std::pair<int, int>();
}
bool ObtainSmallestContinentsFirstStrategy::defend(GameState state, int countryAttacked, int countryAttacking)
{
    return false;
}

std::vector<std::tuple<int, int, int> > ObtainSmallestContinentsFirstStrategy::fortify(GameState state)
{
    return std::vector<std::tuple<int, int, int> >();
}


