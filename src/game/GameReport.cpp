#include "GameReport.hpp"

GameReport::GameReport()
{
	rounds = 0;	
	map = MapEnum::Earth;
	for (int i=0; i<6; i++)
	{
		players[i] = StrategyEnum::NOPLAYER;
		winners[i] = -1;
	}
}
GameReport::GameReport(int theRounds, MapEnum::MapEnum theMap, StrategyEnum::StrategyEnum thePlayers[], int theWinners[])
{
	rounds = theRounds;
	map = theMap;
	for (int i=0; i<6; i++)
	{
		players[i] = thePlayers[i];
		winners[i] = theWinners[i];
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
		winners[i] = inData[i+8];
	}
}

void GameReport::write(std::ostream *outputStream)
{
	int numberOfPlayers = 0;
	while (numberOfPlayers<6 && players[numberOfPlayers]!=StrategyEnum::NOPLAYER)
		numberOfPlayers++;
	*outputStream << rounds <<";"<< map;
	for (int i=0; i<numberOfPlayers; i++)
		*outputStream <<(i==0?';':',')<< players[i];
	for (int i=0; i<numberOfPlayers; i++)
		*outputStream <<(i==0?';':',')<< winners[i];
	*outputStream << std::endl;
}
