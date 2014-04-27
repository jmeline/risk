/*
 * =====================================================================================
 *       Filename:  GameTask.hpp
 *    Description:  Contains parameters for running a game
 * =====================================================================================
 */

#ifndef  gametask_INC
#define  gametask_INC

#include "Strategy.hpp"
#include <vector>

class GameTask
{
public:
	const static int encodedSize = 7;
	MapEnum::MapEnum map;					//the map where the game occurred
	StrategyEnum::StrategyEnum players[6];	//players[0] got the first move, players[1] got the second, etc.

	GameTask();
	GameTask(MapEnum::MapEnum theMap, StrategyEnum::StrategyEnum thePlayers[]);

	void encode(int outData[]);
	
	void decode(int inData[]);
        
        void print(int data[]);
};

#endif   /* ----- #ifndef gametask_INC ----- */
