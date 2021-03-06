/* 
 * File:   Island.cpp
 * Author: Jacob
 *
 * A location for testing
 */

#include    "../gamemap/GameMap.hpp"

enum Regions
{
	//Paradise
	Beach,
	Forest,
	Garden,
	Volcano
};

const char* IslandStrings[] = 
{
	"Beach",
	"Forest",
	"Garden",
	"Volcano"

};

Island::Island():GameMap(10,2)  // startingTroops,perPlayerTroopPenalty
{
	initialize();
}

void Island::constructContinentsAndRegions()
{
	//Paradise
	Continent paradise("Paradise", 5);
	paradise.addRegion(std::make_pair(Beach, IslandStrings[Beach]));
	paradise.addRegion(std::make_pair(Forest, IslandStrings[Forest]));
	paradise.addRegion(std::make_pair(Garden, IslandStrings[Garden]));
	paradise.addRegion(std::make_pair(Volcano, IslandStrings[Volcano]));
	continentList.push_back(paradise);
}

void Island::constructBorders()
{
	makeBorder(Volcano, Forest);
	makeBorder(Volcano, Beach);
	makeBorder(Garden, Forest);
	makeBorder(Garden, Beach);
}