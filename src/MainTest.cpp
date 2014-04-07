#include "Game.hpp"
#include <iostream>

#include <stdlib.h>

std::default_random_engine rng;					//yep... it's a global variable.  But it makes sense here.

int main(int argc, char** argv) {
	srand((unsigned int)time(NULL));		//We must seed the random number generator.  This one is only used for testing; rng is used more.
	rng = std::default_random_engine(rand());
	
	Strategy s1 = ManualStrategy();
	Strategy s2 = ManualStrategy();
	Strategy s3 = ManualStrategy();
	Game game = Game(Earth);
	game.addPlayer(&s1);
	game.addPlayer(&s2);
	game.addPlayer(&s3);
	game.runGame();
	GameReport report = game.getReport();

	std::cout << "I am running." << std::endl;
}
