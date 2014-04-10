#include "Game.hpp"
#include "location\Earth.hpp"
#include <iostream>
#include <random>
#include <ctime>

std::default_random_engine rng;					//yep... it's a global variable.  But it makes sense here.

int main(int argc, char** argv) {
	srand((unsigned int)time(NULL));		//We must seed the random number generator.  This one is only used for testing; rng is used more.
	rng = std::default_random_engine(rand());
	
	Game game(new Earth());
	game.addPlayer(new HumanControlledStrategy());
	game.addPlayer(new HumanControlledStrategy());
	std::cout << "Ready to run." << std::endl;
	game.runGame();
	//GameReport report = game.getReport();
	std::cout << "All done." << std::endl;
}
