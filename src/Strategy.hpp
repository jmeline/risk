/*
 * =====================================================================================
 *       Filename:  Strategy.hpp
 *    Description: Defines the interface for a strategy
 * =====================================================================================
 */

#ifndef  strategy_INC
#define  strategy_INC

#include "GameState.hpp"
#include "gamemap/GameMap.hpp"
#include <vector>
#include <utility>
#include <tuple>

class Strategy {
public:
	/* Called once to let the strategy understand the relationship between countries and Contenents */
	void init(GameMap map, int myPlayerNumber);

	/* Returns the region to claim */
	int claim(GameState state);
	
	/* Returns a list of pairs representing country placement:
	 * The first int in the each pair is the region where to place
	 * The second int in each pair is the number of troops to place there */
	std::vector<std::pair<int,int> > place(GameState state, int numTroops);

	/* Returns a pair representing an attack:
	 * The first int in the pair is the country being attacked
	 * The second int in the pair is the country doing the attacking */
	std::pair<int,int> attack(GameState state);

	/* Returns true if the defender wish to defend with two armies, and false if it wishes to defend with only one */
	bool defend(GameState state, int countryAttacked, int countryAttacking);

	/* Returns a vector of triplets, each representing the movement of troops for fortification:
	 * The first int of each triplet is the country moving from
	 * The second int of each triplet is the country moving to
	 * The third int of each triplet is the number of troops to move */
	std::vector<std::tuple<int,int,int> > fortify(GameState state);
};

#endif   /* ----- #ifndef strategy_INC  ----- */
