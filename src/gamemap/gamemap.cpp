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

