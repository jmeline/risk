/*
 * =====================================================================================
 *       Filename:  GameSlave.hpp
 *    Description:  Runs games as requested, and handles MPI communication and such
					NOTE: This class may not be neccecary:  We may put it all in TrueMain.
						The advantage of having it here, however, is the ability to separate non-changing parts, so that TrueMain can be adapted to specific runs without harm.
 * =====================================================================================
 */

#ifndef  gameslave_INC
#define  gameslave_INC

#include "game/Game.hpp"

class GameSlave {
private:
	//MEMBER VARIABLES GO HERE
public:
	void runIt();
private:
	GameReport runOneGame(GameTask task);
	GameTask receiveGameTask();					//communicates with the manager: obtains work.  May be told to stop, rather than to work, in which case it will return a GameTask where every member is -1.
	void sendReport(GameReport report);			//communicates with the manager: reports work
};

#endif   /* ----- #ifndef gameslave_INC  ----- */
