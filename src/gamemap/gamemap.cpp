/*
 * =====================================================================================
 *
 *       Filename:  gamemap.cpp
 *
 *    Description:  handles the game map
 *
 *        Version:  1.0
 *        Created:  03/29/2014 11:08:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jacob Meline 
 * =====================================================================================
 */

#include    <vector>
#include    <utility>
#include    <string>
#include    "gamemap.hpp"
#include    "continent.hpp"

/*
 *--------------------------------------------------------------------------------------
 *       Class:  GameMap
 *      Method:  GameMap :: GameMap
 * Description:  Constructor
 *--------------------------------------------------------------------------------------
 */
GameMap::GameMap() {

    // initialize borderMatrix
    for (int i = 0; i < 41; ++i) {
        std::vector<bool> row = false;
        borderMatrix.push_back(row);
    }

    this->constructContinents();
    this->constructRegions();
    this->constructRegions();
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  GameMap
 *      Method:  GameMap :: constructContinents
 * Description:  Builds each Continent
 *--------------------------------------------------------------------------------------
 */
GameMap::constructContinents() {
    // string countryName, int troopBonus
    this->continentList.push_back(new Continent("Australia", 2));
    this->continentList.push_back(new Continent("North America", 5));
    this->continentList.push_back(new Continent("South America", 2));
    this->continentList.push_back(new Continent("Europe", 5));
    this->continentList.push_back(new Continent("Asia", 7));
    this->continentList.push_back(new Continent("Africa", 3));
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  GameMap
 *      Method:  GameMap :: constructRegions
 * Description:  Assigns Regions to each Continent
 *--------------------------------------------------------------------------------------
 */

GameMap::constructRegions() {
    std::vector<Region> regionList;

    // Australia
    Continent australia = this->continentList[0];
    regionList.push_back(std::make_pair(EasternAustralia, RegionStrings[EasternAustralia]));
    regionList.push_back(std::make_pair(Indonesia, RegionStrings[Indonesia]));
    regionList.push_back(std::make_pair(NewGuinea, RegionStrings[NewGuinea]));
    regionList.push_back(std::make_pair(WesternAustralia, RegionStrings[WesternAustralia]));
    australia.setRegions(regionList);
    regionList.clear();

    // North America
    Continent northAmerica = this->continentList[1];
    regionList.push_back(std::make_pair(Alaska, RegionStrings[Alaska]));
    regionList.push_back(std::make_pair(Alberta, RegionStrings[Alberta]));
    regionList.push_back(std::make_pair(CentralAmerica, RegionStrings[CentralAmerica]));
    regionList.push_back(std::make_pair(EasternUnitedStates, RegionStrings[EasternUnitedStates]));
    regionList.push_back(std::make_pair(Greenland, RegionStrings[Greenland]));
    regionList.push_back(std::make_pair(NorthwestTerritory, RegionStrings[NorthwestTerritory]));
    regionList.push_back(std::make_pair(Ontario, RegionStrings[Ontario]));
    regionList.push_back(std::make_pair(Quebec, RegionStrings[Quebec]));
    regionList.push_back(std::make_pair(WesternUnitedStates, RegionStrings[WesternUnitedStates]));
    northAmerica.setRegions(regionList);
    regionList.clear();

    // South America
    Continent southAmerica = this->continentList[2];
    regionList.push_back(std::make_pair(Argentina, RegionStrings[Argentina]));
    regionList.push_back(std::make_pair(Brazil, RegionStrings[Brazil]));
    regionList.push_back(std::make_pair(Peru, RegionStrings[Peru]));
    regionList.push_back(std::make_pair(Venezuela, RegionStrings[Venezuela]));
    southAmerica.setRegions(regionList);
    regionList.clear();

    // Europe
    Continent europe = this->continentList[3];
    regionList.push_back(std::make_pair(GreatBritain, RegionStrings[GreatBritain]));
    regionList.push_back(std::make_pair(Iceland, RegionStrings[Iceland]));
    regionList.push_back(std::make_pair(NorthernEurope, RegionStrings[NorthernEurope]));
    regionList.push_back(std::make_pair(Scandinavia, RegionStrings[Scandinavia]));
    regionList.push_back(std::make_pair(SouthernEurope, RegionStrings[SouthernEurope]));
    regionList.push_back(std::make_pair(Ukraine, RegionStrings[Ukraine]));
    regionList.push_back(std::make_pair(WesternEurope, RegionStrings[WesternEurope]));
    europe.setRegions(regionList);
    regionList.clear();

    // Asia
    Continent asia = this->continentList[4];
    regionList.push_back(std::make_pair(Afghanistan, RegionStrings[Afghanistan]));
    regionList.push_back(std::make_pair(China, RegionStrings[China]));
    regionList.push_back(std::make_pair(India, RegionStrings[India]));
    regionList.push_back(std::make_pair(Irkutsk, RegionStrings[Irkutsk]));
    regionList.push_back(std::make_pair(Japan, RegionStrings[Japan]));
    regionList.push_back(std::make_pair(Kamchatka, RegionStrings[Kamchatka]));
    regionList.push_back(std::make_pair(MiddleEast, RegionStrings[MiddleEast]));
    regionList.push_back(std::make_pair(Mongolia, RegionStrings[Mongolia]));
    regionList.push_back(std::make_pair(Siam, RegionStrings[Siam]));
    regionList.push_back(std::make_pair(Siberia, RegionStrings[Siberia]));
    regionList.push_back(std::make_pair(Ural, RegionStrings[Ural]));
    regionList.push_back(std::make_pair(Yakutsk, RegionStrings[Yakutsk]));
    asia.setRegions(regionList);
    regionList.clear();

    // Africa
    Continent africa = this->continentList[5];
    regionList.push_back(std::make_pair(Congo, RegionStrings[Congo]));
    regionList.push_back(std::make_pair(EastAfrica, RegionStrings[EastAfrica]));
    regionList.push_back(std::make_pair(Egypt, RegionStrings[Egypt]));
    regionList.push_back(std::make_pair(Madagascar, RegionStrings[Madagascar]));
    regionList.push_back(std::make_pair(NorthAfrica, RegionStrings[NorthAfrica]));
    regionList.push_back(std::make_pair(SouthAfrica, RegionStrings[SouthAfrica]));
    africa.setRegions(regionList);
    regionList.clear();

}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  GameMap
 *      Method:  GameMap :: constructBorders
 * Description:  Assigns which region borders which
 *--------------------------------------------------------------------------------------
 */

void GameMap::makeBorder(int i, int j) {
    this->borderMatrix[i][j] = true;
    this->borderMatrix[j][i] = true;
}

GameMap::constructBorders() {

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