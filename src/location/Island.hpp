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
