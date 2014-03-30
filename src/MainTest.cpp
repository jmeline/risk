#include "Game.hpp"
#include <iostream>

std::default_random_engine rng;					//yep... it's a global variable.  But it makes sense here.

int main(int argc, char** argv) {
	srand((unsigned int)time(NULL));		//We must seed the random number generator.  This one is only used for testing; rng is used more.
	rng = std::default_random_engine(rand());

	std::cout << "I am running." << std::endl;
}
