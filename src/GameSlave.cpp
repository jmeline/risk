#include "GameSlave.hpp"

#include <iostream>
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
        {
            std::cout << "Stopping Slave!" << std::endl;
            break; //we got the "stop" task
        }

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
    std::cout << "Running Receive Game Task" << std::endl;

    GameTask task;
    int dataIn[GameTask::encodedSize];
    //std::cout << "Size of DataIn: " << (sizeof (dataIn) / sizeof (*dataIn)) << std::endl;
#ifndef DONTUSEMPI
    MPI_Recv(&dataIn, GameTask::encodedSize, MPI_INT, 0, MPI_TASK, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    std::cout << "TASK AS RECEIVED:" << std::endl;
    for (int i = 0; i < GameTask::encodedSize; i++)
        std::cout << dataIn[i] << " ";
    std::cout << std::endl;

    task.decode(dataIn);
    //task.print(dataIn);
#else
    std::cout << "MPI is disabled.  Would have recieved a GameTask." << std::endl;
#endif
    return task;
}

void GameSlave::sendReport(GameReport report)
{
    std::cout << "Running Send Report" << std::endl;
    int dataOut[GameReport::encodedSize];
    report.encode(dataOut);
#ifndef DONTUSEMPI
    std::cout << "REPORT AS SENT:" << std::endl;
    for (int i = 0; i < GameReport::encodedSize; i++)
        std::cout << dataOut[i] << " ";
    std::cout << std::endl;

    MPI_Send(&dataOut, GameReport::encodedSize, MPI_INT, 0, MPI_REPORT, MPI_COMM_WORLD);
#else
    std::cout << "MPI is disabled.  Would have sent a report." << std::endl;
#endif
}
