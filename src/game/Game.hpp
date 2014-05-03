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
#include <array>

class Game
{
public:
	static const int PROBABILITYCACHESIZE = 50;
	static bool probabilityCacheIsInitialized;
//	static double probabilityCache[PROBABILITYCACHESIZE*PROBABILITYCACHESIZE];
	static double* probabilityCache;

private:
    GameMap* map;
    Strategy** player;
    GameState state;
    int numberOfPlayers;
	
	////static std::array<std::array<double,PROBABILITYCACHESIZE>,PROBABILITYCACHESIZE> probabilityCache;

public:
    Game(GameTask task);
    ~Game();

    /* Uses the strategies to run the game through. */
    GameReport runGame();

    static GameReport quickRun(GameTask task);

    /* The first double is the probability of attacker victory.  The second is the probability of a tie.  Probability of defender victory is what remains. */
    static std::pair<double, double> getProbability(int attackingDice, int defendingDice);

    /* A useful method for strategies: Gives the probability of victory after repeated combat.
     * NOTE: "attacking" should refer to AVAILIABLE armies, which is occupyingArmies-1 (someone must stay to defend the land) */
    static double getProbabilityOfVictory(int attacking, int defending);

	/* Initializes probabilityCache.  Called only by getProbabilityOfVictory, only once */
	static void initializeProbabilityCache();

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
