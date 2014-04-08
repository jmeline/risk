/* 
 * File:   Earth.hpp
 * Author: Jacob
 */

#ifndef EARTH_HPP
#define	EARTH_HPP
/*
 * =====================================================================================
 *        Class:  GameMap
 *  Description:  Handles the functionality of the game map 
 * =====================================================================================
 */

//typedef std::pair<int, std::string> Region;

#include    <vector>
#include    <string>
#include    <utility>

enum Regions {
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

const char* RegionStrings[] = {
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

class Earth : public GameMap {
    
public:
    Earth();

private:
    virtual void constructContinents();

    virtual void constructRegions();

    virtual void constructBorders();



};

#endif	/* EARTH_HPP */

