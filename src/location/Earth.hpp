/* 
 * File:   Earth.hpp
 * Author: Jacob
 */

#ifndef EARTH_HPP
#define	EARTH_HPP

#include    "../gamemap/GameMap.hpp"
#include    <vector>
#include    <string>
#include    <utility>

class Earth : public GameMap
{
public:
    Earth();
    void constructContinentsAndRegions();
    void constructBorders();
};

#endif	/* EARTH_HPP */
