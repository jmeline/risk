/*
 * =====================================================================================
 *       Filename:  Game.cpp
 *    Description: Manages the flow of a game
 * =====================================================================================
 */

#include <vector>
#include <utility>
#include <random>
#include <iostream>
#include "Game.hpp"


extern std::default_random_engine rng;

GameReport Game::quickRun(GameTask task)
{
	Game game(task);
	return game.runGame();
}

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
		std::cout<<"Type any char to continue>";
		char ch;
		std::cin >> ch;
    }
    return report;
}

void Game::claimCountries()
{
    int whoseTurn = 0;
    for (int i = 0; i < this->map->getNumberOfRegions(); i++)
    {
        std::cout << "this->map->getNumberOfRegions() " << this->map->getNumberOfRegions() << std::endl;
        int regionToClaim = -1;
        while (regionToClaim < 0)
        {
            std::cout << "regionToClaim: " << regionToClaim << std::endl;
            regionToClaim = player[whoseTurn]->claim(state);
            std::cout << "returning value" << regionToClaim << std::endl;
            if (regionToClaim < 0 || regionToClaim >= state.getNumRegions() || state.getRegionInfo(regionToClaim).first != -1)
                regionToClaim = -1; //they chose an already-chosen region
        }
        state.setRegionInfo(regionToClaim, std::pair<int, int>(whoseTurn, 1));
        whoseTurn = (whoseTurn + 1) % numberOfPlayers;
    }
}

void Game::placeFirstTroops()
{
    int numberPerPlayer = map->startingTroops - map->perPlayerTroopPenalty * numberOfPlayers;
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
		bool regionConquered = false;
		int defeatedPlayer = 0;
        int attackFrom = attackInfo.first;
		int attackTo = attackInfo.second;
        if (attackFrom < 0 || attackTo < 0) //the "magic value" indicating "all done" received
            break;
        std::pair<int, int> attackedRegionInfo = state.getRegionInfo(attackTo);
        std::pair<int, int> attackingRegionInfo = state.getRegionInfo(attackFrom);
        int playerAttacked = attackedRegionInfo.first;
        if (attackingRegionInfo.first == whoseTurn && playerAttacked != whoseTurn && attackingRegionInfo.second > 1 && map->areConnected(attackFrom, attackTo))
        {
			int numToDefendWith = (attackedRegionInfo.second >= 2) ? 2 : 1;
            int numToAttackWith = (attackingRegionInfo.second >= 4) ? 3 : ((attackingRegionInfo.second >= 3) ? 2 : (attackingRegionInfo.second ? 1 : 0));	//we may later give the player a choice, but for now this works
            ////roll and compare dice.  Meaning of 2,1,0,-1,-2:  attacker wins 2, attacker wins 1, each wins one, defender wins 1, defender wins 2
			if (numToAttackWith > 0)
			{
				int numberConquered = rollToConquer(numToAttackWith, numToDefendWith);
				if (numberConquered > 0)
				{
					attackedRegionInfo.second -= numberConquered;
					if (attackedRegionInfo.second == 0)
					{ //conquered;
						regionConquered = true;
						defeatedPlayer = attackedRegionInfo.first;
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
					{ //conquered;
						regionConquered = true;
						defeatedPlayer = attackedRegionInfo.first;
						attackedRegionInfo.first = attackingRegionInfo.first;
						attackedRegionInfo.second = numToAttackWith - 1;
						attackingRegionInfo.second -= numToAttackWith - 1;
					}
				}
			}
        }

        state.setRegionInfo(attackFrom, attackingRegionInfo);
        state.setRegionInfo(attackTo, attackedRegionInfo);

		// Check if a player has been completely defeated.
		if(regionConquered)		//only if we have to
		{
			if (isTotallyDefeated(defeatedPlayer))	//Note:  This check must be performed AFTER the actual state of the game has changed
			{
				fallenPlayers.push_back(defeatedPlayer);
				if (state.getNumberOccupiedBy(whoseTurn)==state.getNumRegions())
					return fallenPlayers;	//no need to wait for the player to say they're done
			}
		}
    }
    return fallenPlayers;
}

