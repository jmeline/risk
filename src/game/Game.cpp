
/*
 * =====================================================================================
 *       Filename:  Game.hpp
 *    Description: Manages the flow of a game
 * =====================================================================================
 */

#include <vector>
#include <utility>
#include <random>
#include <iostream>
#include "Game.hpp"


std::default_random_engine rng;

Game::Game(GameTask task)
{
    map = GameMap::make(task.map);
    state = GameState(map);
    player = new Strategy*[6];
    numberOfPlayers = 0;
    for (int i = 0; i < 6; i++)
    {
        if (task.players[i] == StrategyEnum::NOPLAYER)
            break;
        player[i] = Strategy::make(task.players[i]);
        (player[i])->init(map, i);
        numberOfPlayers++;
    }
    if (numberOfPlayers < 2)
    {
        for (int i = numberOfPlayers; i < 2; i++)
            player[i] = Strategy::make(StrategyEnum::NOPLAYER); //let it use the default
        numberOfPlayers = 2;
    }
    for (int i = numberOfPlayers; i < 6; i++)
        player[i] = NULL;
}

Game::~Game()
{
    delete map;
    for (int i = numberOfPlayers; i < 6; i++)
        delete player[i];
}

GameReport Game::runGame()
{
    claimCountries();
    placeFirstTroops();
    GameReport report;
    report.rounds = 0;
    report.map = map->getIdentifier();
    for (int i = 0; i < numberOfPlayers; i++)
        report.players[i] = player[i]->getIdentifier();
    bool isDead[] = {false, false, false, false, false, false};
    int numberDead = 0;
    int whoseTurn = 0;
    while (true)
    {
        if (whoseTurn == 0)
            report.rounds++;
        if (!isDead[whoseTurn])
        {
            getAndPlaceTroops(whoseTurn);
            std::vector<int> killedInConquest = doATurnOfBattles(whoseTurn);
            for (int i = 0; i < killedInConquest.size(); i++)
            {
                numberDead++;
                report.winners[numberOfPlayers - numberDead] = killedInConquest[i];
                isDead[killedInConquest[i]] = true;
            }
            if (numberDead == (numberOfPlayers - 1))
            {
                report.winners[0] = whoseTurn;
                break;
            }
            fortify(whoseTurn);
        }
        whoseTurn = (whoseTurn + 1) % numberOfPlayers;
    }
    return report;
}

void Game::claimCountries()
{
    int whoseTurn = 0;
    for (int i = 0; i < this->map->getNumberOfRegions(); i++)
    {
        int regionToClaim = -1;
        while (regionToClaim < 0)
        {
            regionToClaim = player[whoseTurn]->claim(state);
            if (regionToClaim < 0 || regionToClaim >= state.getNumRegions() || state.getRegionInfo(regionToClaim).first != -1)
                regionToClaim = -1; //they chose an already-chosen region
        }
        state.setRegionInfo(regionToClaim, std::pair<int, int>(whoseTurn, 1));
        whoseTurn = (whoseTurn + 1) % numberOfPlayers;
    }
}

void Game::placeFirstTroops()
{
    int numberPerPlayer = 50 - 5 * numberOfPlayers;
    int numberOfRegions = map->getNumberOfRegions();
    int piecesAlreadyUsed = numberOfRegions / numberOfPlayers;
    int playersWhoPlacedExtra = numberOfRegions % numberOfPlayers;
    for (int i = 0; i < numberOfPlayers; i++)
    {
        int numTroops = numberPerPlayer - piecesAlreadyUsed - ((i < playersWhoPlacedExtra) ? 1 : 0);
        while (numTroops > 0)
        {
            std::vector<std::pair<int, int>> placementActions = player[i]->place(state, numTroops);
            for (int j = 0; j < placementActions.size(); j++)
            {
                int wherePut = placementActions[j].first;
                int howMany = placementActions[j].second;
                std::pair<int, int> regionInfo = state.getRegionInfo(wherePut);
                if (regionInfo.first == i && numTroops > 0)
                {
                    regionInfo.second += (howMany < numTroops) ? howMany : numTroops;
                    numTroops -= howMany;
                    state.setRegionInfo(wherePut, regionInfo);
                }
            }
        }
    }
}

