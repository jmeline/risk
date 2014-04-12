/////
/////
/////  !!!NOTE: I intend to delete this, since, like the strategies, the headers for these are all in GameMap.hpp
/////
/////

#ifndef ISLAND_HPP_
#define ISLAND_HPP_

#include    "../gamemap/GameMap.hpp"

class Island : public GameMap
{
public:
    Island();
    void constructContinentsAndRegions();
    void constructBorders();
};


#endif /* end ISLAND_HPP_ */
