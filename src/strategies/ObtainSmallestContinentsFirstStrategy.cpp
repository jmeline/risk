#include "../game/Strategy.hpp"
#include <vector>
#include <iostream>
#include <algorithm>	// std::sort
#include <utility>		//std::pair
#include <string>

/**************************************************

            COMPUTER CONTROLLED
    ObtainSmallestContinentsFirstStrategy
    Seeks to get the smallest Continent first

 **************************************************/

/*  GOAL: Own the smallest continents (to which you have access), to quickly get troop bonuses.

Rules for claiming regions:
	-Go for continents in order of decreasing size, claiming a "random" region if availiable (in-order works fine for "random").

Rules for placing:
	1) Get a list of all OWNED continents (with "r" total regions)
	2) Get the smallest non-owned accessible continent.  This is where we want to conquer.  Let there be "R" unowned regions there
	3) The number of troops sent to be used to attack is t*F*R/(r+R), where F is the a "favoring factor".  They are distributed uniformly around unconquered regions.
	4) All other troops fortify owned continents.  Get a list of y regions in the co:ntinents bordering hostile territory, and sort by the number of touching enemy troops.  Then serve round-robin.

Rules for attacking:
	1) A region with <=2 troops will not attack.
	2) Vie for the smallest non-owned continent
		a) Get a list of all owned regions touching that continent's regions.
		b) For each, if there are f friently troops and e enemy troops, calculate the diffrence D=(f-e).  Subtract 3 if the attack-from location is a part of an owned continent.
		c) Attack from the location with the highest D so long as (f>2 if in an owned continent; f>6 otherwise).
	3) Once there is no one able to attack that continent, repeat with the next smallest unowned region, etc.

Rules for fortifying:
	-If a region is surrounded only by friendly regions, move it's troops into a random one of those regions (eventually getting to the battlefront).

NOTE: attackFavoringFactor should be kept constant, but is adjustable by child classes for cheap variety.
*/


ObtainSmallestContinentsFirstStrategy::ObtainSmallestContinentsFirstStrategy() {}


int ObtainSmallestContinentsFirstStrategy::claim(GameState state)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
	if (beVerbose)  std::cout << "ObtainSmallestContinentsFirstStrategy "<<myPlayerNumber<<" is claiming his land" << std::endl;
	if (beVerbose)  state.display();

    // get list of continents
    std::vector<Continent> continentList = this->map->getContinentList();

    // sort the list from the lowest region count to the largest region count
    // std::vector<index> == index from lowest region count to largest region count
    // (1) Continent index
    // (2) region count
    std::vector<std::pair<int, int> > lowRegioncountList;
    if (beVerbose)  std::cout << "continentList.size(): " << continentList.size() << std::endl;
    for (int i = 0; i < continentList.size(); i++)
    {
        lowRegioncountList.push_back(std::make_pair(i, continentList[i].getRegionList().size()));
    }

    // sort lowRegioncountList by the second element in the pair: region count. End result is a sorted list of the smallest number of regions 
    // to the largest number of regions. its original index is preserved as established in the map->continentList
    //std::sort(lowRegioncountList.begin(), lowRegioncountList.end(), sortByNumberOfRegions);
    std::sort(lowRegioncountList.begin(), lowRegioncountList.end(),
              [](const std::pair<int, int> &lhs, const std::pair<int, int> &rhs)
              {
                  return lhs.second < rhs.second; });

    // select the first continent unless it is already taken.
    // If the continent has been taken, grab the continent with the next smallest region count
	int index = 0;
    std::vector<std::pair<int, std::string>> regionList;
	int pickedContinent = -1;
    int pickedRegion = -1;
	bool placeSelected = false;
    while (!placeSelected && index < lowRegioncountList.size())
    {
        // grab the index of the continent with the lowest region count
        // countries with the lowest region count have been sorted from lowest to highest
        pickedContinent = lowRegioncountList[index].first;
        regionList = continentList[pickedContinent].getRegionList();

        int regionIndex = 0;

        // loop through each region within the continent for availability
        while (regionIndex < regionList.size())
        {
            pickedRegion = regionList[regionIndex].first;
            // the region is already claimed
            if (state.getRegionInfo(pickedRegion).first == -1)
            {
                // select this continent (Found a winner)
                placeSelected = true;
                break;
            }

            regionIndex++;
        }

        if (placeSelected)
            break;

        index++;
    }

    if (beVerbose)  std::cout << "Chose: " << pickedRegion << std::endl;
	if (beVerbose)  std::cout << "________________________________" << std::endl;

    // return the available country with the lowest region count
    return pickedRegion;
}


