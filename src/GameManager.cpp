#include "GameManager.hpp"
#include <fstream>
#ifndef DONTUSEMPI
	#include <mpi.h>
#endif

#define TASK = 1;	//identifies a type of MPI message
#define REPORT = 1;	//identified a type of MPI message

GameManager::GameManager(int numberOfSlaves)
{
	slaveTasks = std::vector<int>(numberOfSlaves);
}

std::vector<GameTask> GameManager::getRunsFor(std::vector<StrategyEnum::StrategyEnum> strategies, std::vector<MapEnum::MapEnum> maps, int numberOfPlayers, int timesToRepeatEachGame)
{
	//get relevent counts
	int strategyPossibilities = strategies.size();
	int strategyCombinations = 1;
	for (int i=0; i<numberOfPlayers; i++)
		strategyCombinations *= strategyPossibilities;
	int numberOfGames = maps.size() * strategyCombinations * timesToRepeatEachGame;
	//set up all of the gamesInfos
	std::vector<GameTask> gameTaskList = std::vector<GameTask>(numberOfGames);
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
			//Set up the current GameTask
			GameTask info(maps[map], strategies);
			for (int rep=0; rep<timesToRepeatEachGame; rep++)
				gameTaskList[(map*strategyCombinations+currentCombo)*timesToRepeatEachGame+rep] = info;
		}
	}
	return gameTaskList;
}

void GameManager::setGamesToRun(std::vector<GameTask> gameTaskList) {
	gamesToRun = gameTaskList;
}

void GameManager::readInExistingReport(std::string reportLocation)
{
	//// FEATURE NOT YET AVAILIABLE
}

void GameManager::runIt(std::string outputFileLocation)
{
	//prepare output file
	std::ofstream outputStream;
	outputStream.open(outputFileLocation.c_str(), std::ios::out);
	//launch initial games to get all slaves working
	int slavesToUse = (gamesToRun.size()>=slaveTasks.size() ? slaveTasks.size() : gamesToRun.size());
	for (int i=0; i<slavesToUse; i++)
		launchGame(i,i);
	int nextGameNumber = slavesToUse;	//since we just assigned game (since we just assigned game number (slavesToUse-1)
	//communicate, until all done
	for (int i=0; i<gamesToRun.size(); i++)
	{
		int whoDone = getAndHandleReport(&outputStream);
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
	int dataOut[GameTask::encodedSize];
	gamesToRun[gameNumber].encode(dataOut);
	#ifndef DONTUSEMPI
		MPI_Send(&dataOut, GameTask::encodedSize, MPI_INT, slaveNumber+1, TASK, MPI_COMM_WORLD);
	#else
		std::cout << "MPI is disabled.  Would have launched at slave"<<slaveNumber<<":";
		for (int i=0; i<GameTask::encodedSize; i++)
			std::cout <<" "<< dataOut[i];
		std::cout << std::endl;
	#endif
}

int GameManager::getAndHandleReport(std::ostream *outputStream)
{
	GameReport report;
	int dataIn[GameTask::encodedSize];
	#ifndef DONTUSEMPI
		MPI_Status status;
		MPI_Recv(&dataIn, GameTask::encodedSize, MPI_INT, MPI_ANY_SOURCE, REPORT, MPI_COMM_WORLD, &status);
        report.read(inData);
		int slaveNumber = status.MPI_SOURCE - 1;
	#else
		std::cout << "MPI is disabled.  Would have recieved a report." << std::endl;
		int slaveNumber = 1;
	#endif
	slaveTasks[slaveNumber] = -1;	//mark this slave as not working
	report.write(outputStream);
	return slaveNumber;
}
