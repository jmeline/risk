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

/*
 * =====================================================================================
 *        Class:  GameMap
 *  Description:  Handles the functionality of the game map 
 * =====================================================================================
 */
class GameMap
{
    public:
        /* ====================  LIFECYCLE     ======================================= */
        GameMap ();                             /* constructor */

        /* ====================  ACCESSORS     ======================================= */

    private:
        /* ====================  METHODS       ======================================= */
        void constructContinents();             /* builds each continent */
        void constructRegions();                /* builds each region */
        void constructBorders();                /* builds each border relationship with each region */
        Continent getContinent(std::string);
        
        /* Returns the list of all Continents on the map */
        std::vector<Contenent> getContinentList();

        /* Returns the number of regions on the map */
        int getNumberOfRegions();
        
        /* Returns a list of all regions bordering a given region */
        std::vector<int> getNeighborsOfRegion(int region);

        /* ====================  DATA MEMBERS  ======================================= */
        std::vector<Continent> continentList;   /* list of continents */
        std::vector<std::pair<int, int>> borderList; /*  list of borders */

}; /* -----  end of class GameMap  ----- */

#endif   /* ----- #ifndef gamemap_INC  ----- */