std::vector<std::pair<int, int>> ObtainSmallestContinentsFirstStrategy::place(GameState state, int numTroops)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
    if (beVerbose)  std::cout << "ObtainSmallestContinentsFirstStrategy " << myPlayerNumber << " is placing " << numTroops << " troops." << std::endl;
	if (beVerbose)  state.display();

	std::vector<std::pair<int, int>> actions;

	// get lists for the two types of continents we care about
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
	if (beVerbose)  std::cout<<"Continents I own: "<<continentsIOwn.size() << std::endl;
	
	//get all friendly regions on the battlefront
	int regionCountInOwnedContinents = 0;
	std::vector<std::pair<int, int>> frontlineRegions;		//first int is the region's index, the second is its "score"
	for (int i = 0; i < continentsIOwn.size(); i++)
    {
		std::vector<std::pair<int, std::string>> tempRegionList = continentList[i].getRegionList();
		regionCountInOwnedContinents += tempRegionList.size();
		bool iOwnThisContinent = true;
		for (int j=0; j<tempRegionList.size(); j++)
		{
			int regionBeingConsidered = tempRegionList[j].first;
			std::vector<int> neighbors = map->getNeighborsOfRegion(regionBeingConsidered);
			int enemyCount = 0;
			for (int k=0; k<neighbors.size(); k++)
			{
				if (state.getRegionInfo(neighbors[k]).first != myPlayerNumber)
					enemyCount += state.getRegionInfo(neighbors[k]).second;
			}
			if (enemyCount>0)
			{
				int score = state.getRegionInfo(regionBeingConsidered).second - enemyCount;
				frontlineRegions.push_back(std::pair<int,int>(regionBeingConsidered, score));
			}
		}
    }
	std::sort(frontlineRegions.begin(), frontlineRegions.end(),
              [](const std::pair<int, int> &lhs, const std::pair<int, int> &rhs)
					{ return lhs.second < rhs.second; });
	if (beVerbose)  std::cout<<"Regions in my continents: "<<regionCountInOwnedContinents<<"; Regions on my frontline: "<<frontlineRegions.size()<<std::endl;
	
	// Get a list of regions where to send attackers, and get the number of hostile regions there
	Continent targetContinent = continentList[continentsToConquer[0].first];
	std::vector<int> attackWorthyLocations;
	int hostileRegions;
	for (int h=0; h<continentsToConquer.size()&&attackWorthyLocations.size()==0; h++)
	{
		targetContinent = continentList[continentsToConquer[h].first];
		if (beVerbose)  std::cout<<"Considering target continent: " << continentsToConquer[h].first << std::endl;
		hostileRegions = 0;
		std::vector<std::pair<int, std::string>> tempRegionList = targetContinent.getRegionList();
		for (int i=0; i<tempRegionList.size(); i++)
		{
			int regionBeingConsidered = tempRegionList[i].first;
			if (state.getRegionInfo(regionBeingConsidered).first != myPlayerNumber)
			{
				if (beVerbose)  std::cout << "Hostile: " << regionBeingConsidered << std::endl;
				hostileRegions++;
				std::vector<int> neighbors = map->getNeighborsOfRegion(regionBeingConsidered);
				for (int j=0; j<neighbors.size(); j++)
				{
					if (state.getRegionInfo(neighbors[j]).first==myPlayerNumber) {
						attackWorthyLocations.push_back(neighbors[j]);
						if (beVerbose)  std::cout << "Attack worthy: " << neighbors[j] << std::endl;
					}
				}
			}
		}
	}
	if (beVerbose)  std::cout<<"In my target continenet:  Hostile regions: "<<hostileRegions<<"; Attack worthy locations: "<<attackWorthyLocations.size()<<std::endl;

	// Decide an appropriate number of troops to each type of location
	int numberOfTroopsToSendToAttack = (int)(numTroops*attackPlacementPreferenceFactor*hostileRegions/(hostileRegions+regionCountInOwnedContinents));
	if (numberOfTroopsToSendToAttack > numTroops || regionCountInOwnedContinents==0)
		numberOfTroopsToSendToAttack = numTroops;
	int numberOfTroopsToSendToDefend = numTroops - numberOfTroopsToSendToAttack;
	if (beVerbose)  std::cout<<"Attacking troops: "<<numberOfTroopsToSendToAttack<<"; Defending troops: "<<numberOfTroopsToSendToDefend<<std::endl;
	
	// Send troops to target continent
	if (numberOfTroopsToSendToAttack > 0)
	{
		int numToSendPerRegion = numberOfTroopsToSendToAttack / attackWorthyLocations.size();
		int remainder = numberOfTroopsToSendToAttack - numToSendPerRegion;
		for (int i=0; i<attackWorthyLocations.size(); i++)
		{
			int howMany = numToSendPerRegion + ((i<remainder)?1:0);
			actions.push_back(std::pair<int,int>(attackWorthyLocations[i], howMany));
			if (beVerbose)  std::cout << "Placing "<<howMany<<" troops at "<<attackWorthyLocations[i] << std::endl;
		}
	}

	// Send troops to frontline defenses
	if (numberOfTroopsToSendToDefend > 0)
	{
		int numToSendPerRegion = numberOfTroopsToSendToDefend / frontlineRegions.size();
		int remainder = numberOfTroopsToSendToDefend - numToSendPerRegion;
		for (int i=0; i<frontlineRegions.size(); i++)
		{
			int howMany = numToSendPerRegion + ((i<remainder)?1:0);
			actions.push_back(std::pair<int,int>(frontlineRegions[i].first, howMany));
			if (beVerbose)  std::cout << "Placing "<<howMany<<" troops at "<<attackWorthyLocations[i] << std::endl;
		}
	}

	if (beVerbose)  std::cout << "________________________________" << std::endl;
	return actions;
}


std::pair<int, int> ObtainSmallestContinentsFirstStrategy::attack(GameState state)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
    if (beVerbose)  std::cout << "ObtainSmallestContinentsFirstStrategy "<<myPlayerNumber<<" is attacking" << std::endl;
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


bool ObtainSmallestContinentsFirstStrategy::defend(GameState state, int countryAttacked, int countryAttacking)
{
    if (state.getRegionInfo(countryAttacked).second >= 2)
        return true;
    else
        return false;
}


std::vector<std::tuple<int, int, int> > ObtainSmallestContinentsFirstStrategy::fortify(GameState state)
{
	if (beVerbose)  std::cout << "________________________________" << std::endl;
	if (beVerbose)  std::cout << "ObtainSmallestContinentsFirstStrategy "<<myPlayerNumber<<" is fortifying" << std::endl;
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
