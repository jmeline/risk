/*
 * =====================================================================================
 *       Filename:  GameState.cpp
 *    Description: Represents the state of a game
 * =====================================================================================
 */

#include "GameState.hpp"

GameState::GameState(int size)
{
	internalState = std::vector<std::pair<int,int>>(size);
	for (int i=0; i<size; i++)
		internalState[i] = std::pair<int,int>(-1,0);
}

GameState::GameState(GameMap* map)
{
	int size = map->getNumberOfRegions();
	internalState = std::vector<std::pair<int,int>>(size);
	for (int i=0; i<size; i++)
		internalState[i] = std::pair<int,int>(-1,0);
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
	for each (std::pair<int,int> p in internalState)
	{
		if (p.first==player)
			count++;
	}
	return count;
	//
}
