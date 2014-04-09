/* 
 * File:   Earth.cpp
 * Author: Jacob&BJ
 *
 * Created on April 7, 2014, 9:53 PM
 */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Earth
 *      Method:  GameMap :: constructContinents
 * Description:  Builds each Continent
 *--------------------------------------------------------------------------------------
 */

#include    "Earth.hpp"
#include    "../gamemap/GameMap.hpp"
#include    "../gamemap/Continent.hpp"
#include    <string>
#include    <utility>
#include    <vector>

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

Earth::Earth()
{
	initialize();
}

void Earth::constructContinentsAndRegions()
{
    // Australia
    Continent australia("Australia",2);
	australia.addRegion(std::make_pair(EasternAustralia, RegionStrings[EasternAustralia]));
    australia.addRegion(std::make_pair(Indonesia, RegionStrings[Indonesia]));
    australia.addRegion(std::make_pair(NewGuinea, RegionStrings[NewGuinea]));
    australia.addRegion(std::make_pair(WesternAustralia, RegionStrings[WesternAustralia]));
	continentList.push_back(australia);

    // North America
    Continent northAmerica("North America", 5);
	northAmerica.addRegion(std::make_pair(Alaska, RegionStrings[Alaska]));
    northAmerica.addRegion(std::make_pair(Alberta, RegionStrings[Alberta]));
    northAmerica.addRegion(std::make_pair(CentralAmerica, RegionStrings[CentralAmerica]));
    northAmerica.addRegion(std::make_pair(EasternUnitedStates, RegionStrings[EasternUnitedStates]));
    northAmerica.addRegion(std::make_pair(Greenland, RegionStrings[Greenland]));
    northAmerica.addRegion(std::make_pair(NorthwestTerritory, RegionStrings[NorthwestTerritory]));
    northAmerica.addRegion(std::make_pair(Ontario, RegionStrings[Ontario]));
    northAmerica.addRegion(std::make_pair(Quebec, RegionStrings[Quebec]));
    northAmerica.addRegion(std::make_pair(WesternUnitedStates, RegionStrings[WesternUnitedStates]));
    continentList.push_back(northAmerica);

    // South America
    Continent southAmerica("South America", 2);
    southAmerica.addRegion(std::make_pair(Argentina, RegionStrings[Argentina]));
    southAmerica.addRegion(std::make_pair(Brazil, RegionStrings[Brazil]));
    southAmerica.addRegion(std::make_pair(Peru, RegionStrings[Peru]));
    southAmerica.addRegion(std::make_pair(Venezuela, RegionStrings[Venezuela]));
    continentList.push_back(southAmerica);

    // Europe
    Continent europe("Europe", 5);
    europe.addRegion(std::make_pair(GreatBritain, RegionStrings[GreatBritain]));
    europe.addRegion(std::make_pair(Iceland, RegionStrings[Iceland]));
    europe.addRegion(std::make_pair(NorthernEurope, RegionStrings[NorthernEurope]));
    europe.addRegion(std::make_pair(Scandinavia, RegionStrings[Scandinavia]));
    europe.addRegion(std::make_pair(SouthernEurope, RegionStrings[SouthernEurope]));
    europe.addRegion(std::make_pair(Ukraine, RegionStrings[Ukraine]));
    europe.addRegion(std::make_pair(WesternEurope, RegionStrings[WesternEurope]));
    continentList.push_back(europe);

    // Asia
    Continent asia("Asia", 7);
    asia.addRegion(std::make_pair(Afghanistan, RegionStrings[Afghanistan]));
    asia.addRegion(std::make_pair(China, RegionStrings[China]));
    asia.addRegion(std::make_pair(India, RegionStrings[India]));
    asia.addRegion(std::make_pair(Irkutsk, RegionStrings[Irkutsk]));
    asia.addRegion(std::make_pair(Japan, RegionStrings[Japan]));
    asia.addRegion(std::make_pair(Kamchatka, RegionStrings[Kamchatka]));
    asia.addRegion(std::make_pair(MiddleEast, RegionStrings[MiddleEast]));
    asia.addRegion(std::make_pair(Mongolia, RegionStrings[Mongolia]));
    asia.addRegion(std::make_pair(Siam, RegionStrings[Siam]));
    asia.addRegion(std::make_pair(Siberia, RegionStrings[Siberia]));
    asia.addRegion(std::make_pair(Ural, RegionStrings[Ural]));
    asia.addRegion(std::make_pair(Yakutsk, RegionStrings[Yakutsk]));
    continentList.push_back(asia);

    // Africa
    Continent africa("Africa", 3);
    africa.addRegion(std::make_pair(Congo, RegionStrings[Congo]));
    africa.addRegion(std::make_pair(EastAfrica, RegionStrings[EastAfrica]));
    africa.addRegion(std::make_pair(Egypt, RegionStrings[Egypt]));
    africa.addRegion(std::make_pair(Madagascar, RegionStrings[Madagascar]));
    africa.addRegion(std::make_pair(NorthAfrica, RegionStrings[NorthAfrica]));
    africa.addRegion(std::make_pair(SouthAfrica, RegionStrings[SouthAfrica]));
    continentList.push_back(africa);
}

