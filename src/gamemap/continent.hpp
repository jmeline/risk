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
 *         Author:  Jacob Meline  
 *
 * =====================================================================================
 */

#ifndef  country_INC
#define  country_INC

#include <string>
#include <vector>
#include <utility> // std::pair, std::make_pair

class Continent
{
    public:
        Continent(std::string, int);

        // getters
        int getValue(){ 
            return this->value; 
        }

        std::string getContinentName() {
            return this->continentName; 
        }
        std::vector<std::pair<int, std::string> > getRegions() {
            return this->regions;
        }

        // setters
        void setValue(int value) { 
            this->value = value; 
        }
        void setContinentName( std::string name) { 
            this->continentName = name; 
        }
        void setRegions(std::vector<std::pair<int, std::string> > region) { 
            this->regions = region;
        }



    private:
        // Country value
        int value;
        // Country's name
        std::string continentName;
        // list of regions within the continent
        std::vector<std::pair<int, std::string> > regions;
};

#endif   /* ----- #ifndef country_INC  ----- */
