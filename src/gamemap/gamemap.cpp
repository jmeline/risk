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

#ifdef gamemap_INC

/*
 *--------------------------------------------------------------------------------------
 *       Class:  GameMap
 *      Method:  GameMap :: GameMap
 * Description:  Constructor
 *--------------------------------------------------------------------------------------
 */
GameMap::GameMap()
{
	constructRegions();		//uses addRegion(String)
	// initialize borderMatrix
    for (int i = 0; i < 41; ++i)
    {
		for (int j = 0; j < 41; ++j) {
            borderMatrix[i][j] = false;
        }
	}
	constructBorders();		//uses addBorder(int,int)
	constructContinents();	//uses addContinent(Continent)
}



void GameMap::makeBorder(int i, int j) {
    this->borderMatrix[i][j] = true;
    this->borderMatrix[j][i] = true;
}

#endif
