#include    "GameMap.hpp"

GameMap* GameMap::make(MapEnum::MapEnum mapType)
{
	switch(mapType)
	{
		case (MapEnum::Earth):
			return (GameMap*)(new Earth());
		case (MapEnum::Island):
			return (GameMap*)(new Island());
		case (MapEnum::ThreeContinent):
			return (GameMap*)(new ThreeContinent());
		default:
			return (GameMap*)(new Earth());
	}
}


void GameMap::initialize()
{
    constructContinentsAndRegions();
    int regionCount = getNumberOfRegions();
    borderMatrix = std::vector<std::vector<bool>>(regionCount);
    for (int i = 0; i < regionCount; i++)
        borderMatrix[i] = std::vector<bool>(regionCount, false);
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
    for (int i = 0 ; i < continentList.size() ; ++i)
        count += continentList[i].getRegionList().size();
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

int GameMap::whereIs(int region) {
	for (int i=0; i<continentList.size(); i++)
	{
		std::vector<std::pair<int,std::string>> regionList = continentList[i].getRegionList();
		for (int j=0; j<regionList.size(); j++)
		{
			if (regionList[j].first == region)
				return i;
		}
	}
	return -1;
}

void GameMap::makeBorder(int i, int j)
{
    this->borderMatrix[i][j] = true;
    this->borderMatrix[j][i] = true;
}
