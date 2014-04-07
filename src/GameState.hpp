/*
 * =====================================================================================
 *       Filename:  GameState.hpp
 *    Description: Represents the state of a game
 * =====================================================================================
 */

#ifndef  gamestate_INC
#define  gamestate_INC

#include <vector>

class GameState
{
public:
	/* Creates a state where all regions in the map are represented by <allegence,troops> pairs of value <-1,0>*/
	static GameState createInitialStateFromMap(gamemap map);

	/* Returns the information about a region, in the form of a pair of integers
	 * The first integer of the pair is the playerNumber to whom the troops in the region belong (the region's allegance)
	 * The second integer in the pair is the number of troops in the region */
	std::pair<int,int> getRegionInfo(int region);

	/* Sets the information about a region, in the form of a pair of integers
	 * The first integer of the pair is the playerNumber to whom the troops in the region belong (the region's allegance)
	 * The second integer in the pair is the number of troops in the region */
	void setRegionInfo(int region, std::pair<int,int> regionInfo);

	/* Returns the number of regions occupied by a player */
	int getNumberOccupiedBy(int player);
	
	void display();
	
private:
	/*
	The index gives the regions
	The first int gives the allegance
	The second int gives the number of troops
	*/
	std::vector<pair<int,int>> internalState;
};

#endif   /* ----- #ifndef gamestate_INC  ----- */
