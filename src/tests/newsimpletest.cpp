#include <stdlib.h>
#include <iostream>

#include "../game/GameReport.hpp"
#include <fstream>
#include <random>
#include <ctime>

std::default_random_engine rng;

void test1() {
    std::cout << "newsimpletest test 1: Outputting some report" << std::endl;
	//prepare a few reports
	StrategyEnum::StrategyEnum players[] = {
		StrategyEnum::HumanControlledStrategy,
		StrategyEnum::HumanControlledStrategy,
		StrategyEnum::NOPLAYER,
		StrategyEnum::NOPLAYER,
		StrategyEnum::NOPLAYER,
		StrategyEnum::NOPLAYER
	};
	int winners[] = { 1, 0, -1,-1,-1,-1 };
	GameReport report1(23,MapEnum::Earth,players,winners);
	GameReport report2(142,MapEnum::Island,players,winners);
	players[2] = StrategyEnum::ObtainSmallestContinentsFirstStrategy;
	winners[0] = 2;
	winners[2] = 1;
	GameReport report3(13,MapEnum::Earth,players,winners);
	//output
	std::ofstream outStream;
	outStream.open("testReportOutput.txt", std::ios::out);
	report1.write(&outStream);
	report2.write(&outStream);
	report3.write(&outStream);
	outStream.close();
	std::cout << "DONE. testReportOutput.txt generated." << std::endl;
}

void test2() {
    std::cout << "newsimpletest test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (newsimpletest) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
	//We must seed the random number generator.  This one is only used for testing; rng is used more.
	srand((unsigned int)time(NULL));		
	rng = std::default_random_engine(rand());

    std::cout << "%SUITE_STARTING% newsimpletest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (newsimpletest)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (newsimpletest)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (newsimpletest)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (newsimpletest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

