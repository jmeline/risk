#include "GameSlave.hpp"
#ifndef DONTUSEMPI
#include <mpi.h>
#define MPI_TASK 1		//identifies a type of MPI message
#define MPI_REPORT 2		//identifies a type of MPI message
#endif

void GameSlave::runIt()
{
    while (true)
    {
        GameTask task = receiveGameTask();
        if (task.players[0] == StrategyEnum::NOPLAYER 
                && task.players[1] == StrategyEnum::NOPLAYER 
                && task.players[2] == StrategyEnum::NOPLAYER 
                && task.players[3] == StrategyEnum::NOPLAYER 
                && task.players[4] == StrategyEnum::NOPLAYER 
                && task.players[5] == StrategyEnum::NOPLAYER)
            break; //we got the "stop" task
        GameReport report = runOneGame(task);
        sendReport(report);
    }
}

GameReport GameSlave::runOneGame(GameTask task)
{
    return Game::quickRun(task);
}

GameTask GameSlave::receiveGameTask()
{
    GameTask task;
    int dataIn[GameTask::encodedSize];
#ifndef DONTUSEMPI
    MPI_Recv(&dataIn, GameTask::encodedSize, MPI_INT, 0, MPI_TASK, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    task.encode(dataIn);
#else
    std::cout << "MPI is disabled.  Would have recieved a GameTask." << std::endl;
#endif
    return task;
}

void GameSlave::sendReport(GameReport report)
{
    int dataOut[GameReport::encodedSize];
    report.encode(dataOut);
#ifndef DONTUSEMPI
    MPI_Send(&dataOut, GameTask::encodedSize, MPI_INT, 0, MPI_REPORT, MPI_COMM_WORLD);
#else
    std::cout << "MPI is disabled.  Would have sent a report." << std::endl;
#endif
}
