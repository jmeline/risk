#include "../game/Strategy.hpp"
#include "../game/Game.hpp"
#include <vector>
#include <iostream>
#include <algorithm>	// std::sort
#include <utility>		//std::pair
#include <string>

/**************************************************

            COMPUTER CONTROLLED
    Immediate Best Value Strategy 1
	A greedy method: maximizes a "value" function that considers troop bonus, exposed borders, and versatility (neighboring region counts).
    Value function: V=x1*T/B+x2*N
		T=troop bonus (region and continent); B=exposed borders; N=neighboring regions; x1 and x2 are scaling factors for stability vs having expansion options

Rules for claiming regions:
	-Just greedily maximize V
	-Among equally valued locations, pick at "random" (first is good enough)

Rules for placing:
	1) Makes sure all regions have a minimum of two troops
	2) Get a list of all exposed borders, sorted by Danger = number of enemies around - number of troops
	3) Make sure all exposed borders above a certain "danger threshold" are served enough to reach the threshold
	4) If there are still troops:
		a) Get a list of neighboring enemies
		b) Sort by the (amount by which V would increase if it were conquered) * (probability of victory if all remaining troops were placed to attack it)
		c) Place to attack the best of those neighboring enemies, as determined by the sort

Rules for attacking:
	1) Get a list of neighboring enemies
	2) Sort by the (amount by which V would increase if it were conquered) * (probability of victory if attack from strongest place)
	3) Go through the list, attacking as soon as a candidate is found that can attack without leaving the player with a Danger = (number of enemies around - number of troops) above the "danger threshold".  If none found, don't attack.
		a) If none of the owned regions touching the target is above the danger threshold, no attack.

Rules for fortifying:
	-If a region is surrounded only by friendly regions, move all but two of it's troops into a random one of those regions (eventually getting to the battlefront).

NOTE: "danger threshold", x1=stabilityFactor, and x2=versatilityFactor should be kept constant, but be adjustable by child classes for cheap variety.

**************************************************/


ImmediateBestValueStrategy1::ImmediateBestValueStrategy1() {
	dangerThreshold = 2;
	stabilityFactor = 4.0;
	versatilityFactor = 2.0;
}


int ImmediateBestValueStrategy1::claim(GameState state)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
	if (beVerbose)  std::cout << "ImmediateBestValueStrategy1 "<<myPlayerNumber<<" is claiming his land" << std::endl;
	if (beVerbose)  state.display();

	std::vector<std::pair<int,double>> choices;
	for (int i=0; i<state.getNumRegions(); i++)
	{
		if (state.getRegionInfo(i).first < 0)
			choices.push_back(std::pair<int,double>(i,testValue(state, i)));
	}
	std::sort(choices.begin(), choices.end(),
			[](const std::pair<int, double> &lhs, const std::pair<int, double> &rhs)
				{ return lhs.second < rhs.second; });
	int pickedRegion = choices[choices.size()-1].first;

    if (beVerbose)  std::cout << "Chose: " << pickedRegion << std::endl;
	if (beVerbose)  std::cout << "________________________________" << std::endl;

    // return the available country with the lowest region count
    return pickedRegion;
}


