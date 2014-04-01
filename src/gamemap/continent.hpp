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
class Continent
{
    public:
        /* ====================  LIFECYCLE     ======================================= */
        Continent(std::string, int);            /* constructor */

        /* ====================  ACCESSORS     ======================================= */
        
        // getters
        int getValue(){ return this->value; }

        std::string getContinentName() {return this->continentName; }

        std::vector<std::pair<int, std::string>> getRegionList() { return this->regionList; }

        // setters
        void setValue(int value) { this->value = value; }
        
        void setContinentName( std::string name) { this->continentName = name; }

        int getTroopValue() { return value; }

    private:
        /* ====================  DATA MEMBERS  ======================================= */
        
        // Country's troop value
        int value;
        // Country's name
        std::string continentName;
        // region list
        std::vector<std::pair<int, std::string>> regionList;

}; /* -----  end of class Continent  ----- */

#endif   /* ----- #ifndef country_INC  ----- */
