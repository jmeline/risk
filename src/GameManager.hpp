/*
 * =====================================================================================
 *       Filename:  GameManager.hpp
 *    Description: Assigns games to run
 * =====================================================================================
 */

#ifndef  gamemanager_INC
#define  gamemanager_INC

#include "game/Game.hpp"

class GameManager {
private:
	std::vector<GameTask> gamesToRun;	//the index of each GameInfo in this list is its identifier
	std::vector<int> slaveTasks;		//used to keep track of which task each slave is working on.  The integer is the index in the gamesToRun list.  -1 denotes "not assigned", which should not often be used

public:
	GameManager(int numberOfSlaves);
	static std::vector<GameTask> getAllPermutationsFor(std::vector<StrategyEnum::StrategyEnum> strategies, std::vector<MapEnum::MapEnum> maps, int numberOfPlayers, int timesToRepeatEachGame);
	static std::vector<GameTask> getNonrepeatingPermutationsFor(std::vector<StrategyEnum::StrategyEnum> strategies, std::vector<MapEnum::MapEnum> maps, int numberOfPlayers, int timesToRepeatEachGame);
	void setGamesToRun(std::vector<GameTask> gameInfoList);
	void readInExistingReport(std::string reportLocation);	//allows us to resume unfinished work by reading an old report file
	void runIt(std::string outputFileLocation);
private:
	#ifndef DONTUSEMPI
	void launchGame(int slaveNumber, int gameNumber);	//communicates with a slave: assigns a slave to run gamesToRun[gameNumber]
	int getAndHandleReport(std::ostream *outputStream);	//communicates with a slave: gets the results back, then adds them to the output file.  Returns the number of the slave that gave the report
	void informThatIsDone(int slaveNumber);				//communicates with a slave: tells it thtat it is all done.  Does so by sending a GameTask with StrategyEnum::NOPLAYER strategies.
	#endif
};

#endif   /* ----- #ifndef gamemanager_INC  ----- */
