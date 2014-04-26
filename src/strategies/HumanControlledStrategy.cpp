#include "../game/Strategy.hpp"
#include <iostream>

/*************************
 * HUMAN CONTROLLED
 *************************/

HumanControlledStrategy::HumanControlledStrategy() {}

int HumanControlledStrategy::claim(GameState state)
{
	std::cout << std::endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout<<"Player "<<myPlayerNumber<<": Claim a region."<<std::endl;
	state.display();
	int whichRegion = 0;

	bool outOfBounds = false;
	bool alreadyClaimed = false;
	// Validation loop
	do
	{
		outOfBounds = false;
		alreadyClaimed = false;
		std::cout << "Which region would you like to claim? ";
		std::cin >> whichRegion;

		// Set flags (do check)
		outOfBounds = whichRegion < 0 || whichRegion > (state.getNumRegions()-1);
		if(!outOfBounds)
			alreadyClaimed = state.getRegionInfo(whichRegion).first != -1;

		if(outOfBounds)
			std::cout << "You have to claim somewhere ON the map." << std::endl;
		if(alreadyClaimed)
			std::cout << "You have to choose unclaimed territory." << std::endl;
	} while(alreadyClaimed || outOfBounds);

	// I think we checked all the dumb mistakes the user can make
	return whichRegion;
}

std::vector<std::pair<int, int>> HumanControlledStrategy::place(GameState state, int numTroops)
{
	std::vector<std::pair<int,int>> actions;
	int wherePut = 0;
	int howMany = 0;
	bool outOfBounds = false;
	bool notYours = false;
	bool badAmount = false;
	std::cout << std::endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout<<"Player "<<myPlayerNumber<<": Place your troops."<<std::endl;
	state.display();
	do
	{
		outOfBounds = false;
		notYours = false;
		std::cout<<"You have " << numTroops << " to place." << std::endl;
		std::cout << "Where would you like to place troops?";
		std::cin >> wherePut;
		// Error Checking
		outOfBounds = wherePut < 0 || wherePut > (state.getNumRegions()-1);
		if(!outOfBounds)
			notYours = state.getRegionInfo(wherePut).first != myPlayerNumber;

		// Location choice is valid
		if(!outOfBounds && !notYours)
		{
			do
			{
				badAmount = false;
				std::cout << "How many troops will you place here? ";
				std::cin >> howMany;

				badAmount = howMany < 0 || howMany > numTroops;
				if(badAmount)
					std::cout << "You have " << numTroops << " to place.  Choose a valid number." << std::endl;

			}while(badAmount);

			//Should be valid placement, let them do it.
			if(howMany == 0)		// Gonna let 0 work so they can change their mind, but not gonna add it to the action list
				std::cout << "Cute..." << std::endl;
			else
			{
				// THEY CHOSE CORRECTLY YAY!
				actions.push_back(std::pair<int,int>(wherePut,howMany));
				numTroops -= howMany;
			}
		}


		if(outOfBounds)
			std::cout << "That's not on the map." << std::endl;
		if(notYours)
			std::cout << "You have to add them to YOUR territory." << std::endl;
		
	} while(numTroops > 0 || outOfBounds || notYours);
	return actions;
}

