#include "GameManager.hpp"
#include "GameSlave.hpp"

#include <random>
#include <iostream>
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
    if (argc >= 2)
        outputFileString = std::string(argv[1]);
    if (argc >= 3)
        timesToRepeatEachGame = atoi(argv[2]);
    if (argc >= 4)
        numberOfPlayers = atoi(argv[3]);

    std::cout << "**********************************" << std::endl;
    std::cout << "Configurations" << std::endl;
    std::cout << "OutputFile: " << outputFileString << std::endl;
    std::cout << "TimesToRepeatEachGame: " << timesToRepeatEachGame << std::endl;
    std::cout << "NumberOfPlayers: " << numberOfPlayers << std::endl;
    std::cout << "**********************************" << std::endl;

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
    srand((unsigned int) time(NULL) + 10 * myMpiRank); //We must seed the random number generator.  This one is only used for initializing; rng is used more

    //Spawn the appropriate object
    if (myMpiRank == 0)
    {
        std::cout << "Starting Master: " << myMpiRank << std::endl;

        //prepare tasks to run
        std::vector<StrategyEnum::StrategyEnum> strategies;
        strategies.push_back(StrategyEnum::HumanControlledStrategy);
        strategies.push_back(StrategyEnum::AggressiveStrategy);
		strategies.push_back(StrategyEnum::ObtainSmallestContinentsFirstStrategy);
		////strategies.push_back(StrategyEnum::PacifistStrategy);
		////strategies.push_back(StrategyEnum::ImmediateBestValueStrategy1);
		////strategies.push_back(StrategyEnum::PreyOnWeakStrategy);
		///strategies.push_back(StrategyEnum::BadStrategy1);

        std::cout << "Strategies being used: ";
        for (StrategyEnum::StrategyEnum e : strategies)
        {
            std::cout << e << " ";
        }
        std::cout << std::endl;

        std::vector<MapEnum::MapEnum> maps;
        maps.push_back(MapEnum::Earth);
		////maps.push_back(MapEnum::ThreeContinent);
        ////maps.push_back(MapEnum::Island);

        std::cout << "Maps being used: ";
        for (MapEnum::MapEnum e : maps)
        {
            std::cout << e << " ";
        }
        std::cout << std::endl;

        std::vector<GameTask> tasksToRun = GameManager::getNonrepeatingPermutationsFor(strategies, maps, numberOfPlayers, 1);//timesToRepeatEachGame);

		for (int i=0; i<tasksToRun.size(); i++) {
			for (int j=0; j<6; j++)
				std::cout<<tasksToRun[i].players[j]<<" ";
			std::cout<<std::endl;
		}



        //start the manager
        GameManager manager(totalProcCount - 1);
        manager.setGamesToRun(tasksToRun);
        ////manager.readInExistingReport("reportLocation");
        manager.runIt(outputFileString);
    }
    else
    {
        std::cout << "Starting Slave: " << myMpiRank << std::endl;

        GameSlave slave;
        slave.runIt();
    }

    std::cout << "My MPI Rank has finished: " << myMpiRank << std::endl;

    //clean up MPI stuff
#ifndef DONTUSEMPI
    //MPI_Barrier(MPI_COMM_WORLD);
    //std::cout << "Leaving Barrier" << myMpiRank << std::endl;
    MPI_Finalize();
#endif
}
