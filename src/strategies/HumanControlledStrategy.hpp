#ifndef HUMANSTRATEGY_HPP
#define	HUMANSTRATEGY_HPP

#include "../Strategy.hpp"
#include    "../gamemap/GameMap.hpp"

class HumanControlledStrategy : public Strategy
{
public:
	HumanControlledStrategy();
	virtual int claim(GameState state);
	std::vector<std::pair<int,int>> place(GameState state, int numTroops);
	std::pair<int,int> attack(GameState state);
	bool defend(GameState state, int countryAttacked, int countryAttacking);
	std::vector<std::tuple<int,int,int> > fortify(GameState state);
private:
	int movesDoneAttacking;
};

#endif	/* HUMANSTRATEGY_HPP */
