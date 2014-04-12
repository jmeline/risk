#include "GameManager.hpp"
#include "GameSlave.hpp"

#include <random>
#include <time.h>
#ifndef DONTUSEMPI
	#include <mpi.h>
#endif

// yep... it's a global variable.  But it makes sense here.
std::default_random_engine rng;

int main(int argc, char** argv)
{
	//Get Inputs
	std::string outputFileString = "output.txt";
	int timesToRepeatEachGame = 3;
	int numberOfPlayers = 2;
	if (argc>=2)
		outputFileString = std::string(argv[1]);
	if (argc>=3)
		timesToRepeatEachGame = atoi(argv[2]);
	if (argc>=4)
		numberOfPlayers = atoi(argv[3]);
	
	//set up MPI , and then init the randomizer
	int myMpiRank, totalProcCount;
	#ifndef DONTUSEMPI
		MPI_Init(NULL, NULL);
		MPI_Comm_rank(MPI_COMM_WORLD, &myMpiRank);
		MPI_Comm_size(MPI_COMM_WORLD, &totalProcCount);
	#else
		myMpiRank = 0,
		totalProcCount = 1;
	#endif
	srand((unsigned int)time(NULL)+10*myMpiRank);		//We must seed the random number generator.  This one is only used for initializing; rng is used more

	//Spawn the appropriate object
	if (myMpiRank==0)
	{
		//prepare tasks to run
		std::vector<StrategyEnum::StrategyEnum> strategies;
			strategies.push_back(StrategyEnum::HumanControlledStrategy);
			////strategies.push_back(StrategyEnum::BadStrategy1);
			////strategies.push_back(StrategyEnum::ObtainSmallestContinentsFirstStrategy);
		std::vector<MapEnum::MapEnum> maps;
			////maps.push_back(MapEnum::Earth);
			maps.push_back(MapEnum::Island);
		std::vector<GameTask> tasksToRun = GameManager::getRunsFor(strategies,maps,numberOfPlayers,timesToRepeatEachGame);
		//start the manager
		GameManager manager(totalProcCount-1);
		manager.setGamesToRun(tasksToRun);
		////manager.readInExistingReport("reportLocation");
		manager.runIt(outputFileString);
	}
	else
	{
		GameSlave slave;
		slave.runIt();
	}

	//clean up MPI stuff
	#ifndef DONTUSEMPI
		MPI_Finalize();
	#endif
}
