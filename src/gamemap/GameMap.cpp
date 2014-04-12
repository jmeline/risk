/*
 * =====================================================================================
 *
 *       Filename:  gamemap.cpp
 *
 *    Description:  handles the game map
 *
 *        Version:  1.0
 *        Created:  03/29/2014 11:08:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jacob Meline 
 * =====================================================================================
 */

#include    <vector>
#include    <utility>
#include    <string>
#include    "GameMap.hpp"
#include    "Continent.hpp"

/*
 *--------------------------------------------------------------------------------------
 *       Class:  GameMap
 *      Method:  GameMap :: GameMap
 * Description:  Constructor
 *--------------------------------------------------------------------------------------
 */
void GameMap::initialize()
{
	constructContinentsAndRegions();
	int regionCount = getNumberOfRegions();
	borderMatrix = std::vector<std::vector<bool>>(regionCount);
	for (int i=0; i<regionCount; i++)
		borderMatrix[i] = std::vector<bool>(regionCount,false);
    constructBorders();
}

std::vector<Continent> GameMap::getContinentList()
{
	return this->continentList;
}

void GameMap::setContinentList(std::vector<Continent> c)
{
    this->continentList = c;
}

int GameMap::getNumberOfRegions()
{
	int count = 0;
	int continentCount = continentList.size();
	for each (Continent c in continentList)
		count += c.getRegionList().size();
	return count;
}

std::vector<int> GameMap::getNeighborsOfRegion(int region)
{
	std::vector<int> neighborList;
	for (int j=0; j<(this->borderMatrix[region].size()); j++)
	{
		if (borderMatrix[region][j]==true)
			neighborList.push_back(j);
	}
	return neighborList;
}

bool GameMap::areConnected(int region1, int region2)
{
	return borderMatrix[region1][region2];
}

void GameMap::makeBorder(int i, int j)
{
    this->borderMatrix[i][j] = true;
    this->borderMatrix[j][i] = true;
}
