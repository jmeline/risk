/*
 * =====================================================================================
 *       Filename:  Game.hpp
 *    Description: Manages the flow of a game
 * =====================================================================================
 */

#include "Game.hpp"

Game::Game(gamemap gMap) {
	map = gMap;
	strategies = new (Strategy*)[4];
	strategies[0] = null;
	strategies[1] = null;
	strategies[2] = null;
	strategies[3] = null;
	state = State.createInitialStateFromMap(map);
}

void Game::addPlayer(Strategy s) {
	if (numberOfPlayers < 4) {
		strategies[numberOfPlayers] = s;
		numberOfPlayers++;
	}
}

void Game::runGame() {
	std::cout << 
}