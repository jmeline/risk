#include "GameManager.hpp"
#include <fstream>

#ifndef DONTUSEMPI
	#include <mpi.h>
	#define MPI_TASK 1		//identifies a type of MPI message
	#define MPI_REPORT 2		//identifies a type of MPI message
#endif

GameManager::GameManager(int numberOfSlaves)
{
	slaveTasks = std::vector<int>(numberOfSlaves);
}

std::vector<GameTask> GameManager::getAllPermutationsFor(std::vector<StrategyEnum::StrategyEnum> strategies, std::vector<MapEnum::MapEnum> maps, int numberOfPlayers, int timesToRepeatEachGame)
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
			StrategyEnum::StrategyEnum strategiesThisRun[] = { StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER };
			for (int playerNum=0; playerNum<numberOfPlayers; playerNum++) {
				//pick out the "digit" of currentCombo that corresponds to this player
				int strategyIndex = currentCombo;
				for (int j=0; j<playerNum; j++)
					strategyIndex /= strategyPossibilities;	//"shift right" one digit
				strategyIndex %= strategyPossibilities;		//keep the lowest surviving digit
				strategiesThisRun[playerNum] = strategies[strategyIndex];
			}
			//Set up the current GameTask
			GameTask info(maps[map], strategiesThisRun);
			for (int rep=0; rep<timesToRepeatEachGame; rep++)
				gameTaskList[(map*strategyCombinations+currentCombo)*timesToRepeatEachGame+rep] = info;
		}
	}
	return gameTaskList;
}

std::vector<GameTask> GameManager::getNonrepeatingPermutationsFor(std::vector<StrategyEnum::StrategyEnum> strategies, std::vector<MapEnum::MapEnum> maps, int numberOfPlayers, int timesToRepeatEachGame)
{
	int strategyPossibilities = strategies.size();
	int strategyPermutations = 1;
	for (int i=0; i<numberOfPlayers; i++)
		strategyPermutations *= strategyPossibilities;

	//set up all of the gamesInfos
	std::vector<GameTask> gameTaskList;
	for (int map=0; map<maps.size(); map++) {
		//iterate through all strategy combinations
		//"currentCombo" tracks the particular strategy combination.  Think of it as a number with "numberOfPlayers" many "digits", each of which can take on one of "strategyPossibilities" possible values
		for (int currentCombo=0; currentCombo<strategyPermutations; currentCombo++) {
			//get the set of strategies for this combination
			bool thisRunIsBad = false;
			StrategyEnum::StrategyEnum strategiesThisRun[] = { StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER };
			for (int playerNum=0; playerNum<numberOfPlayers; playerNum++) {
				//pick out the "digit" of currentCombo that corresponds to this player
				int strategyIndex = currentCombo;
				for (int j=0; j<playerNum; j++)
					strategyIndex /= strategyPossibilities;	//"shift right" one digit
				strategyIndex %= strategyPossibilities;		//keep the lowest surviving digit
				//now, see if it was valid (so far)
				for (int j=0; j<playerNum; j++) {
					if (strategiesThisRun[j] == strategies[strategyIndex]) {
						thisRunIsBad = true;
						break;
					}
				}
				if (thisRunIsBad)
					break;
				//if we made it this far, there are no problems (yet)
				strategiesThisRun[playerNum] = strategies[strategyIndex];
			}
			if (!thisRunIsBad) {
				//Set up the current GameTask
				GameTask info(maps[map], strategiesThisRun);
				for (int rep=0; rep<timesToRepeatEachGame; rep++)
					gameTaskList.push_back(info);
			}
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
	
	#ifndef DONTUSEMPI
		//launch initial games to get all slaves working
		int slavesToUse = (gamesToRun.size()>=slaveTasks.size() ? slaveTasks.size() : gamesToRun.size());
		for (int i=0; i<slavesToUse; i++)
			launchGame(i,i);
		for (int i=slavesToUse; i<slaveTasks.size(); i++)
			informThatIsDone(i);	//tell unneeded slaves they are done
		int nextGameNumber = slavesToUse;	//since we just assigned game number (slavesToUse-1)
	
		//communicate, until all done
		for (int i=0; i<gamesToRun.size(); i++)
		{
			int whoDone = getAndHandleReport(&outputStream);
			if (nextGameNumber<gamesToRun.size())
			{
				launchGame(whoDone, nextGameNumber);
				nextGameNumber++;
			}
			else
			{
				informThatIsDone(whoDone);
			}
		}
	#else
		//just run it locally
		for (int i=0; i<gamesToRun.size(); i++)
		{
			GameReport report = Game::quickRun(gamesToRun[i]);
			report.write(&outputStream);
		}
	#endif
	
	//close output file
	outputStream.close();
}

#ifndef DONTUSEMPI

void GameManager::launchGame(int slaveNumber, int gameNumber)
{
	slaveTasks[slaveNumber] = gameNumber;
	int dataOut[GameTask::encodedSize];
	gamesToRun[gameNumber].encode(dataOut);

	std::cout<<"TASK AS SENT:"<<std::endl;
	for ( int i = 0; i <GameTask::encodedSize; i++)
            std::cout << dataOut[i] << " ";
        std::cout << std::endl;

	MPI_Send(&dataOut, GameTask::encodedSize, MPI_INT, slaveNumber+1, MPI_TASK, MPI_COMM_WORLD);
}

int GameManager::getAndHandleReport(std::ostream *outputStream)
{
	GameReport report;

	int dataIn[GameReport::encodedSize];
	MPI_Status status;
	MPI_Recv(&dataIn, GameReport::encodedSize, MPI_INT, MPI_ANY_SOURCE, MPI_REPORT, MPI_COMM_WORLD, &status);
        int number = 0;
        MPI_Get_count(&status, MPI_INT, &number);
	std::cout<<"REPORT AS RECEIVED: " << number << " " << std::endl;
	for ( int i = 0; i <GameReport::encodedSize; i++)
            std::cout << dataIn[i] << " ";
        std::cout << std::endl;
        
        
        

	report.decode(dataIn);
	int slaveNumber = status.MPI_SOURCE - 1;
	slaveTasks[slaveNumber] = -1;	//mark this slave as not working on anything

        //report.write(&(std::cout));
	report.write(outputStream);
	return slaveNumber;
}

void GameManager::informThatIsDone(int slaveNumber)
{
	StrategyEnum::StrategyEnum fakePlayers[] = { StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER, StrategyEnum::NOPLAYER };
	GameTask fakeTask(MapEnum::Earth, fakePlayers);
	int dataOut[GameTask::encodedSize];		//make it the same size as a Task, since that's what the slave will be listening for
	fakeTask.encode(dataOut);
        
	std::cout<<"TASK AS SENT:"<<std::endl;
	for ( int i = 0; i <GameTask::encodedSize; i++)
            std::cout << dataOut[i] << " ";
        std::cout << std::endl;
	
	fakeTask.encode(dataOut);
	MPI_Send(&dataOut, GameTask::encodedSize, MPI_INT, slaveNumber+1, MPI_TASK, MPI_COMM_WORLD);
}

#endif
