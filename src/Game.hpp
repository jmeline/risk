/*
 * =====================================================================================
 *       Filename:  Game.hpp
 *    Description: Manages the flow of a game
 * =====================================================================================
 */

#ifndef  game_INC
#define  game_INC

#include "gamemap\gamemap.hpp"
#include "GameState.hpp"
#include "Strategy.hpp"

class Game
{
private:
	gamemap map;
	GameState state;
	int numberOfPlayers = 0;
	Strategy** players;

public:
	Game(gamemap gMap) {
		map = gMap;
		players = new (Strategy*)[4];
		players[0] = null;
		players[1] = null;
		players[2] = null;
		players[3] = null;
		state = State.createInitialStateFromMap(map);
	}

	/* Adds a strategy for the next player, up to 4 */
	void addPlayer(Strategy s) {
		if (numberOfPlayers < 4) {
			strategies[numberOfPlayers] = s;
			numberOfPlayers++;
		}
	}

	/* Uses the strategies to run the game through */
	void runGame();
};

#endif   /* ----- #ifndef game_INC  ----- */
