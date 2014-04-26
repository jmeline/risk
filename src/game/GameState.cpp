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

std::vector<std::pair<int,int>> GameState::getAllExposedBorders(int playerNumber, GameMap* map)
{
	std::vector<std::pair<int,int>> exposedBorders;
	std::vector<int> regionList = getRegionsOwnedByPlayer(playerNumber);
	for (int i=0; i<regionList.size(); i++)
	{
		int numEnemyTroops = 0;
		std::vector<int> neighbors = map->getNeighborsOfRegion(regionList[i]);
		for (int j=0; j<neighbors.size(); j++)
		{
			if (getRegionInfo(neighbors[j]).first != playerNumber)
				numEnemyTroops += getRegionInfo(neighbors[j]).second;
		}
		if (numEnemyTroops > 0)
			exposedBorders.push_back(std::pair<int,int>(regionList[i],numEnemyTroops));
	}
	return exposedBorders;
}

std::vector<int> GameState::getAllNeighborsOfPlayer(int playerNumber, GameMap* map)
{
	std::vector<int> myNeighbors;
	for (int i=0; i<getNumRegions(); i++)
	{
		if (getRegionInfo(i).first != playerNumber)
		{
			std::vector<int> itsNeighbors = map->getNeighborsOfRegion(i);
			for (int j=0; j<itsNeighbors.size(); j++)
			{
				if (getRegionInfo(itsNeighbors[j]).first == playerNumber)
				{
					myNeighbors.push_back(i);
					break;
				}
			}
		}
	}
	return myNeighbors;
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
