/*
 * =====================================================================================
 *       Filename:  GameReport.hpp
 *    Description: Reports the results of a single game
 * =====================================================================================
 */

#ifndef  gamereport_INC
#define  gamereport_INC

#include "Strategy.hpp"
#include <vector>
#include <iostream>

class GameReport
{
public:
	// Order is preserved in vectors.
	int rounds;								//how long the game lasted
	MapEnum::MapEnum map;					//the map where the game occurred
	StrategyEnum::StrategyEnum players[6];	//players[0] got the first move, players[1] got the second, etc.
	StrategyEnum::StrategyEnum winners[6];	//winners[0] is the winner, winners[1] came in second, etc.

	GameReport();

	void write(std::ofstream outputStream);
	
	void read(std::ifstream inputStream);
};

#endif   /* ----- #ifndef gamereport_INC  ----- */
