/*
 * =====================================================================================
 *
 *       Filename:  continent.cpp
 *
 *    Description:  continent contains region
 *                  if all regions of a continent are claimed, player recieves bonus
 *                  
 *
 *        Version:  1.0
 *        Created:  03/28/2014 08:22:50 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Jacob Meline  
 *
 * =====================================================================================
 */

#include    <iostream>
#include    <vector>
#include    <utility>
#include    <string>
#include    "Continent.hpp"
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  continent
 *  Description:  Constructor for class continent
 * =====================================================================================
 */

Continent::Continent ( std::string name, int value)
{
    setValue(value);
    setContinentName(name);
}		/* -----  end of function continent  ----- */








