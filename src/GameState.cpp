/*
 * =====================================================================================
 *       Filename:  GameState.cpp
 *    Description: Represents the state of a game
 * =====================================================================================
 */

#include "GameState.hpp"

#ifdef  gamestate_INC

static GameState createInitialStateFromMap(GameMap map) {

}

/* Returns the information about a region, in the form of a pair of integers
 * The first integer of the pair is the playerNumber to whom the troops in the region belong (the region's allegance)
 * The second integer in the pair is the number of troops in the region */
std::pair<int,int> getRegionInfo(int region) {
	return internalState[region];
}

/* Sets the information about a region, in the form of a pair of integers
 * The first integer of the pair is the playerNumber to whom the troops in the region belong (the region's allegance)
 * The second integer in the pair is the number of troops in the region */
void setRegionInfo(int region, std::pair<int,int> regionInfo) {
	internalState[region] = regionInfo;
	}

/* Returns the number of regions occupied by a player */
int getNumberOccupiedBy(int player) {
	int count = 0;
	for (int i=0; i<internalState.size(); i++) {
		if (internalState[i].first == player)
			count++;
	}
	return count;
}

void display() {

}

#endif