std::pair<int, int> HumanControlledStrategy::attack(GameState state)
{
	std::cout << std::endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout<<"Player "<<myPlayerNumber<<": Attack."<<std::endl;
	state.display();

	int fromLocation, toLocation;
	while (true)
	{
		// Get where to attack from, or quit
		bool successfulChoice = false;
		std::cout << "From which country do you want to attack (-1 to stop this phase)? ";
		std::cin >> fromLocation;
		if (fromLocation < 0)	// Check for quit FIRST
			return std::pair<int,int>(-1,-1);	//let the turn be over
		if (fromLocation >= state.getNumRegions())
			std::cout << "Location invalid." << std::endl;
		else if (state.getRegionInfo(fromLocation).first !=myPlayerNumber)
			std::cout << "You must attack from a region you own." << std::endl;
		else if (state.getRegionInfo(fromLocation).second < 2)
			std::cout << "Not enough troops in that region to attack." << std::endl;
		else
			successfulChoice = true;
		
		// Get where to attack (unless the last part was unsuccessful
		if (successfulChoice)
		{
			successfulChoice = false;
			std::cout << "Which country do you want to attack? ";
			std::cin >> toLocation;
			if (toLocation < 0 || toLocation >= state.getNumRegions())
				std::cout << "Location invalid." << std::endl;
			else if (state.getRegionInfo(toLocation).first == myPlayerNumber)
				std::cout << "You can't attack yourself, dummy." << std::endl;
			else if (!map->areConnected(fromLocation,toLocation))
				std::cout << "Those two regions don't actually touch..." << std::endl;
			else
				successfulChoice = true;
		}

		//send in the decision, if successful
		if (successfulChoice)
			return std::pair<int,int>(fromLocation, toLocation);
	}
}

std::vector<std::tuple<int, int, int> > HumanControlledStrategy::fortify(GameState state)
{
	std::cout << std::endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout<<"Player "<<myPlayerNumber<<": Fortify."<<std::endl;
	state.display();
	std::vector<std::tuple<int,int,int>> actions;
	int numberOfRegions = map->getNumberOfRegions();
	std::vector<int> leaving(numberOfRegions,0);	//tracks how many troops have marched out of a region.  Cannot exceed the number that started there, and must leave one behind;
	std::vector<int> arriving(numberOfRegions,0);	//tracks how many troops have marched into a region.  Allows the state to be updated in the end without confusing the number availiable to move
	int fromLocation, toLocation, howManyToMove;
    
	//keep getting input until told to stop
	while (true)
	{
		//get where moving from, or quit
		bool successfulChoice = false;
		std::cout << "From where would you like to move troops (-1 to skip this phase)? ";
			std::cin >> fromLocation;
		if (fromLocation < 0)	// Check for quit FIRST
			return actions;
		if (fromLocation >= numberOfRegions)
			std::cout << "Location invalid." << std::endl;
		else if (state.getRegionInfo(fromLocation).first!=myPlayerNumber)
			std::cout << "You can't move troops you don't own." << std::endl;
		else
			successfulChoice = true;

		//get how many to move, unless already failed
		if (successfulChoice)
		{
			successfulChoice = false;
			int maxNumTroops = state.getRegionInfo(fromLocation).second-leaving[fromLocation];
			if (arriving[fromLocation]<1)
				maxNumTroops--;		//someone has to stay and guard the region
			if (maxNumTroops<=0)
			{
				std::cout << "You've already moved all the troops you can from that region." << std::endl;
			}
			else
			{
				std::cout << "Move how many troops (up to "<<maxNumTroops<<")?";
				std::cin >> howManyToMove;
				if (howManyToMove<=0 && howManyToMove>maxNumTroops)
					std::cout << "Invalid number of troops for this location." << std::endl;
				else
					successfulChoice = true;
			}

		}

		//get who to attack, unless already failed
		if (successfulChoice)
		{
			successfulChoice = false;
			std::cout << "To where would you like to move those troops? ";
			std::cin >> toLocation;
			if (toLocation<0 || toLocation>=numberOfRegions)
				std::cout << "Location invalid." << std::endl;
			else if (state.getRegionInfo(toLocation).first!=myPlayerNumber)
			{
				std::cout << "You can't move troops to un-owned regions." << std::endl;
			}
			else
				successfulChoice = true;
		}

		//finalize current action, unless already failed
		if (successfulChoice)
		{
			std::cout << "Move queued." << std::endl;
			actions.push_back(std::tuple<int,int,int>(fromLocation,toLocation,howManyToMove));
			leaving[fromLocation] += howManyToMove;
			arriving[toLocation] += howManyToMove;
		}
    }
}