void Earth::constructBorders()
{
    // Australia Border
    makeBorder(EasternAustralia, NewGuinea);
    makeBorder(EasternAustralia, WesternAustralia);
    makeBorder(WesternAustralia, NewGuinea);
    makeBorder(WesternAustralia, Indonesia);
    makeBorder(Indonesia, NewGuinea);

    // North America
    makeBorder(Alaska, NorthwestTerritory);
    makeBorder(Alaska, Alberta);
    makeBorder(Alaska, Kamchatka);

    makeBorder(Alberta, NorthwestTerritory);
    makeBorder(Alberta, Ontario);
    makeBorder(Alberta, WesternUnitedStates);

    makeBorder(WesternUnitedStates, Ontario);
    makeBorder(WesternUnitedStates, EasternUnitedStates);
    makeBorder(WesternUnitedStates, CentralAmerica);

    makeBorder(EasternUnitedStates, Ontario);
    makeBorder(EasternUnitedStates, Quebec);
    makeBorder(EasternUnitedStates, CentralAmerica);

    makeBorder(CentralAmerica, Venezuela);

    makeBorder(Greenland, Quebec);
    makeBorder(Greenland, Ontario);
    makeBorder(Greenland, NorthwestTerritory);
    makeBorder(Greenland, Iceland);

    // South America
    makeBorder(Argentina, Brazil);
    makeBorder(Argentina, Peru);

    makeBorder(Brazil, Peru);
    makeBorder(Brazil, Venezuela);
    makeBorder(Brazil, NorthAfrica);

    makeBorder(Peru, Venezuela);

    // Europe
    makeBorder(Iceland, GreatBritain);
    makeBorder(Iceland, Scandinavia);

    makeBorder(GreatBritain, Scandinavia);
    makeBorder(GreatBritain, NorthernEurope);
    makeBorder(GreatBritain, WesternEurope);

    makeBorder(WesternEurope, NorthAfrica);
    makeBorder(WesternEurope, NorthernEurope);
    makeBorder(WesternEurope, SouthernEurope);

    makeBorder(NorthernEurope, SouthernEurope);
    makeBorder(NorthernEurope, Scandinavia);
    makeBorder(NorthernEurope, Ukraine);

    makeBorder(Scandinavia, Ukraine);

    makeBorder(Ukraine, MiddleEast);
    makeBorder(Ukraine, Afghanistan);
    makeBorder(Ukraine, Ural);

    makeBorder(SouthernEurope, Egypt);
    makeBorder(SouthernEurope, NorthAfrica);
    makeBorder(SouthernEurope, MiddleEast);

    // Asia
    makeBorder(Ural, Siberia);
    makeBorder(Ural, China);
    makeBorder(Ural, Afghanistan);

    makeBorder(Afghanistan, MiddleEast);
    makeBorder(Afghanistan, India);
    makeBorder(Afghanistan, China);

    makeBorder(MiddleEast, EastAfrica);
    makeBorder(MiddleEast, India);

    makeBorder(India, China);
    makeBorder(India, Siam);

    makeBorder(China, Siam);
    makeBorder(China, Mongolia);
    makeBorder(China, Siberia);

    makeBorder(Siam, Indonesia);

    makeBorder(Mongolia, Japan);
    makeBorder(Mongolia, Irkutsk);
    makeBorder(Mongolia, Siberia);
    makeBorder(Mongolia, Kamchatka);

    makeBorder(Kamchatka, Japan);
    makeBorder(Kamchatka, Irkutsk);
    makeBorder(Kamchatka, Yakutsk);

    makeBorder(Irkutsk, Siberia);
    makeBorder(Irkutsk, Yakutsk);

    // Africa
    makeBorder(Congo, EastAfrica);
    makeBorder(Congo, NorthAfrica);
    makeBorder(Congo, SouthAfrica);

    makeBorder(SouthAfrica, Madagascar);
    makeBorder(SouthAfrica, EastAfrica);

    makeBorder(EastAfrica, Madagascar);
    makeBorder(EastAfrica, NorthAfrica);
    makeBorder(EastAfrica, Egypt);

    makeBorder(Egypt, MiddleEast);
    makeBorder(Egypt, NorthAfrica);

}