/* Gives the current player a chance to fortify by moving troops around */
void Game::fortify(int whoseTurn)
{
    //get player input, then prepare vectors needed to ensure valid moves
    std::vector<std::tuple<int, int, int>> moves = player[whoseTurn]->fortify(state);
    int numberOfRegions = map->getNumberOfRegions();
	std::vector<int> leaving(numberOfRegions,0);	//tracks how many troops have marched out of a region.  Cannot exceed the number that started there, and must leave one behind;
	std::vector<int> arriving(numberOfRegions,0);	//tracks how many troops have marched into a region.  Allows the state to be updated in the end without confusing the number availiable to move

    //load adjustmentsToMake, asserting move validity along the way
    for (int i = 0; i < moves.size(); i++)
    {
        int regionFrom = std::get<0>(moves.at(i));
        int regionTo = std::get<1>(moves.at(i));
        int numTroopsRequested = std::get<2>(moves.at(i));
		std::pair<int,int> infoFrom = state.getRegionInfo(regionFrom);
		std::pair<int,int> infoTo = state.getRegionInfo(regionTo);
		//assert validity, and adjust leaving and arriving if valid
		if (infoFrom.first==whoseTurn && infoTo.first==whoseTurn && map->areConnected(regionTo,regionFrom))
		{
			int maxNumTroops = infoFrom.second-leaving[regionFrom];
			if (arriving[regionFrom]<1)
				maxNumTroops--;		//someone has to stay and guard the region
			if (numTroopsRequested <= maxNumTroops)
			{
				leaving[regionFrom] += numTroopsRequested;
				arriving[regionTo] += numTroopsRequested;
			}
		}
    }

    //move the troops
    for (int i = 0; i < numberOfRegions; i++)
    {
		int change = arriving[i] - leaving[i];
		if (change != 0)
		{
			std::pair<int, int> regionInfo = state.getRegionInfo(i);
			regionInfo.second += change;
			state.setRegionInfo(i, regionInfo);
		}
    }
}

/* Rolls dice (numToAttackWith vs numToDefendWith), and returns a number representing the outcome.
 * Meaning of 2,1,0,-1,-2: attacker won 2, attacker won 1, each won one, defender won 1, defender won 2 */
int Game::rollToConquer(int numToAttackWith, int numToDefendWith)
{
    std::uniform_real_distribution<double> distrubution(0,1);
	double randResult = distrubution(rng);
	std::pair<double,double> probabilities = getProbability(numToAttackWith, numToDefendWith);
	
	//determine the return value for the state.  See "dice probability notes.txt" for calculation steps
	if (numToAttackWith==1 || numToDefendWith==1)
		return (randResult < probabilities.first) ? 1 : -1;
	else
		return (randResult < probabilities.first) ? 2 : ((randResult-probabilities.first < probabilities.second) ? 0 : -2);
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

/* See the "dice probability notes.txt" doc for the calculations behind this. */
std::pair<double,double> Game::getProbability(int attackingDice, int defendingDice)
{
	if (defendingDice == 1)
    {
        if (attackingDice == 1)
            return std::pair<double,double>(15.0/36.0, 0);
        else if (attackingDice == 2)
			return std::pair<double,double>(125.0/216.0, 0);
        else
			return std::pair<double,double>(853.0/1296.0, 0);
    }
    else
    {
        if (attackingDice == 1)
			return std::pair<double,double>(55.0/216.0, 0);
        else if (attackingDice == 2)
			return std::pair<double,double>(295.0/1296.0, 420.0/1296.0);
        else
			return std::pair<double,double>(2890.0/7776.0, 2611.0/7776.0);
    }
}

double Game::getProbabilityOfVictory(int attacking, int defending) {
////	for (int i=0; i<20; i++) {
////		for (int j=0; j<20; j++)
////			std::cout<< (Game::probabilityCacheSet[i][j] ? "T " : "F ");
////		std::cout<<std::endl;
////	}
////	std::cout<<"CLEAR"<<std::endl;

	//Recursive method
	if (attacking==0)
		return 0;
	if (defending==0)
		return 1.0;
	std::pair<double,double> probabilities = getProbability(attacking,defending);
	if (attacking==1 || defending==1)
		return probabilities.first*getProbabilityOfVictory(attacking, defending-1)
			+ (1-probabilities.first)*getProbabilityOfVictory(attacking-1, defending);
	else
		return probabilities.first*getProbabilityOfVictory(attacking, defending-2)
			+ probabilities.second*getProbabilityOfVictory(attacking-1, defending-1)
			+ (1-probabilities.first-probabilities.second)*getProbabilityOfVictory(attacking-2, defending);
}
