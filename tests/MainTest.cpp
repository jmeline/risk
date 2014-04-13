#include "game/Game.hpp"
#include "game/GameTask.hpp"
#include "game/GameState.hpp"
#include "gamemap/GameMap.hpp"
#include "game/GameReport.hpp"
#include <iostream>
#include <random>
#include <ctime>

// yep... it's a global variable.  But it makes sense here.
extern std::default_random_engine rng;

int main(int argc, char** argv) {
	//We must seed the random number generator.  This one is only used for testing; rng is used more.
	srand((unsigned int)time(NULL));		
	rng = std::default_random_engine(rand());
	
	StrategyEnum::StrategyEnum strategies[] = {
		StrategyEnum::HumanControlledStrategy,
		StrategyEnum::HumanControlledStrategy,	////StrategyEnum::ObtainSmallestContinentsFirstStrategy,
		StrategyEnum::NOPLAYER,
		StrategyEnum::NOPLAYER,
		StrategyEnum::NOPLAYER,
		StrategyEnum::NOPLAYER
	};
	GameTask task(MapEnum::Island, strategies);
	Game game(task);
	std::cout << "Ready to run." << std::endl;
	GameReport report = game.runGame();
	std::cout << "DONE.  Report:" << std::endl;
	report.write(&(std::cout));
	std::cout << "All done." << std::endl;
}
 