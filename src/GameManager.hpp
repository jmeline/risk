/*
 * =====================================================================================
 *       Filename:  GameManager.hpp
 *    Description: Assigns games to run
 * =====================================================================================
 */

#ifndef  gamemanager_INC
#define  gamemanager_INC

#include "gamemap/GameMap.hpp"
#include "GameReport.hpp"
#include "Game.hpp"
#include "Strategy.hpp"

typedef std::tuple<MapEnum::MapEnum,StrategyEnum::StrategyEnum,StrategyEnum::StrategyEnum,StrategyEnum::StrategyEnum,StrategyEnum::StrategyEnum,StrategyEnum::StrategyEnum,StrategyEnum::StrategyEnum> GameInfo;
	//in order, these are the map followed by the strategies for p1-p6, in order.

class GameManager {
private:
	std::vector<StrategyEnum::StrategyEnum> strategies;	//lists the strategies that should be evaluated
	std::vector<MapEnum::MapEnum> maps;			//lists the maps that should be used to evaluate strategies
	std::vector<GameInfo> gamesToRun;	//the index of each GameInfo in this list is its identifier
	std::vector<short> gameFinished;	//used to keep track of which tasks have finished. 0 denotes "no", 1 "yes".  Not using bool because I hear vectors of bools are unsafe
	std::vector<int> slaveTasks;		//used to keep track of which task each slave is working on.  The integer is the index in the gamesToRun list.  -1 denotes "not assigned", which should not often be used

public:
	GameManager(std::string outputFileLocation);
	void addMap(MapEnum::MapEnum e);
	void addStrategy(StrategyEnum::StrategyEnum e);
	void runIt();
	void stopEverything();	//used to stop, if time gets close to running out (we may not actually implement this)
	GameReport runGame();
private:
    void readInExistingReport(std::string reportLocation);	//allows us to resume unfinished work by reading an old report file
	void launchGame(int gameNumber);	//assigns a slave to run gamesToRun[gameNumber]
	void handleReport(int whoGaveReport, GameReport report);	//adds a report to the output file
};

#endif   /* ----- #ifndef gamemanager_INC  ----- */
