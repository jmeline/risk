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
	const static int encodedSize = 14;
	int rounds;								//how long the game lasted
	MapEnum::MapEnum map;					//the map where the game occurred
	StrategyEnum::StrategyEnum players[6];	//players[0] got the first move, players[1] got the second, etc.
	int winners[6];							//winners[0] is the winning player, winners[1] came in second, etc.

	GameReport();
	GameReport(int theRounds, MapEnum::MapEnum theMap, StrategyEnum::StrategyEnum thePlayers[], int theWinners[]);

	void encode(int outData[]);
	
	void decode(int inData[]);

	void write(std::ostream *outputStream);
};

#endif   /* ----- #ifndef gamereport_INC  ----- */