std::vector<std::pair<int, int>> ImmediateBestValueStrategy1::place(GameState state, int numTroops)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
    if (beVerbose)  std::cout << "ImmediateBestValueStrategy1 " << myPlayerNumber << " is placing " << numTroops << " troops." << std::endl;
	if (beVerbose)  state.display();

	std::vector<std::pair<int, int>> actions;

	//First, make sure there are no regions with only one troop
	std::vector<int> myRegions = state.getRegionsOwnedByPlayer(myPlayerNumber);
	for (int i=0; i<myRegions.size() && numTroops>0; i++)
	{
		if (state.getRegionInfo(myRegions[i]).second == 1)
		{
			if (beVerbose)  std::cout<<"Placing a troop at "<<myRegions[i]<<" to meet the minimum."<<std::endl;
			actions.push_back(std::pair<int,int>(myRegions[i],1));
			numTroops--;
		}
	}

	//Now, get a list of all exposed borders, sorted by danger, and give them troops, if needed
	if (numTroops>0)
	{
		std::vector<std::pair<int,int>> borders = state.getAllExposedBorders(myPlayerNumber, map);
		//border's second index is the number of enemy troops; change it to the amount of danger
		for (int i=0; i<borders.size(); i++)
		{
			int danger = borders[i].second - state.getRegionInfo(borders[i].first).second;
			borders[i].second = danger;
		}
		std::sort(borders.begin(), borders.end(),
			[](const std::pair<int, int> &lhs, const std::pair<int, int> &rhs)
				{ return lhs.second < rhs.second; });
		for (int i=borders.size()-1; i>=0 && numTroops>0; i--)
		{
			if (borders[i].second > dangerThreshold)
			{
				int troopsToSend = borders[i].second - dangerThreshold;
				if (troopsToSend > numTroops)
					troopsToSend = numTroops;
				if (beVerbose)  std::cout<<"Placing "<<troopsToSend<<" troops at "<<borders[i].first<<" to get down to the danger threshold."<<std::endl;
				actions.push_back(std::pair<int,int>(borders[i].first,troopsToSend));
				numTroops -= troopsToSend;
			}
		}
	}

	//if we still have troops, put them all in the border region that would score the highest points
	if (numTroops > 0) {
		//get a list of ideal targets
		std::vector<std::pair<int,double>> targets;
		std::vector<int> options = state.getAllNeighborsOfPlayer(myPlayerNumber, map);
		for (int i=0; i<options.size(); i++)
			targets.push_back(std::pair<int,double>(options[i], testValue(state, options[i])));
		std::sort(targets.begin(), targets.end(),
			[](const std::pair<int, double> &lhs, const std::pair<int, double> &rhs)
				{ return lhs.second < rhs.second; });
		//and pick a neighboring spot to strengthen
		std::vector<int> theirNeighbors = map->getNeighborsOfRegion(targets[targets.size()-1].first);
		for (int i=0; i<theirNeighbors.size(); i++)
		{
			if (state.getRegionInfo(theirNeighbors[i]).first == myPlayerNumber)
			{
				if (beVerbose)  std::cout<<"Placing "<<numTroops<<" troop at "<<theirNeighbors[i]<<" to prepare to attack "<<targets[targets.size()-1].first<<std::endl;
				actions.push_back(std::pair<int,int>(theirNeighbors[i],numTroops));
				break;
			}
		}
	}

	if (beVerbose)  std::cout << "________________________________" << std::endl;
	return actions;
}


std::pair<int, int> ImmediateBestValueStrategy1::attack(GameState state)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
    if (beVerbose)  std::cout << "ImmediateBestValueStrategy1 "<<myPlayerNumber<<" is attacking" << std::endl;
	if (beVerbose)  state.display();

	//sort neighbors by weight=valueOfVictory*probabilyOfVictory
	std::vector<std::tuple<int,int,double>> weightedNeighborList;	//first int is whereTo, second is whereFrom; double is the weight
	std::vector<int> myNeighbors = state.getAllNeighborsOfPlayer(myPlayerNumber, map);
	for (int i=0; i<myNeighbors.size(); i++)
	{
		double value = testValue(state,myNeighbors[i]);
		//we have the value, now we need the probability at the best attack-from location
		double probability = 0;
		int bestAttackFromSpot = -1;
		std::vector<int> theirNeighbors = map->getNeighborsOfRegion(myNeighbors[i]);
		for (int j=0; j<theirNeighbors.size(); j++)
		{
			if (state.getRegionInfo(theirNeighbors[j]).first == myPlayerNumber)
			{
				double currentProbability = Game::getProbabilityOfVictory(state.getRegionInfo(theirNeighbors[j]).second - 1, state.getRegionInfo(myNeighbors[i]).second);
				if (currentProbability>probability)
				{
					probability = currentProbability;
					bestAttackFromSpot = theirNeighbors[j];
				}
			}
		}
		if (beVerbose)  std::cout<<"Region "<<myNeighbors[i]<<" has weight "<<probability*value<<" when attacked from "<<bestAttackFromSpot<<std::endl;
		weightedNeighborList.push_back(std::tuple<int,int,double>(myNeighbors[i],bestAttackFromSpot,probability*value));
	}
	std::sort(weightedNeighborList.begin(), weightedNeighborList.end(),
              [](const std::tuple<int,int,double> &lhs, const std::tuple<int,int,double> &rhs)
					{ return std::get<2>(lhs) < std::get<2>(rhs); });

	//Now, we pick one to attack
	int chosen = weightedNeighborList.size() - 1;
	while (chosen >= 0)
	{
		if (beVerbose)  std::cout<<"Considering attacking "<<std::get<0>(weightedNeighborList[chosen])<<std::endl;
		int attackFrom = std::get<1>(weightedNeighborList[chosen]);
		int danger = 0 - state.getRegionInfo(attackFrom).second;
		std::vector<int> localNeighbors = map->getNeighborsOfRegion(attackFrom);
		for (int k=0; k<localNeighbors.size(); k++)
		{
			if (state.getRegionInfo(localNeighbors[k]).first != myPlayerNumber)
				danger += state.getRegionInfo(localNeighbors[k]).second;
		}
		if (danger < dangerThreshold)
			break;
		chosen--;
	}

	//And attack it
	if (chosen >= 0)
	{
		int attackTo = std::get<0>(weightedNeighborList[chosen]);
		int attackFrom = std::get<1>(weightedNeighborList[chosen]);
		if (beVerbose)  std::cout << "Attacking " << attackTo << " with " << attackFrom << std::endl;
		if (beVerbose)  std::cout << "________________________________" << std::endl;
		return std::pair<int,int>(attackFrom,attackTo);
	}
	else
	{
		if (beVerbose)  std::cout<<"Not attacking."<<std::endl;
		if (beVerbose)  std::cout << "________________________________" << std::endl;
		return std::pair<int,int>(-1,-1);
	}
}