void Game::getAndPlaceTroops(int whoseTurn)
{
    //get number of troops
    int numOfTroops = state.getNumberOccupiedBy(whoseTurn) / 3;
    if (numOfTroops < 3)
        numOfTroops = 3;
    std::vector<Continent> continentList = map->getContinentList();
    for (int i = 0; i < continentList.size(); i++)
    {
        std::vector<Region> regionList = continentList[i].getRegionList();
        bool gotIt = true;
        for (int j = 0; j < regionList.size(); j++)
        {
            if (state.getRegionInfo(regionList[j].first).first != whoseTurn)
            {
                gotIt = false;
                break;
            }
        }
        if (gotIt)
            numOfTroops += continentList[i].getTroopValue();
    }
    //place them
    while (numOfTroops > 0)
    {
        std::vector<std::pair<int, int>> placementActions = player[whoseTurn]->place(state, numOfTroops);
        for (int j = 0; j < placementActions.size(); j++)
        {
            int wherePut = placementActions[j].first;
            int howMany = placementActions[j].second;
            std::pair<int, int> regionInfo = state.getRegionInfo(wherePut);
            if (regionInfo.first == whoseTurn && numOfTroops > 0)
            {
                regionInfo.second += (howMany < numOfTroops) ? howMany : numOfTroops;
                numOfTroops -= howMany;
                state.setRegionInfo(wherePut, regionInfo);
            }
        }
    }
}

/* Lets the current player do battle as much as desired for a turn
 * Returns a vector of all players defeated during these battles (if any), given in order of death */
std::vector<int> Game::doATurnOfBattles(int whoseTurn)
{
    std::vector<int> fallenPlayers;
    int numberOfPlayersEliminated = 0;
    while (true)
    { //we will keep going until the player's choice is a "magic value" indicating to end the turn
        std::pair<int, int> attackInfo = player[whoseTurn]->attack(state);
        int attackTo = attackInfo.first;
        int attackFrom = attackInfo.second;
        if (attackFrom < 0 || attackTo < 0) //the "magic value" indicating "all done" received
            break;
        std::pair<int, int> attackedRegionInfo = state.getRegionInfo(attackTo);
        std::pair<int, int> attackingRegionInfo = state.getRegionInfo(attackFrom);
        int playerAttacked = attackedRegionInfo.first;
        if (attackingRegionInfo.first == whoseTurn && playerAttacked != whoseTurn && attackingRegionInfo.second > 1 && map->areConnected(attackFrom, attackTo))
        {
            int numToDefendWith = player[playerAttacked]->defend(state, attackTo, attackFrom);
            int numToAttackWith = (attackingRegionInfo.second >= 4) ? 3 : ((attackingRegionInfo.second >= 3) ? 2 : 1); //we may later give the player a choice, but for now this works
            ////roll and compare dice.  Meaning of 2,1,0,-1,-2:  attacker wins 2, attacker wins 1, each wins one, defender wins 1, defender wins 2
            int numberConquered = rollToConquer(numToAttackWith, numToDefendWith);
            if (numberConquered > 0)
            {
                attackedRegionInfo.second -= numberConquered;
                if (attackedRegionInfo.second == 0)
                { //conquered; see if the looser is all dead
                    if (isTotallyDefeated(attackedRegionInfo.first))
                        fallenPlayers.push_back(attackedRegionInfo.first);
                    attackedRegionInfo.first = attackingRegionInfo.first;
                    attackedRegionInfo.second = numToAttackWith;
                    attackingRegionInfo.second -= numToAttackWith;
                }
            }
            else if (numberConquered < 0)
            {
                attackingRegionInfo.second += numberConquered; //note that numberConquered is a negative number, here
            }
            else
            { //they both took a hit
                attackedRegionInfo.second -= 1;
                attackingRegionInfo.second -= 1;
                if (attackedRegionInfo.second == 0)
                { //conquered; see if the looser is all dead
                    if (isTotallyDefeated(attackedRegionInfo.first))
                        fallenPlayers.push_back(attackedRegionInfo.first);
                    attackedRegionInfo.first = attackingRegionInfo.first;
                    attackedRegionInfo.second = numToAttackWith - 1;
                    attackingRegionInfo.second -= numToAttackWith - 1;
                }
            }
        }
        state.setRegionInfo(attackFrom, attackingRegionInfo);
        state.setRegionInfo(attackTo, attackedRegionInfo);
    }
    return fallenPlayers;
}

