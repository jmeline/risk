/*
 * =====================================================================================
 *       Filename:  GameState.cpp
 *    Description: Represents the state of a game
 * =====================================================================================
 */

#include "GameState.hpp"
#include <vector>

GameState::GameState(int size)
{
    internalState = std::vector<std::pair<int, int>>(size);
    for (int i = 0; i < size; i++)
        internalState[i] = std::pair<int, int>(-1, 0);
}

GameState::GameState(GameMap *map)
{
    int size = map->getNumberOfRegions();
    internalState = std::vector<std::pair<int, int>>(size);
    for (int i = 0; i < size; i++)
        internalState[i] = std::pair<int, int>(-1, 0);
}

GameState::GameState()
{
	internalState = std::vector<std::pair<int, int>>(1);
	internalState[0] = std::pair<int,int>(-1,0);
}

std::pair<int, int> GameState::getRegionInfo(int region)
{
    return internalState[region];
}

void GameState::setRegionInfo(int region, std::pair<int, int> regionInfo)
{
    internalState[region] = regionInfo;
}

int GameState::getNumberOccupiedBy(int player)
{
    int count = 0;
    for (int i = 0 ; i < internalState.size() ; ++i)
    {
        if (internalState[i].first == player)
            count++;
    }
    return count;
    //
}


std::vector<int> GameState::getRegionsOwnedByPlayer(int playerNumber)
{
    std::vector<int> regionsOwned;
    //for (std::pair<int, int> p : internalState)
    for( int i = 0; i < internalState.size(); i++)
    {
        if (internalState[i].first == playerNumber)
        {
            regionsOwned.push_back(i);
        }
    }

    return regionsOwned;
}



void GameState::display()
{
    std::cout << "Location:Owner:Number of Troops" << std::endl;
    for (int i = 0; i < internalState.size(); ++i)
    {
        std::cout << "    " << i << ":" << internalState[i].first << ":" << internalState[i].second;
        std::cout << std::endl;
    }
}

int GameState::getNumRegions()
{
    return internalState.size();
}
