/*
 * =====================================================================================
 *       Filename:  Game.hpp
 *    Description: Manages the flow of a game
 * =====================================================================================
 */

#include "Game.hpp"
#include <iostream>

Game::Game(gamemap gMap) {
	map = gMap;
	player = new (Strategy*)[6];
	for (int i=0; i<6; i++)
		player[i] = null;
	state = State.createInitialStateFromMap(map);
}

void Game::addPlayer(Strategy* strat) {
	if (numberOfPlayers < 6) {
		player[numberOfPlayers] = strat;
		(player[numberOfPlayers]).init(map, numberOfPlayers);
		numberOfPlayers++;
	}
}

std::vector<int> Game::runGame() {
	claimCountries();
	placeFirstTroops();
	std::vector<int> winnerRanking;
	bool isDead[] = {false, false, false, false, false, false};
	int numberDead = 0;
	int whoseTurn = 0;
	int roundNumber = 0;
	while (true) {
		if (whoseTurn==0)
			roundNumber++;
		if (!isDead[whoseTurn]) {
			getAndPlaceTroops(whoseTurn);
			std::vector<int> killedInConquest = doATurnOfBattles(whoseTurn);
			for (int i=0; i<killedInConquest.size(); i++) {
				numberDead++;
				winnerRanking[numberOfPlayers-numberDead] = killedInConquest[i];
				isDead[killedInConquest[i]] = true;
			}
			if (numberDead==(numberOfPlayers-1)) {
				winnerRanking[0] = whoseTurn;
				break;
			}
			fortify(whoseTurn);
		}
		whoseTurn = (whoseTurn+1) % numberOfPlayers;
	}
	return winnerRanking;
}

void Game::claimCountries() {
	int whoseTurn = 0;
	for (int i=0; i<map.getNumberOfCountries(); i++) {
		int regionToClaim = player[whoseTurn]->claim(state);
		state.setRegionInfo(regionToClaim, std::pair<int,int>(whoseTurn,1));
		whoseTurn = (whoseTurn+1) % numberOfPlayers;
	}
}

void Game::placeFirstTroops() {
	int numberPerPlayer = 50 - 5*numberOfPlayers;
	int piecesAlreadyUsed = map.getNumberOfRegions() / numberOfPlayers;
	int playersWhoPlacedExtra = map.getNumberOfRegions() % numberOfPlayers;
	for (int i=0; i<numberOfPlayers; i++) {
		int numTroops = numberPerPlayer - piecesAlreadyUsed - ((i<playersWhoPlacedExtra)?1:0);
		std::vector<std::pair<int,int>> placementActions = player[i]->place(state,numTroops);
		for (int j=0; j<placementActions.size(); j++) {
			int wherePut = placementActions[j].first;
			int howMany = placementActions[j].second;
			std::pair<int,int> regionInfo = state.getRegionInfo(wherePut);
			if (regionInfo.first==i) {
				regionInfo.second += (howMany<numTroops)?howMany:numTroops;
				numTroops -= howMany;
			}
		}
	}
}

void Game::getAndPlaceTroops(int whoseTurn) {
	//get number of troops
	int numOfTroops = state.getNumberOccupiedBy(whoseTurn) / 3;
	if (numOfTroops < 3)
		numOfTroops = 3;
	std::vector<Contenent> continentList = map.getContinentList();
	for (int i=0; i<continentList.size(); i++) {
		std::vector<int> regionList = continentList[i].getRegionList();
		bool gotIt = true;
		for (int j=0; j<regionList.size(); j++) {
			if (state.getRegionInfo(regionList[j]).first != whoseTurn) {
				gotIt = false;
				break;
			}
		}
		if (gotIt)
			numOfTroops += continentList[i].getTroopValue();
	}
	//place them
	player[whoseTurn]->place(state,numOfTroops);
}

/* Lets the current player do battle as much as desired for a turn
	* Returns a vector of all players defeated during these battles (if any), given in order of death */
std::vector<int> Game::doATurnOfBattles(int whoseTurn) {
	std::vector<int> fallenPlayers;
	int numberOfPlayersElliminated = 0;
	while (true) {
		std::pair<int,int> attackInfo = player[whoseTurn]->attack(state);
		int attackTo = attackInfo.first;
		int attackFrom = attackInfo.second;
		if (attackFrom<0 || attackTo<0)
			break;
		std::pair<int,int> attackedRegionInfo = state.getRegionInfo(attackTo);
		std::pair<int,int> attackingRegionInfo = state.getRegionInfo(attackFrom);
		int playerAttacked = attackedRegionInfo.first;
		if (attackingRegionInfo.first==whoseTurn && playerAttacked!=whoseTurn && attackingRegionInfo.second>1) {
			int numToDefendWith = player[playerAttacked]->defend(state,attackTo,attackFrom);
			int numToAttackWith = 3;
			if (attackingRegionInfo.second < 4)
				numToAttackWith = (attackingRegionInfo.second < 3) ? 1 : 2;
			////roll and compare dice
			////update the two regionInfos
			////write out the two regionInfos
			////if one was killed, check whether they had been the last (player dead)
			////handle occupation
		}
	}
	return fallenPlayers;
}
	
/* Gives the current player a chance to fortify by moving troops around */
void Game::fortify(int whoseTurn) {
	////COMING SOON
}
