///// incorporate #if statements for MPI vs non-mpi.  Default should be MPI


#include "GameManager.hpp"
#include <fstream>
#ifndef DONTUSEMPI
	#include <mpi.h>
#endif

#define REQUEST = 1;
#define REPLY = 1;

GameManager::GameManager(int numberOfSlaves)
{
	slaveTasks = std::vector<int>(numberOfSlaves);
}

std::vector<GameInfo> GameManager::getRunsFor(std::vector<StrategyEnum::StrategyEnum> strategies, std::vector<MapEnum::MapEnum> maps, int numberOfPlayers, int timesToRepeatEachGame)
{
	//get relevent counts
	int strategyPossibilities = strategies.size();
	int strategyCombinations = 1;
	for (int i=0; i<numberOfPlayers; i++)
		strategyCombinations *= strategyPossibilities;
	int numberOfGames = maps.size() * strategyCombinations * timesToRepeatEachGame;
	//set up all of the gamesInfos
	std::vector<GameInfo> gameInfoList = std::vector<GameInfo>(numberOfGames);
	for (int map=0; map<maps.size(); map++) {
		//iterate through all strategy combinations
		//"currentCombo" tracks the particular strategy combination.  Think of it as a number with "numberOfPlayers" many "digits", each of which can take on one of "strategyPossibilities" possible values
		for (int currentCombo=0; currentCombo<strategyCombinations; currentCombo++) {
			//get the set of strategies for this combination
			StrategyEnum::StrategyEnum strategies[] = { StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER };
			for (int playerNum=0; playerNum<numberOfPlayers; playerNum++) {
				//pick out the "digit" of currentCombo that corresponds to this player
				int strategyIndex = currentCombo;
				for (int j=0; j<playerNum; j++)
					strategyIndex /= strategyPossibilities;	//"shift right" one digit
				strategyIndex %= strategyPossibilities;		//keep the lowest surviving digit
				strategies[playerNum] = strategies[strategyIndex];
			}
			//Set up the current GameInfo
			GameInfo info =
				std::tuple<MapEnum::MapEnum,StrategyEnum::StrategyEnum,StrategyEnum::StrategyEnum,StrategyEnum::StrategyEnum,StrategyEnum::StrategyEnum,StrategyEnum::StrategyEnum,StrategyEnum::StrategyEnum>(
					maps[map], strategies[0],strategies[1],strategies[2],strategies[3],strategies[4],strategies[5]
				);
			for (int rep=0; rep<timesToRepeatEachGame; rep++)
				gameInfoList[(map*strategyCombinations+currentCombo)*timesToRepeatEachGame+rep] = info;
		}
	}
	return gameInfoList;
}

void GameManager::setGamesToRun(std::vector<GameInfo> gameInfoList) {
	gamesToRun = gameInfoList;
}

void GameManager::readInExistingReport(std::string reportLocation)
{
	//// FEATURE NOT YET AVAILIABLE
}

void GameManager::runIt(std::string outputFileLocation)
{
	//prepare output file
	std::ofstream outputStream;
	outputStream.open(outputFileLocation.c_str(), std::ios::out | std::ios::binary);
	//launch initial games to get all slaves working
	int slavesToUse = (gamesToRun.size()>=slaveTasks.size() ? slaveTasks.size() : gamesToRun.size());
	for (int i=0; i<slavesToUse; i++)
		launchGame(i,i);
	int nextGameNumber = slavesToUse;	//since we just assigned game (since we just assigned game number (slavesToUse-1)
	//communicate, until all done
	for (int i=0; i<gamesToRun.size(); i++)
	{
		int whoDone = getAndHandleReport(outputStream);
		if (nextGameNumber<gamesToRun.size())
		{
			launchGame(whoDone, nextGameNumber);
			nextGameNumber++;
		}
	}
	//close output file
	outputStream.close();
}

void GameManager::launchGame(int slaveNumber, int gameNumber)
{
	slaveTasks[slaveNumber] = gameNumber;
	GameInfo info = gamesToRun[gameNumber];
	#ifndef DONTUSEMPI
		MPI_Send(data, numOfDatas, MPI_LONG, slaveNumber+1, REQUEST, MPI_COMM_WORLD);
	#else
		std::cout << "MPI is disabled.  Would have launched at slave"<<slaveNumber<<": ";
		for (int i=0; i<7; i++)
			std::cout << std::get<0>(info);
		std::cout << std::endl;
	#endif
}

int GameManager::getAndHandleReport(std::ofstream outputStream)
{
	#ifndef DONTUSEMPI
		MPI_Status status
		MPI_Recv(&data, sizeof(GameReport)/sizeof(int), MPI_INT, MPI_ANY_SOURCE, REPLY, MPI_COMM_WORLD, &status);
        int slaveNumber = status.MPI_SOURCE;
		GameReport report;
		////fill report with the data, once you've put something in "data"
	#else
		std::cout << "MPI is disabled.  Would have recieved a report." << std::endl;
		int slaveNumber = 1;
		GameReport report;
	#endif
	slaveTasks[slaveNumber] = -1;
	report.write(outputStream);
	return slaveNumber;
}
