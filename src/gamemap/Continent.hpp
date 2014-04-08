/*
 * =====================================================================================
 *
 *       Filename:  continent.hpp
 *
 *    Description: Represents a continent
 *                 Each continent contains regions
 *
 *        Version:  1.0
 *        Created:  03/28/2014 08:59:46 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Jacob Meline, Joseph Richardson  
 *
 * =====================================================================================
 */

#ifndef  country_INC
#define  country_INC

#include    <string>
#include    <vector>
#include    <utility> // std::pair, std::make_pair

/*
 * =====================================================================================
 *        Class:  Continent
 *  Description:  Handles the Continent
 * =====================================================================================
 */

typedef std::pair<int, std::string> Region;

class Continent {
public:
    Continent(std::string, int); /* constructor */

    // getters

    int getValue() {
        return this->value;
    }

    std::string getContinentName() {
        return this->continentName;
    }

    std::vector<Region> getRegionList() {
        return this->regionList;
    }

    int getTroopValue() {
        return this->value;
    }

    // setters

    void setValue(int value) {
        this->value = value;
    }

    void setContinentName(std::string name) {
        this->continentName = name;
    }

    void setRegions(std::vector<Region> r) {
        this->regionList = r;
    }


private:
    /* ====================  DATA MEMBERS  ======================================= */

    // Country's troop value
    int value;
    // Country's name
    std::string continentName;
    // region list
    std::vector<Region> regionList;

}; /* -----  end of class Continent  ----- */

#endif   /* ----- #ifndef country_INC  ----- */
