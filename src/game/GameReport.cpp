#include "GameReport.hpp"

GameReport::GameReport()
{
	rounds = 0;	
	map = MapEnum::Earth;
	for (int i=0; i<6; i++)
	{
		players[i] = StrategyEnum::NOPLAYER;
		winners[i] = StrategyEnum::NOPLAYER;
	}
}
GameReport::GameReport(int theRounds, MapEnum::MapEnum theMap, StrategyEnum::StrategyEnum thePlayers[], StrategyEnum::StrategyEnum theWinners[])
{
	rounds = theRounds;
	map = theMap;
	for (int i=0; i<6; i++)
	{
		players[i] = thePlayers[i];
		winners[i] = theWinners[i];
	}
}

void GameReport::write(std::ofstream *outputStream)
{
	*outputStream << rounds;
	*outputStream << map;
	for (int i=0; i<6; i++)
		*outputStream << players[i];
	for (int i=0; i<6; i++)
		*outputStream << winners[i];
}

void GameReport::read(std::ifstream *inputStream)
{
	int temp;	//used for our enums, before casting
	*inputStream >> rounds;
	*inputStream >> temp;
	map = (MapEnum::MapEnum)temp;
	for (int i=0; i<6; i++)
	{
		*inputStream >> temp;
		players[i] = (StrategyEnum::StrategyEnum)temp;
	}
	for (int i=0; i<6; i++)
	{
		*inputStream >> temp;
		winners[i] = (StrategyEnum::StrategyEnum)temp;
	}
}

void GameReport::encode(int outData[])
{
	outData[0] = rounds;
	outData[1] = map;
	for (int i=0; i<6; i++)
	{
		outData[i+2] = players[i];
		outData[i+8] = winners[i];
	}
}

void GameReport::decode(int inData[])
{
	rounds = inData[0];
	map = (MapEnum::MapEnum)inData[1];
	for (int i=0; i<6; i++)
	{
		players[i] = (StrategyEnum::StrategyEnum)inData[i+2];
		winners[i] = (StrategyEnum::StrategyEnum)inData[i+8];
	}
}
