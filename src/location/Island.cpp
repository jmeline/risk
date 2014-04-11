
/* 
 * File:   Island.cpp
 * Author: Jacob
 *
 * Test location 
 */

#include 	<iostream>
#include    "../gamemap/GameMap.hpp"
#include    "../gamemap/Continent.hpp"
#include    <string>
#include    <utility>
#include    <vector>

#include 	"Island.hpp"

enum Regions
{
	//Paradise
	Beach,
	Forest,
	Garden,
	Volcano
};

const char* RegionStrings[] = 
{
	"Beach",
	"Forest",
	"Garden",
	"Volcano"

};

Island::Island()
{
	initialize();
}

void Island::constructContinentsAndRegions()
{
	//Paradise
	Continent paradise("Paradise", 5);
	paradise.addRegion(std::make_pair(Beach, RegionStrings[Beach]));
	paradise.addRegion(std::make_pair(Forest, RegionStrings[Forest]));
	paradise.addRegion(std::make_pair(Garden, RegionStrings[Garden]));
	paradise.addRegion(std::make_pair(Volcano, RegionStrings[Volcano]));
	ContinentList.push_back(paradise);
}

void Island::constructBorders()
{
	makeBorder(Volcano, Forest);
	makeBorder(Volcano, Beach);
	makeBorder(Garden, Forest);
	makeBorder(Garden, Beach);
}