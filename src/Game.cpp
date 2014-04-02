/*
 * =====================================================================================
 *       Filename:  Game.hpp
 *    Description: Manages the flow of a game
 * =====================================================================================
 */

#include "Game.hpp"
#include <iostream>

#ifdef  game_INC	//never hurts to be cautious

extern std::default_random_engine rng;

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
	while (true) {	//we will keep going until the player's choice is a "magic value" indicating to ned the turn
		std::pair<int,int> attackInfo = player[whoseTurn]->attack(state);
		int attackTo = attackInfo.first;
		int attackFrom = attackInfo.second;
		if (attackFrom<0 || attackTo<0)	//the "magic value" indicating "all done" received
			break;
		std::pair<int,int> attackedRegionInfo = state.getRegionInfo(attackTo);
		std::pair<int,int> attackingRegionInfo = state.getRegionInfo(attackFrom);
		int playerAttacked = attackedRegionInfo.first;
		if (attackingRegionInfo.first==whoseTurn && playerAttacked!=whoseTurn && attackingRegionInfo.second>1) {
			int numToDefendWith = player[playerAttacked]->defend(state,attackTo,attackFrom);
			int numToAttackWith = 3;
			if (attackingRegionInfo.second < 4)
				numToAttackWith = (attackingRegionInfo.second < 3) ? 1 : 2;
			////roll and compare dice.  Meaning of 2,1,0,-1,-2:
				//attacker 2, attacker 1, each one, defender 1, defender 2
			int numberConquered = rollToConquer(numToAttackWith,numToDefendWith);
			if (numberConquered > 0) {
				attackedRegionInfo.second -= numberConquered;
				if (attackedRegionInfo.second == 0) {	//conquered
					if (isTotallyDefeated(attackedRegionInfo.first))
						fallenPlayers.push_back(attackedRegionInfo.first);
					attackedRegionInfo.first = attackingRegionInfo.first;
					attackedRegionInfo.second = numToAttackWith;
				}
			} else if (numberConquered < 0) {
				attackingRegionInfo.second += numberConquered;	//note that numberConquered is a negative number, here
			} else {
				attackedRegionInfo.second -= 1;
				attackingRegionInfo.second -= 1;
				if (attackedRegionInfo.second == 0) {	//conquered
					if (isTotallyDefeated(attackedRegionInfo.first))
						fallenPlayers.push_back(attackedRegionInfo.first);
					attackedRegionInfo.first = attackingRegionInfo.first;
					attackedRegionInfo.second = numToAttackWith-1;
				}
			}
		}
		state.setRegionInfo(attackFrom, attackingRegionInfo);
		state.setRegionInfo(attackTo, attackedRegionInfo);
	}
	return fallenPlayers;
}
	
/* Gives the current player a chance to fortify by moving troops around */
void Game::fortify(int whoseTurn) {
	//get player input, then prepare vectors needed to ensure valid moves
	std::vector<std::tuple<int,int,int>> moves = player[whoseTurn]->fortify(state);
	std::vector<int> adjustmentsToMake;
	std::vector<bool> regionAlreadyMoved;
	for (int i=map.getNumberOfRegions()-1; i>=0; i++) {
		adjustmentsToMake[i] = 0;
		regionAlreadyMoved[i] = false;
	}
	//load adjustmentsToMake, asserting move validity along the way
	for (int i=0; i<moves.size(); i++) {
		int regionFrom = std::get<0>(moves.at(i));
		int regionTo = std::get<1>(moves.at(i));
		int numTroops = std::get<2>(moves.at(i));
		if (!regionAlreadyMoved[regionFrom] && state.getRegionInfo(regionFrom).first==whoseTurn && state.getRegionInfo(regionTo).first==whoseTurn && numTroops<state.getRegionInfo(regionFrom).second) {
			regionAlreadyMoved[regionFrom] = true;
			adjustmentsToMake[regionFrom] -= numTroops;
			adjustmentsToMake[regionTo] += numTroops;
		}
	}
	//move the troops
	for (int i=0; i<adjustmentsToMake.size(); i++) {
		if (adjustmentsToMake[i] == 0)
			continue;
		std::pair<int,int> regionInfo = state.getRegionInfo(i);
		regionInfo.second += adjustmentsToMake[i];
		state.setRegionInfo(i,regionInfo);
	}
}

/* Rolls dice (numToAttackWith vs numToDefendWith), and returns a number representing the outcome.
* Meaning of 2,1,0,-1,-2: attacker won 2, attacker won 1, each won one, defender won 1, defender won 2 */
int Game::rollToConquer(int numToAttackWith, int numToDefendWith) {
	//determine how many possibilities there are, and roll one of those states
	int numOfDice = numToAttackWith+numToDefendWith;
	int numOfPossibilities = 36;
	if (numOfDice>2)
		numOfPossibilities *= ((numOfDice==5) ? 216 : ((numOfDice==4)?36:6));
	std::uniform_int_distribution<int> distribution(0,numOfPossibilities-1);
	int rollResult = distribution(rng);
	//determine the return value for the state.  See "dice probability notes.txt" for calculation steps
	if (numToDefendWith==1) {
		if (numToAttackWith==1) {
			if (rollResult<15)	return 1;	//probability 15/36
			else				return -1;	//probability 21/36
		} else if (numToAttackWith==2) {
			if (rollResult<125)	return 1;	//probability 125/216
			else				return -1;	//probability 91/216
		} else if (numToAttackWith==3) {
			if (rollResult<853)	return 1;	//probability 853/1296
			else				return -1;	//probability 443/1296
		}
	} else if (numToDefendWith==2) {
		if (numToAttackWith==1) {
			if (rollResult<55)	return 1;	//probability 55/216
			else				return -1;	//probability 161/216
		} else if (numToAttackWith==2) {
			if (rollResult<295)				return 2;	//probability 295/1296
			else if (rollResult<(295+420))	return 0;	//probability 420/1296
			else							return -2;	//probability 581/1296
		} else if (numToAttackWith==3) {
			if (rollResult<2890)			return 2;	//probability 2890/7776
			else if (rollResult<(2890+2611)) return 0;	//probability 2611/7776
			else							return -2;	//probability 2275/7776
		}
	}
	return 0;	//here for compiling, but this point should never be reached
}
	
/* Returns true is there are no countries left owned by a given player. */
bool Game::isTotallyDefeated(int playerNumber) {
	for (int i=0; i<map.getNumberOfRegions(); i++) {
		if (state.getRegionInfo(i).first == playerNumber)
			return false;
	}
	return true;
}

#endif //end ifdef