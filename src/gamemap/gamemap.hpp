/*
 * =====================================================================================
 *
 *       Filename:  gamemap.hpp
 *
 *    Description:  Contains a list of continents and their respective bordering regions
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

#include    "continent.hpp"
#include    <vector>
#include    <utility>
#include    <string>

typedef std::pair<int, std::string> Region;

/*
 * =====================================================================================
 *        Class:  GameMap
 *  Description:  Handles the functionality of the game map 
 * =====================================================================================
 */

class GameMap {
    
public:
    /* ====================  LIFECYCLE     ======================================= */
    GameMap(); /* constructor */

    /* ====================  ACCESSORS     ======================================= */
	/* Returns the list of all Continents on the map */
    std::vector<Continent> getContinentList();

    /* Returns the number of regions on the map */
    int getNumberOfRegions();

    /* Returns a list of all regions bordering a given region */
    std::vector<int> getNeighborsOfRegion(int region);
	
	String getName(int region);
	
	void display();

private:
    /* ====================  METHODS       ======================================= */
    virtual void constructRegions() = 0; /* builds each region - set numberOfRegions, and assign names for displaying */
	virtual void constructBorders() = 0; /* builds each border relationship with each region */
	virtual void constructContinents() = 0; /* builds each continent */
    
	/* For use in children, in constructBorders() */
	void addRegion(Region r);
	void makeBorder(int i, int j);
	void addContinent(Continent c);

    /* ====================  DATA MEMBERS  ======================================= */
    std::vector<Continent> continentList; /* list of continents */
	int numberOfRegions;
	std::vector<Region> regionList;	//used only for display


}; /* -----  end of class GameMap  ----- */

#endif   /* ----- #ifndef gamemap_INC  ----- */
