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

#include    "Continent.hpp"
#include    <vector>
#include    <utility>
#include    <string>

////typedef std::pair<int, std::string> Region;

/*
 * =====================================================================================
 *        Class:  GameMap
 *  Description:  Handles the functionality of the game map 
 * =====================================================================================
 */


class GameMap {
protected:
    /* ====================  DATA MEMBERS  ======================================= */
    std::vector<Continent> continentList; /* list of continents */

    // border list
    std::vector<std::vector<bool> > borderMatrix;

public:
    /* ====================  LIFECYCLE METHODS FOR PUBLIC USE  ======================================= */
	void initialize(); /* To be called in the implementations' constructors */

    /* Returns the list of all Continents on the map */
    std::vector<Continent> getContinentList();

    void setContinentList(std::vector<Continent> c);

    /* Returns the number of regions on the map */
    int getNumberOfRegions();

    /* Returns a list of all regions bordering a given region */
    std::vector<int> getNeighborsOfRegion(int region);

    /* ====================  METHODS FOR SPECIFIC INSTANCES TO OVERRIDE ======================================= */
    virtual void constructContinentsAndRegions() = 0;	/* builds each continent */
    virtual void constructBorders() = 0;				/* builds each border relationship with each region (by repeatedly calling makeBorder())*/

	/* ====================  METHODS USED AS HELPERS FOR SPECIFIC INSTANCES =================================== */
    void makeBorder(int i, int j);

}; /* -----  end of class GameMap  ----- */

#endif   /* ----- #ifndef gamemap_INC  ----- */

