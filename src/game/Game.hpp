/*
 * =====================================================================================
 *       Filename:  Game.hpp
 *    Description: Manages the flow of a game
 * =====================================================================================
 */

#ifndef  game_INC
#define  game_INC

#include "../gamemap/GameMap.hpp"
#include "GameTask.hpp"
#include "GameState.hpp"
#include "GameReport.hpp"
#include "Strategy.hpp"

class Game {
private:
    GameMap* map;
    Strategy** player;
	GameState state;
    int numberOfPlayers;

public:
	Game(GameTask task);
	~Game();

    /* Uses the strategies to run the game through. */
    GameReport runGame();

private:
    /* Lets the players take turns choosing countries to claim until all are claimed */
    void claimCountries();

    /* Gives out initial troops and lets the players place them */
    void placeFirstTroops();

    /* Gives out the troops for a round and lets the current player place them */
    void getAndPlaceTroops(int whoseTurn);

    /* Lets the current player do battle as much as desired for a turn.
     * Returns a vector of all players defeated during these battles (if any), given in order of death. */
    std::vector<int> doATurnOfBattles(int whoseTurn);

    /* Gives the current player a chance to fortify by moving troops around. */
    void fortify(int whoseTurn);
    //int numTroops = getNumTroopsToAdd(whoseTurn);
    //std::vector<std::pair<int,int>> troopsToPlace

    /* Rolls dice (numToAttackWith vs numToDefendWith), and returns a number representing the outcome.
     * Meaning of 2,1,0,-1,-2: attacker won 2, attacker won 1, each won one, defender won 1, defender won 2 */
    int rollToConquer(int numToAttackWith, int numToDefendWith);

    /* Returns true is there are no countries left owned by a given player. */
    bool isTotallyDefeated(int playerNumber);
};

#endif   /* ----- #ifndef game_INC  ----- */
