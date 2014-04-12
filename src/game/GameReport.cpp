#include "GameReport.hpp"

GameReport::GameReport()
{		//all of these should end up being set later, anyhow; these are public variables
	rounds = 0;						//how long the game lasted
	map = MapEnum::Earth;			//the map where the game occurred
	for (int i=0; i<6; i++)
	{
		players[i] = StrategyEnum::NOPLAYER;
		winners[i] = StrategyEnum::NOPLAYER;
	}
}

void GameReport::write(std::ofstream outputStream)
{
	outputStream << rounds;
	outputStream << map;
	for (int i=0; i<6; i++)
		outputStream << players[i];
	for (int i=0; i<6; i++)
		outputStream << winners[i];
}

void GameReport::read(std::ifstream inputStream)
{
	int temp;	//used for our enums, before casting
	inputStream >> rounds;
	inputStream >> temp;
	map = (MapEnum::MapEnum)temp;
	for (int i=0; i<6; i++)
	{
		inputStream >> temp;
		players[i] = (StrategyEnum::StrategyEnum)temp;
	}
	for (int i=0; i<6; i++)
	{
		inputStream >> temp;
		winners[i] = (StrategyEnum::StrategyEnum)temp;
	}
}
