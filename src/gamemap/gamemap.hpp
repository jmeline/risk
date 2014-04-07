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

/*
 * =====================================================================================
 *        Class:  GameMap
 *  Description:  Handles the functionality of the game map 
 * =====================================================================================
 */

typedef std::pair<int, std::string> Region;

class GameMap {
    
public:
    /* ====================  LIFECYCLE     ======================================= */
    GameMap(); /* constructor */

    /* ====================  ACCESSORS     ======================================= */

private:
    /* ====================  METHODS       ======================================= */
    virtual void constructContinents() = 0; /* builds each continent */
    virtual void constructRegions() = 0; /* builds each region */
    virtual void constructBorders() = 0; /* builds each border relationship with each region */
    Continent getContinent(std::string);

    /* Returns the list of all Continents on the map */
    std::vector<Continent> getContinentList();

    /* Returns the number of regions on the map */
    int getNumberOfRegions();

    /* Returns a list of all regions bordering a given region */
    std::vector<int> getNeighborsOfRegion(int region);
    
    void makeBorder(int i, int j);

    /* ====================  DATA MEMBERS  ======================================= */
    std::vector<Continent> continentList; /* list of continents */

        // border list
    bool borderMatrix[41][41];

}; /* -----  end of class GameMap  ----- */

#endif   /* ----- #ifndef gamemap_INC  ----- */

