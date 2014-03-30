/*
 * =====================================================================================
 *       Filename:  Strategy.hpp
 *    Description: Defines the interface for a strategy
 * =====================================================================================
 */

#ifndef  strategy_INC
#define  strategy_INC

#include "gamestate\GameState.hpp"
#include "gamemap\gamemap.hpp"
#include <vector>

class Strategy {
pulic:
	void init(GameMap map);
	int claim(GameState state);
	std::vector<pair<int,int>> place(GameState state, int numTroops);
	std::pair<int,int> attack(GameState state);
	int defend(GameState state, int countryAttacked, int countryAttacking);
};

#endif   /* ----- #ifndef strategy_INC  ----- */
