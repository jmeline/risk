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

ObtainSmallestContinentsFirstStrategy::ObtainSmallestContinentsFirstStrategy()
{
}

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

    // sort the list from the lowest region count to the largest region count
    // std::vector<index> == index from lowest region count to largest region count
    // (1) region index
    // (2) region count
    std::vector<std::pair<int, int> > lowRegioncountList;
    std::cout << "continentList.size(): " << continentList.size() << std::endl;
    for (int i = 0; i < continentList.size(); i++)
    {
        lowRegioncountList.push_back(std::make_pair(i, continentList[i].getRegionList().size()));
    }

    // sort lowRegioncountList by the second element in the pair: region count. End result is a sorted list of the smallest number of regions 
    // to the largest number of regions. its original index is preserved as established in the map->continentList
    //std::sort(lowRegioncountList.begin(), lowRegioncountList.end(), sortByNumberOfRegions);
    std::sort(lowRegioncountList.begin(), lowRegioncountList.end(),
              [](const std::pair<int, int> &lhs, const std::pair<int, int> &rhs)
              {
                  return lhs.second < rhs.second; });

    // have we made a valid choice?
    bool placeSelected = false;

    int index = 0;
    int pickedContinent = -1;
    int pickedRegion = -1;
    std::vector<std::pair<int, std::string>> regionList;

    // select the first continent unless it is already taken.
    // If the country has been taken, grab the country with the next smallest region count
    while (!placeSelected && index < lowRegioncountList.size())
    {
        // grab the index of the continent with the lowest region count
        // countries with the lowest region count have been sorted from lowest to highest
        pickedContinent = lowRegioncountList[index].first;
        regionList = continentList[pickedContinent].getRegionList();

        int regionIndex = 0;

        // loop through each region within the continent for availability
        while (regionIndex < regionList.size())
        {
            pickedRegion = regionList[regionIndex].first;
            // the region is already claimed
            if (state.getRegionInfo(pickedRegion).first != -1)
            {
                // don't select this one
            }
            else
            {
                // select this continent (Found a winner)
                placeSelected = true;
                break;
            }

            regionIndex++;
        }

        if (placeSelected)
        {
            break;
        }

        index++;
    }

    std::cout << "Computer picked: " << pickedRegion << std::endl;

    // return the available country with the lowest region count
    return pickedRegion;

}

std::vector<std::pair<int, int>> ObtainSmallestContinentsFirstStrategy::place(GameState state, int numTroops)
{
    std::cout << "ObtainSmallestContinentsFirstStrategy computer is placing his troops" << std::endl;

    // obtain a vector<int> of the regions owned by player
    std::vector<int> regionsOwned = state.getRegionsOwnedByPlayer(myPlayerNumber);

    std::vector<std::pair<int, int>> action;
    int index = 0;

    // place troops in each location
    while (numTroops > 0)
    {
        action.push_back(std::pair<int, int>(regionsOwned[index], 1));
        index = (index + 1) % regionsOwned.size();
        --numTroops;
    }

    return action;
}

std::pair<int, int> ObtainSmallestContinentsFirstStrategy::attack(GameState state)
{
    std::cout << "ObtainSmallestContinentsFirstStrategy computer is placing his troops" << std::endl;

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