/* Gives the current player a chance to fortify by moving troops around */
void Game::fortify(int whoseTurn)
{
    //get player input, then prepare vectors needed to ensure valid moves
    std::vector<std::tuple<int, int, int>> moves = player[whoseTurn]->fortify(state);
    int numberOfRegions = map->getNumberOfRegions();
    std::vector<int> adjustmentsToMake(numberOfRegions); //tracks what change is needed in each region.  We don't modify the state until we know all changes to make.
    std::vector<bool> regionAlreadyMoved(numberOfRegions);
    for (int i = 0; i < numberOfRegions; i++)
    {
        adjustmentsToMake[i] = 0;
        regionAlreadyMoved[i] = false;
    }

    //load adjustmentsToMake, asserting move validity along the way
    for (int i = 0; i < moves.size(); i++)
    {
        int regionFrom = std::get<0>(moves.at(i));
        int regionTo = std::get<1>(moves.at(i));
        int numTroops = std::get<2>(moves.at(i));
        if (!regionAlreadyMoved[regionFrom] && state.getRegionInfo(regionFrom).first == whoseTurn && state.getRegionInfo(regionTo).first == whoseTurn && numTroops < state.getRegionInfo(regionFrom).second && map->areConnected(regionFrom, regionTo))
        {
            regionAlreadyMoved[regionFrom] = true;
            adjustmentsToMake[regionFrom] -= numTroops;
            adjustmentsToMake[regionTo] += numTroops;
        }
    }

    //move the troops
    for (int i = 0; i < adjustmentsToMake.size(); i++)
    {
        if (adjustmentsToMake[i] == 0)
            continue;
        std::pair<int, int> regionInfo = state.getRegionInfo(i);
        regionInfo.second += adjustmentsToMake[i];
        state.setRegionInfo(i, regionInfo);
    }
}

/* Rolls dice (numToAttackWith vs numToDefendWith), and returns a number representing the outcome.
 * Meaning of 2,1,0,-1,-2: attacker won 2, attacker won 1, each won one, defender won 1, defender won 2 */
int Game::rollToConquer(int numToAttackWith, int numToDefendWith)
{
    //determine how many possibilities there are, and roll one of those states
    int numOfDice = numToAttackWith + numToDefendWith;
    int numOfPossibilities = 36;
    if (numOfDice > 2)
        numOfPossibilities *= ((numOfDice == 5) ? 216 : ((numOfDice == 4) ? 36 : 6));
    std::uniform_int_distribution<int> distribution(0, numOfPossibilities - 1);
    int rollResult = distribution(rng);
    //determine the return value for the state.  See "dice probability notes.txt" for calculation steps
    if (numToDefendWith == 1)
    {
        if (numToAttackWith == 1)
        {
            if (rollResult < 15) return 1; //probability 15/36
            else return -1; //probability 21/36
        }
        else if (numToAttackWith == 2)
        {
            if (rollResult < 125) return 1; //probability 125/216
            else return -1; //probability 91/216
        }
        else if (numToAttackWith == 3)
        {
            if (rollResult < 853) return 1; //probability 853/1296
            else return -1; //probability 443/1296
        }
    }
    else if (numToDefendWith == 2)
    {
        if (numToAttackWith == 1)
        {
            if (rollResult < 55) return 1; //probability 55/216
            else return -1; //probability 161/216
        }
        else if (numToAttackWith == 2)
        {
            if (rollResult < 295) return 2; //probability 295/1296
            else if (rollResult < (295 + 420)) return 0; //probability 420/1296
            else return -2; //probability 581/1296
        }
        else if (numToAttackWith == 3)
        {
            if (rollResult < 2890) return 2; //probability 2890/7776
            else if (rollResult < (2890 + 2611)) return 0; //probability 2611/7776
            else return -2; //probability 2275/7776
        }
    }
    return 0; //here for compiling, but this point should never be reached
}

/* Returns true is there are no countries left owned by a given player. */
bool Game::isTotallyDefeated(int playerNumber)
{
    for (int i = 0; i < map->getNumberOfRegions(); i++)
    {
        if (state.getRegionInfo(i).first == playerNumber)
            return false;
    }
    return true;
}
