/* 
 * File:   ThreeContinent.cpp
 * Author: Joseph
 *
 * A location for testing
 */

#include    "../gamemap/GameMap.hpp"
enum Regions
{
	//c1
	C1R1,
	C1R2,
	C1R3,
	//c2
	C2R1,
	C2R2,
	C2R3,
	C2R4,
	//c3
	C3R1,
	C3R2
};

const char* ThreeRegionStrings[] = 
{
	"C1R1",
	"C1R2",
	"C1R3",
	"C2R1",
	"C2R2",
	"C2R3",
	"C2R4",
	"C3R1",
	"C3R2"
};


ThreeContinent::ThreeContinent():GameMap(8,1)  // startingTroops,perPlayerTroopPenalty
{
	initialize();
}

void ThreeContinent::constructContinentsAndRegions()
{
	Continent c1("Continent1", 4);
	c1.addRegion(std::make_pair(C1R1, ThreeRegionStrings[C1R1]));
	c1.addRegion(std::make_pair(C1R2, ThreeRegionStrings[C1R2]));
	c1.addRegion(std::make_pair(C1R3, ThreeRegionStrings[C1R3]));
	continentList.push_back(c1);

	Continent c2("Continent1", 5);
	c2.addRegion(std::make_pair(C2R1, ThreeRegionStrings[C1R1]));
	c2.addRegion(std::make_pair(C2R2, ThreeRegionStrings[C2R2]));
	c2.addRegion(std::make_pair(C2R3, ThreeRegionStrings[C2R3]));
	c2.addRegion(std::make_pair(C2R4, ThreeRegionStrings[C2R4]));
	continentList.push_back(c2);

	Continent c3("Continent3", 2);
	c3.addRegion(std::make_pair(C3R1, ThreeRegionStrings[C3R1]));
	c3.addRegion(std::make_pair(C3R2, ThreeRegionStrings[C3R2]));
	continentList.push_back(c3);
}

void ThreeContinent::constructBorders()
{
	makeBorder(C1R1,C1R2);
	makeBorder(C1R2,C1R3);
	makeBorder(C1R3,C1R1);

	makeBorder(C2R1,C2R2);
	makeBorder(C2R2,C2R3);
	makeBorder(C2R3,C2R1);
	makeBorder(C2R4,C2R3);
	makeBorder(C2R4,C2R2);

	makeBorder(C3R1,C3R2);

	makeBorder(C3R1,C2R2);
	makeBorder(C1R3,C2R1);
	makeBorder(C1R2,C2R2);
}