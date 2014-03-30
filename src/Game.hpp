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

class Game {
private:
	GameMap map;
	GameState state;
	int numberOfPlayers = 0;
	Strategy** player;

public:
	Game(GameMap gMap);

	/* Adds a strategy for the next player, up to 6 */
	void addPlayer(Strategy* s);

	/* Uses the strategies to run the game through.
	 * Returns a vector of winners: the first came in first, the second came in second, etc */
	std::vector<int> runGame();

private:
	/* Lets the players take turns choosing countries to claim until all are claimed */
	void claimCountries();

	/* Gives out initial troops and lets the players place them */
	void placeFirstTroops();

	/* Gives out the troops for a round and lets the current player place them */
	void getAndPlaceTroops(int whoseTurn);

	/* Lets the current player do battle as much as desired for a turn
	 * Returns a vector of all players defeated during these battles (if any), given in order of death */
	std::vector<int> doATurnOfBattles(int whoseTurn);
	
	/* Gives the current player a chance to fortify by moving troops around */
	void fortify(int whoseTurn);
	//int numTroops = getNumTroopsToAdd(whoseTurn);
	//std::vector<std::pair<int,int>> troopsToPlace
};

#endif   /* ----- #ifndef game_INC  ----- */
