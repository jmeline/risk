/*
 * =====================================================================================
 *
 *       Filename:  gamemap.hpp
 *
 *    Description: Represents a country
 *                 
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

#ifndef  gamemap_INC
#define  gamemap_INC

#include <string>
#include <vector>

class Contenent
{
    public:
        Contenent(std::string name, int v){
            this->contenentName = name;
            this->value = v;
        }

		int getTroopValue() { return value; }

		std::vector<int> getRegionList();

    private:
        int value;
        std::string contenentName;
};

class GameMap {
	/* Returns the number of regions on the map */
	int getNumberOfRegions();

	/* Returns a list of all regions bordering a given region */
	std::vector<int> getNeighborsOfRegion(int region);

	/* Returns the list of all Continents on the map */
	std::vector<Contenent> getContinentList();
};

#endif   /* ----- #ifndef gamemap_INC  ----- */
