#include "GameManager.hpp"

GameManager::GameManager(std::string outputFileLocation) {}

void GameManager::addMap(MapEnum::MapEnum e) {}

void GameManager::addStrategy(StrategyEnum::StrategyEnum e) {}

void GameManager::runIt() {}

void GameManager::stopEverything() {}

GameReport GameManager::runGame() { return GameReport(); }

void GameManager::readInExistingReport(std::string reportLocation) {}

void GameManager::launchGame(int gameNumber) {}

void GameManager::handleReport(int whoGaveReport, GameReport report) {}
