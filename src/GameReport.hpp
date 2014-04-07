/*
 * =====================================================================================
 *       Filename:  GameReport.hpp
 *    Description: Reports the results of a single game
 * =====================================================================================
 */

#ifndef  gamereport_INC
#define  gamereport_INC

#include "Strategy.hpp"
#include <vector>

class GameReport
{
public:
// Order is preserved in vectors.
std::vector<Strategy> participants;	//participants[0] is the winner, participants[1] came in second, etc
int rounds;
};

#endif   /* ----- #ifndef gamereport_INC  ----- */