std::vector<std::tuple<int, int, int>> ImmediateBestValueStrategy1::fortify(GameState state)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
	if (beVerbose)  std::cout << "ImmediateBestValueStrategy1 "<<myPlayerNumber<<" is fortifying" << std::endl;
    if (beVerbose)  state.display();

	std::vector<std::tuple<int, int, int>> actions;

	std::vector<int> myRegions = state.getRegionsOwnedByPlayer(myPlayerNumber);
	for (int i=0; i<myRegions.size(); i++)
	{
		std::vector<int> neighbors = map->getNeighborsOfRegion(myRegions[i]);
		bool isOnBattleFront = false;
		for (int j=0; j<neighbors.size(); j++)
		{
			if (state.getRegionInfo(neighbors[j]).first != myPlayerNumber)
			{
				isOnBattleFront = true;
				break;
			}
		}
		if (!isOnBattleFront)
		{
			//move it, randomly, hoping to find the battlefront
			int whereFrom = myRegions[i];
			int whereTo = neighbors[rand() % neighbors.size()];
			int howManyToMove = state.getRegionInfo(whereFrom).second - 2;
			if (howManyToMove > 0)
			{
				if (beVerbose)  std::cout<<"Moving "<<howManyToMove<<" from "<<whereFrom<<" to "<<whereTo<<std::endl;
				actions.push_back(std::tuple<int,int,int>(whereFrom,whereTo,howManyToMove));
			}
		}
	}

	if (beVerbose) { if (actions.size()==0) std::cout<<"Not moving anyone"<<std::endl; }
	if (beVerbose)  std::cout << "________________________________" << std::endl;
	return actions;
}


double ImmediateBestValueStrategy1::getValue(GameState state)
{
	// Find the troopBoost
	double troopBoost = state.getNumberOccupiedBy(myPlayerNumber) / 3.0;
	std::vector<Continent> continentList = map->getContinentList();
	for (int i=0; i<continentList.size(); i++)
	{
		std::vector<std::pair<int,std::string>> regionList = continentList[i].getRegionList();
		int j = 0;
		while (j<regionList.size())
		{
			if (state.getRegionInfo(regionList[i].first).first != myPlayerNumber)
				break;
			j++;
		}
		if (j==regionList.size())
			troopBoost += continentList[i].getValue();
	}

	//find the number of exposed borders, and the number of neighbors
	int borders = state.getAllExposedBorders(myPlayerNumber, map).size();
	int neighborCount = state.getAllNeighborsOfPlayer(myPlayerNumber, map).size();

	//Now compute the value
	return stabilityFactor*troopBoost/borders + versatilityFactor*neighborCount;
}


double ImmediateBestValueStrategy1::testValue(GameState state, int regionToOwn)
{
	//this state is a copy, so it's safe to alter.  It doesn't matter how many troops we place; we just change ownership
	state.setRegionInfo(regionToOwn, std::pair<int,int>(myPlayerNumber,1));
	return getValue(state);
}
