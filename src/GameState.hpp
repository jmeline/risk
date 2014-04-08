/*
 * =====================================================================================
 *       Filename:  GameState.hpp
 *    Description: Represents the state of a game
 * =====================================================================================
 */

#ifndef  gamestate_INC
#define  gamestate_INC

#include <vector>

#include "gamemap/GameMap.hpp"

class GameState
{
public:
    GameState();
    /* Creates a state where all regions in the map are represented by <allegence,troops> pairs of value <-1,0>*/
    static GameState createInitialStateFromMap(GameMap map);

    /* Returns the information about a region, in the form of a pair of integers
     * The first integer of the pair is the playerNumber to whom the troops in the region belong (the region's allegance)
     * The second integer in the pair is the number of troops in the region */
    std::pair<int, int> getRegionInfo(int region);

    /* Sets the information about a region, in the form of a pair of integers
     * The first integer of the pair is the playerNumber to whom the troops in the region belong (the region's allegance)
     * The second integer in the pair is the number of troops in the region */
    void setRegionInfo(int region, std::pair<int, int> regionInfo);

    /* Returns the number of regions occupied by a player */
    int getNumberOccupiedBy(int player);
};

#endif   /* ----- #ifndef gamestate_INC  ----- */
