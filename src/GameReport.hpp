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
// Why not order winning strategies first to represent rank information?
std::vector<Strategy> participants;
int rounds;
};

#endif   /* ----- #ifndef gamereport_INC  ----- */
