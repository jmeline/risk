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
};

#endif   /* ----- #ifndef gamestate_INC  ----- */
