/*
 * =====================================================================================
 *
 *       Filename:  gamemap.hpp
 *
 *    Description:  Contains:
						1) An enum of all types of GameMaps, by which they can be identified
						2) The definintion of a GameMap
							a list of continents and their respective bordering regions
						3) The definitions of the types of GameMaps
 *                  
 *
 *        Version:  1.0
 *        Created:  03/29/2014 01:32:59 AM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Jacob Meline, Joseph Richardson
 *
 * =====================================================================================
 */

#ifndef  gamemap_INC
#define  gamemap_INC

#include "Continent.hpp"
#include <vector>
#include <utility>
#include <string>

namespace MapEnum
{
	enum MapEnum
	{
		Earth,
		Island
	};
}




/*
 * =====================================================================================
 *        Class:  GameMap
 *  Description:  Handles the functionality of the game map 
 * =====================================================================================
 */
class GameMap
{
protected:
	/* ====================  MEMBER VARIABLES  ======================================= */
    std::vector<Continent> continentList;			/* list of continents */
    std::vector<std::vector<bool> > borderMatrix;	/* list of borders */

public:
	/* ====================  LIFECYCLE METHODS FOR PUBLIC USE  ======================================= */
	
	/* To be called in the implementations' constructors */
	void initialize();

    /* Returns the list of all Continents on the map */
    std::vector<Continent> getContinentList();

    void setContinentList(std::vector<Continent> c);

    /* Returns the number of regions on the map */
    int getNumberOfRegions();

    /* Returns a list of all regions bordering a given region */
    std::vector<int> getNeighborsOfRegion(int region);

	/* Indicates whether two regions touch */
	bool areConnected(int region1, int region2);

    /* ====================  METHODS FOR SPECIFIC INSTANCES TO OVERRIDE ======================================= */
    virtual MapEnum::MapEnum getIdentifier() = 0;		/* uniquely identifies the specific GameMap */
	virtual void constructContinentsAndRegions() = 0;	/* builds each continent */
    virtual void constructBorders() = 0;				/* builds each border relationship with each region (by repeatedly calling makeBorder())*/

	/* ====================  METHODS USED AS HELPERS FOR SPECIFIC INSTANCES =================================== */
    void makeBorder(int i, int j);

}; /* -----  end of class GameMap  ----- */




/*
 * BELOW HERE are the headers for all the GameMap types
*/


class Earth : public GameMap
{
public:
    Earth();
	MapEnum::MapEnum getIdentifier() { return MapEnum::Earth; }
    void constructContinentsAndRegions();
    void constructBorders();
};

class Island : public GameMap
{
public:
    Island();
	MapEnum::MapEnum getIdentifier() { return MapEnum::Island; }
    void constructContinentsAndRegions();
    void constructBorders();
};


#endif   /* ----- #ifndef gamemap_INC  ----- */

