#include "Game.hpp"
//#include "location\Earth.hpp"
#include "location\Island.hpp"
#include <iostream>
#include <random>
#include <ctime>

// yep... it's a global variable.  But it makes sense here.
std::default_random_engine rng;					

int main(int argc, char** argv) {
	//We must seed the random number generator.  This one is only used for testing; rng is used more.
	srand((unsigned int)time(NULL));		
	rng = std::default_random_engine(rand());
	
	Game game(new Island());
	//game.addPlayer(new BadStrategy());
	//game.addPlayer(new BadStrategy());
	game.addPlayer(new HumanControlledStrategy());
	game.addPlayer(new HumanControlledStrategy());
	std::cout << "Ready to run." << std::endl;
	game.runGame();
	//GameReport report = game.getReport();
	std::cout << "All done." << std::endl;
}
