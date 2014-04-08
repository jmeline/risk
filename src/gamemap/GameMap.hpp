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

    /* Returns the list of all Continents on the map */
    std::vector<Continent> getContinentList();

    void setContinentList(std::vector<Continent> c) {
        this->continentList = c;
    }
    /* Returns the number of regions on the map */
    int getNumberOfRegions();

    /* Returns a list of all regions bordering a given region */
    std::vector<int> getNeighborsOfRegion(int region);

    /* ====================  METHODS       ======================================= */
    virtual void constructContinents(); /* builds each continent */
    virtual void constructRegions(); /* builds each region */
    virtual void constructBorders(); /* builds each border relationship with each region */

    void makeBorder(int i, int j);
    
protected:
    /* ====================  DATA MEMBERS  ======================================= */
    std::vector<Continent> continentList; /* list of continents */

    // border list
    std::vector<std::vector<bool> > borderMatrix;

}; /* -----  end of class GameMap  ----- */

#endif   /* ----- #ifndef gamemap_INC  ----- */

