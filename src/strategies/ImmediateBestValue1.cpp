#include "../game/Strategy.hpp"
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
	dangerThreshold = 1.0;
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
			choices.push_back(i,testValue(state, i));
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
			int danger = borders[i].second - state.getRegionInfo(borders[i]).second;
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
				if (beVerbose)  std::cout<<"Placing "<<numTroops<<" troop at "<<borders[i]<<" to get down to the danger threshold."<<std::endl;
				actions.push_back(std::pair<int,int>(borders[i],numTroops));
				numTroops -= numTroops;
			}
		}
	}

	//if we still have troops, put them all in the border region that would score the highest points
	if (numTroops > 0) {
		//get a list of ideal targets
		std::vector<std::pair<int,double>> targets;
		std::vector<int> options = state.getAllNeighborsOfPlayer(myPlayerNumber);
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

	// get a list of continents to conquer, and of continents I own
	std::vector<std::pair<int, int>> continentsToConquer;	//first int is the continent's index; the second is its region count
	std::vector<int> continentsIOwn;
	std::vector<Continent> continentList = this->map->getContinentList();
    for (int i = 0; i < continentList.size(); i++)
    {
		bool iOwnThisContinent = true;
		std::vector<std::pair<int, std::string>> regionList = continentList[i].getRegionList();
		for (int j=0; j<regionList.size(); j++)
		{
			if (regionList[j].first != myPlayerNumber) {
				iOwnThisContinent = false;
				break;
			}
		}
		if (iOwnThisContinent)
			continentsIOwn.push_back(i);
		else
			continentsToConquer.push_back(std::make_pair(i, continentList[i].getRegionList().size()));
    }
	std::sort(continentsToConquer.begin(), continentsToConquer.end(),
              [](const std::pair<int, int> &lhs, const std::pair<int, int> &rhs)
					{ return lhs.second < rhs.second; });

	//determine which regions are important to me
	std::vector<int> myImportantRegions;
	for (int i=0; i<continentsIOwn.size(); i++)
	{
		std::vector<std::pair<int,std::string>> tempRegions = continentList[continentsIOwn[i]].getRegionList();
		for (int j=0; j<tempRegions.size(); j++)
			myImportantRegions.push_back(tempRegions[j].first);
	}

	// Get a list of regions where to send attackers, and get the number of hostile regions
	std::vector<std::pair<int,int>> attackWorthyLocations;		//the first int is the region#, and the second is the score
	for (int h=0; h<continentsToConquer.size(); h++)
	{
		Continent targetContinent = continentList[continentsToConquer[h].first];
		if (beVerbose)  std::cout<<"Target Continent: " << continentsToConquer[h].first << std::endl;
		std::vector<std::pair<int, std::string>> tempRegionList = targetContinent.getRegionList();
		for (int i=0; i<tempRegionList.size(); i++)
		{
			if (state.getRegionInfo(tempRegionList[i].first).first != myPlayerNumber)
			{
				int regionTheirs = tempRegionList[i].first;
				if (beVerbose)  std::cout<<"Considering hostile: " << regionTheirs << std::endl;
				std::vector<int> neighborsTheirs = map->getNeighborsOfRegion(regionTheirs);
				for (int j=0; j<neighborsTheirs.size(); j++)
				{
					if (state.getRegionInfo(neighborsTheirs[j]).first==myPlayerNumber)
					{
						int regionMine = neighborsTheirs[j];
						if (beVerbose)  std::cout<<"Considering attack-from region: " << regionMine << std::endl;
						int surroundingEnemies = 0;
						std::vector<int> neighborsMine = map->getNeighborsOfRegion(regionMine);
						for (int k=0; k<neighborsMine.size(); k++)
							surroundingEnemies += state.getRegionInfo(neighborsMine[k]).second;
						int diffrence = state.getRegionInfo(regionMine).second - surroundingEnemies;
						for (int k=0; k<myImportantRegions.size(); k++)
						{
							if (myImportantRegions[i]==regionMine)
							{
								diffrence -= 4;
								break;
							}
						}
						attackWorthyLocations.push_back(std::pair<int,int>(regionMine, diffrence));
					}
				}
			}
		}
		std::sort(attackWorthyLocations.begin(), attackWorthyLocations.end(),
              [](const std::pair<int, int> &lhs, const std::pair<int, int> &rhs)
					{ return lhs.second < rhs.second; });

		//Now attack from the best region that we can
		for (int i=0; i<attackWorthyLocations.size(); i++)
		{
			int troopsAvailiable = state.getRegionInfo(attackWorthyLocations[i].first).second;
			for (int k=0; k<myImportantRegions.size(); k++)
			{
				if (myImportantRegions[k]==attackWorthyLocations[i].first)
				{
					troopsAvailiable -= 4;
					break;
				}
			}
			if (troopsAvailiable>2)
			{
				int attackFrom = attackWorthyLocations[i].first;
				std::vector<int> neighborsMine = map->getNeighborsOfRegion(attackFrom);
				for (int k=0; k<neighborsMine.size(); k++)
				{
					int whereIsMyNeighbor = map->whereIs(neighborsMine[k]);
					if (beVerbose)  std::cout<< "Where is my neighbor: "<<whereIsMyNeighbor<<"; Neighbor Allegance: "<<state.getRegionInfo(neighborsMine[k]).first << std::endl;
					if (state.getRegionInfo(neighborsMine[k]).first != myPlayerNumber && whereIsMyNeighbor==continentsToConquer[h].first)
					{
						int attackTo = neighborsMine[k];
						if (beVerbose)  std::cout << "Attacking " << attackTo << " with " << attackFrom << std::endl;
						if (beVerbose)  std::cout << "________________________________" << std::endl;
						return std::pair<int,int>(attackFrom,attackTo);
					}
				}
			}
		}
	}

	//if we made it this far, there's nowhere good to attack from
	if (beVerbose)  std::cout<<"Not attacking."<<std::endl;
	if (beVerbose)  std::cout << "________________________________" << std::endl;
	return std::pair<int,int>(-1,-1);
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
			int howManyToMove = state.getRegionInfo(whereFrom).second - 1;
			if (howManyToMove > 0)
			{
				if (beVerbose)  std::cout<<"Moving "<<howManyToMove<<" from "<<whereFrom<<" to "<<whereTo<<std::endl;
				actions.push_back(std::tuple<int,int,int>(whereFrom,whereTo,howManyToMove));
			}
		}
	}

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
	int borders = state.getAllExposedBorders(myPlayerNumber, map);
	int neighborCount = state.getAllNeighborsOfPlayer(myPlayerNumber, map);

	//Now compute the value
	return stabilityFactor*troopBoost/borders + versatilityFactor*neighborCount;
}


double ImmediateBestValueStrategy1::testValue(GameState state, int regionToOwn)
{
	//this state is a copy, so it's safe to alter.  It doesn't matter how many troops we place; we just change ownership
	state.setRegionInfo(regionToOwn, std::pair<int,int>(myPlayerNumber,1));
	return getValue(state);
}
