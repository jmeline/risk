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

//typedef std::pair<int, std::string> Region;
enum Regions
{
    // North America
    Alaska,
    Alberta,
    CentralAmerica,
    EasternUnitedStates,
    Greenland,
    NorthwestTerritory,
    Ontario,
    Quebec,
    WesternUnitedStates,
    
    // South America
    Argentina,
    Brazil,
    Peru,
    Venezuela,
    
    // Europe
    GreatBritain,
    Iceland,
    NorthernEurope,
    Scandinavia,
    SouthernEurope,
    Ukraine,
    WesternEurope,
    
    // Africa
    Congo,
    EastAfrica,
    Egypt,
    Madagascar,
    NorthAfrica,
    SouthAfrica,
    
    // Asia
    Afghanistan,
    China,
    India,
    Irkutsk,
    Japan,
    Kamchatka,
    MiddleEast,
    Mongolia,
    Siam,
    Siberia,
    Ural,
    Yakutsk,

    // Australia
    EasternAustralia,
    Indonesia,
    NewGuinea,
    WesternAustralia
};

const char* RegionStrings[] =
{
    // North America
    "Alaska",
    "Alberta",
    "Central America",
    "Eastern UnitedStates",
    "Greenland",
    "Northwest Territory",
    "Ontario",
    "Quebec",
    "Western United States",
    
    // South America
    "Argentina",
    "Brazil",
    "Peru",
    "Venezuela",
    
    // Europe
    "Great Britain",
    "Iceland",
    "Northern Europe",
    "Scandinavia",
    "Southern Europe",
    "Ukraine",
    "Western Europe",
    
    // Africa
    "Congo",
    "East Africa",
    "Egypt",
    "Madagascar",
    "North Africa",
    "South Africa",
    
    // Asia
    "Afghanistan",
    "China",
    "India",
    "Irkutsk",
    "Japan",
    "Kamchatka",
    "Middle East",
    "Mongolia",
    "Siam",
    "Siberia",
    "Ural",
    "Yakutsk",

    // Australia
    "Eastern Australia",
    "Indonesia",
    "New Guinea",
    "Western Australia"
};

class GameMap {
    
public:
    /* ====================  LIFECYCLE     ======================================= */
    GameMap(); /* constructor */

private:
    /* ====================  METHODS       ======================================= */
    void constructContinents(); /* builds each continent */
    void constructRegions(); /* builds each region */
    void constructBorders(); /* builds each border relationship with each region */
    
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
    std::vector<std::vector<bool> > borderMatrix;

}; /* -----  end of class GameMap  ----- */

#endif   /* ----- #ifndef gamemap_INC  ----- */

