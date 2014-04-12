/////
/////
/////  !!!NOTE: I intend to delete this, since, like the strategies, the headers for these are all in GameMap.hpp
/////
/////

#ifndef EARTH_HPP
#define	EARTH_HPP

#include    "../gamemap/GameMap.hpp"

class Earth : public GameMap
{
public:
    Earth();
    void constructContinentsAndRegions();
    void constructBorders();
};

#endif	/* EARTH_HPP */
