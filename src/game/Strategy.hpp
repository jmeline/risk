/*
 * =====================================================================================
 *       Filename:  Strategy.hpp
 *    Description: Defines the interface for a strategy
 *
 *    Contains:
		1) An enum of all types of Strategies, by which they can be identified
		2) The definintion of a Strategy: an abstract interface to be implemented
		3) The definitions of the types of Strategy
 * =====================================================================================
 */

#ifndef  strategy_INC
#define  strategy_INC

#include "GameState.hpp"
#include "../gamemap/GameMap.hpp"
#include <vector>
#include <utility>
#include <tuple>



namespace StrategyEnum {
	enum StrategyEnum
	{
		NOPLAYER,		//used as a placeholder to indicate that a given player isn't in the game (ie, if there are only 3 players, p4-p6 can be said to be running StrategyEnum::NOPLAYER.  There is no actual object called "NOPLAYER".
		HumanControlledStrategy,
		ObtainSmallestContinentsFirstStrategy,
		PacifistStrategy,
		ImmediateBestValueStrategy1,
		BadStrategy1
	};
}




/*
 * =====================================================================================
 *        Class:  Strategy
 *  Description:  An interface that must be implemented to constitute a strategy
 * =====================================================================================
 */

class Strategy {
protected:
	GameMap* map;
	int myPlayerNumber;
	bool beVerbose;

public:

	/* a factoryMethod */
	static Strategy* make(StrategyEnum::StrategyEnum strategyType);

	/* Called once to let the strategy understand the relationship between countries and Contenents.
	 * To be already implemented, but can be overridden if desired. */
	virtual void init(GameMap* m, int num);
	virtual void init(GameMap* m, int num, bool verbose);

	virtual StrategyEnum::StrategyEnum getIdentifier() = 0;		/* uniquely identifies the specific Strategy */

	/* Returns the region to claim */
	virtual int claim(GameState state) = 0;
	
	/* Returns a list of pairs representing country placement:
	 * The first int in the each pair is the region where to place
	 * The second int in each pair is the number of troops to place there */
	virtual std::vector<std::pair<int,int>> place(GameState state, int numTroops) = 0;

	/* Returns a pair representing an attack:
	 * The first int in the pair is the country doing the attacking
	 * The second int in the pair is the country being attacked */
	virtual std::pair<int,int> attack(GameState state) = 0;

	/* Returns a vector of triplets, each representing the movement of troops for fortification:
	 * The first int of each triplet is the country moving from
	 * The second int of each triplet is the country moving to
	 * The third int of each triplet is the number of troops to move */
	virtual std::vector<std::tuple<int,int,int>> fortify(GameState state) = 0;
};

/*
 * BELOW HERE are the headers for all the Strategy types
*/

/* For testing only.  Is the only interactive strategy: gets human input for all nontrivial decisions. */
class HumanControlledStrategy : public Strategy
{
public:
	HumanControlledStrategy();
	StrategyEnum::StrategyEnum getIdentifier() { return StrategyEnum::HumanControlledStrategy; }
	virtual int claim(GameState state);
	virtual std::vector<std::pair<int,int>> place(GameState state, int numTroops);
	virtual std::pair<int,int> attack(GameState state);
	virtual std::vector<std::tuple<int,int,int> > fortify(GameState state);
};

/*
  *	This strategy seeks to claim the two smallest continents first: Australia and South America
  * The reasoning behind this strategy is that by having the smaller continents claimed, you
  * can defend more easily and have easy access to North America, Africa, and Asia to prevent
  * the opposing player from claiming them. 
  */
class ObtainSmallestContinentsFirstStrategy : public Strategy
{
public:
	ObtainSmallestContinentsFirstStrategy();
	virtual int claim(GameState state);	
	StrategyEnum::StrategyEnum getIdentifier() { return StrategyEnum::ObtainSmallestContinentsFirstStrategy; }
	virtual std::vector<std::pair<int,int>> place(GameState state, int numTroops);
	virtual std::pair<int,int> attack(GameState state);
	virtual std::vector<std::tuple<int,int,int> > fortify(GameState state);
private:
	double attackPlacementPreferenceFactor;
};


/*
  *	A greedy method: maximizes a "value" function that considers troop bonus, exposed borders, and versatility (neighboring region counts)
  */
class ImmediateBestValueStrategy1 : public Strategy
{
public:
	ImmediateBestValueStrategy1();
	virtual int claim(GameState state);	
	StrategyEnum::StrategyEnum getIdentifier() { return StrategyEnum::ObtainSmallestContinentsFirstStrategy; }
	virtual std::vector<std::pair<int,int>> place(GameState state, int numTroops);
	virtual std::pair<int,int> attack(GameState state);
	virtual std::vector<std::tuple<int,int,int> > fortify(GameState state);
private:
	double dangerThreshold;
	double stabilityFactor;
	double versatilityFactor;
	std::vector<bool> getBoolOwnershipList(GameState state);
	double getValue(GameState state);
	double testValue(GameState state, int regionToOwn);
};


/* DISCRIPTION HERE */
class BadStrategy1 : public Strategy
{
public:
	BadStrategy1();
	StrategyEnum::StrategyEnum getIdentifier() { return StrategyEnum::BadStrategy1; }
	virtual int claim(GameState state);
	virtual std::vector<std::pair<int,int>> place(GameState state, int numTroops);
	virtual std::pair<int,int> attack(GameState state);
	virtual std::vector<std::tuple<int,int,int> > fortify(GameState state);
private:
	int movesDoneAttacking;
};


/* Never attacks.  Handles other things more or less at random.
  WARNING: this strategy MAY NOT TERMINATE; in any case it will never win. */
class PacifistStrategy : public Strategy
{
	PacifistStrategy();
	StrategyEnum::StrategyEnum getIdentifier() { return StrategyEnum::PacifistStrategy; }
	virtual int claim(GameState state);
	virtual std::vector<std::pair<int,int>> place(GameState state, int numTroops);
	virtual std::pair<int,int> attack(GameState state);
	virtual std::vector<std::tuple<int,int,int> > fortify(GameState state);
private:
	int movesDoneAttacking;
};

#endif   /* ----- #ifndef strategy_INC  ----- */
