#include "GameTask.hpp"

GameTask::GameTask()
{ //all of these should end up being set later, anyhow; these are public variables
    map = MapEnum::Earth; //the map where the game occurred
    for (int i = 0; i < 6; i++)
        players[i] = StrategyEnum::NOPLAYER;
}

GameTask::GameTask(MapEnum::MapEnum theMap, StrategyEnum::StrategyEnum thePlayers[])
{
    map = theMap;
    for (int i = 0; i < 6; i++)
        players[i] = thePlayers[i];
}

void GameTask::encode(int outData[])
{
    outData[0] = map;
    for (int i = 0; i < 6; i++)
        outData[i + 1] = players[i];
}

void GameTask::decode(int inData[])
{
    map = (MapEnum::MapEnum)inData[0];
    for (int i = 0; i < 6; i++)
        players[i] = (StrategyEnum::StrategyEnum)inData[i + 1];
}

void GameTask::print(int data[])
{
    std::cout << "Printing Data!" << std::endl;
    
    for ( int i = 0; i < sizeof(data)/sizeof(*data); i++)
    {
        std::cout << data[i] << " ";
    }
    
    std::cout << std::endl;
}