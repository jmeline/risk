/*
 * =====================================================================================
 *       Filename:  Strategy.hpp
 *    Description: Defines the interface for a strategy
 * =====================================================================================
 */

#ifndef  strategy_INC
#define  strategy_INC

#include "GameState.hpp"
#include "gamemap/GameMap.hpp"
#include <vector>
#include <utility>
#include <tuple>

class Strategy {
protected:
	GameMap* map;
	int myPlayerNumber;

public:
	/* Called once to let the strategy understand the relationship between countries and Contenents.
	 * Already implemented, but can be overridden if desired. */
	virtual void init(GameMap* m, int num) {
		map = m;
		myPlayerNumber = num;
	}

	/* Returns the region to claim */
	virtual int claim(GameState state) = 0;
	
	/* Returns a list of pairs representing country placement:
	 * The first int in the each pair is the region where to place
	 * The second int in each pair is the number of troops to place there */
	virtual std::vector<std::pair<int,int> > place(GameState state, int numTroops) = 0;

	/* Returns a pair representing an attack:
	 * The first int in the pair is the country being attacked
	 * The second int in the pair is the country doing the attacking */
	virtual std::pair<int,int> attack(GameState state) = 0;

	/* Returns true if the defender wish to defend with two armies, and false if it wishes to defend with only one */
	virtual bool defend(GameState state, int countryAttacked, int countryAttacking) = 0;

	/* Returns a vector of triplets, each representing the movement of troops for fortification:
	 * The first int of each triplet is the country moving from
	 * The second int of each triplet is the country moving to
	 * The third int of each triplet is the number of troops to move */
	virtual std::vector<std::tuple<int,int,int> > fortify(GameState state) = 0;
};


class HumanControlledStrategy : public Strategy
{
public:
	HumanControlledStrategy();
	virtual int claim(GameState state);
	virtual std::vector<std::pair<int,int>> place(GameState state, int numTroops);
	virtual std::pair<int,int> attack(GameState state);
	virtual bool defend(GameState state, int countryAttacked, int countryAttacking);
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
	virtual std::vector<std::pair<int,int>> place(GameState state, int numTroops);
	virtual std::pair<int,int> attack(GameState state);
	virtual bool defend(GameState state, int countryAttacked, int countryAttacking);
	virtual std::vector<std::tuple<int,int,int> > fortify(GameState state);
private:
	int movesDoneAttacking;
	bool sortByNumberOfRegions();
};

//http://www.cplusplus.com/articles/NhA0RXSz/



class BadStrategy : public Strategy
{
public:
	BadStrategy();
	virtual int claim(GameState state);
	virtual std::vector<std::pair<int,int>> place(GameState state, int numTroops);
	virtual std::pair<int,int> attack(GameState state);
	virtual bool defend(GameState state, int countryAttacked, int countryAttacking);
	virtual std::vector<std::tuple<int,int,int> > fortify(GameState state);
private:
	int movesDoneAttacking;
};

#endif   /* ----- #ifndef strategy_INC  ----- */
