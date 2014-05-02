#include    "Strategy.hpp"

Strategy* Strategy::make(StrategyEnum::StrategyEnum strategyType)
{
	switch(strategyType)
	{
		case (StrategyEnum::HumanControlledStrategy):
			return (Strategy*)(new HumanControlledStrategy());
		case (StrategyEnum::BadStrategy1):
			return (Strategy*)(new BadStrategy1());
		case (StrategyEnum::ObtainSmallestContinentsFirstStrategy):
			return (Strategy*)(new ObtainSmallestContinentsFirstStrategy());
		case (StrategyEnum::ImmediateBestValueStrategy1):
			return (Strategy*)(new ImmediateBestValueStrategy1());
		default:
			return (Strategy*)(new BadStrategy1());
	}
}

void Strategy::init(GameMap* m, int num) {
	map = m;
	myPlayerNumber = num;
	#ifdef VERBOSE_STRATEGIES
		beVerbose = true;
	#else
		beVerbose = false;
	#endif
}

void Strategy::init(GameMap* m, int num, bool verbose) {
	map = m;
	myPlayerNumber = num;
	beVerbose = verbose;
}
