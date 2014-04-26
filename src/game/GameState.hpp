/*
 * =====================================================================================
 *       Filename:  GameState.hpp
 *    Description: Represents the state of a game
 * =====================================================================================
 */

#ifndef  gamestate_INC
#define  gamestate_INC

#include <iostream>
#include <vector>

#include "../gamemap/GameMap.hpp"

class GameState
{
private:
	std::vector<std::pair<int,int>> internalState;

public:
    /* Creates a state where all regions in the map are represented by <allegence,troops> pairs of value <-1,0>*/
	GameState(int size);
	GameState(GameMap* map);
	GameState();		//used to avoid initialization trouble, but one of the other two should always be called, in reality

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

	/* Returns a list of owned regions */
    std::vector<int> getRegionsOwnedByPlayer(int playerNumber);

	/* Returns a list of exposed owned regions
	 * The first int is the region number, the second is the number of enemy troops surrounding it. */
	std::vector<std::pair<int,int>> getAllExposedBorders(int playerNumber, GameMap* map);

	/* Returns a list of all neighbors of a given player. */
	std::vector<int> getAllNeighborsOfPlayer(int playerNumber, GameMap* map);

	/* Returns the total number of regions on the map (and hence in the state) */
	int getNumRegions();

	void display();

};

#endif   /* ----- #ifndef gamestate_INC  ----- */
