/*
 * =====================================================================================
 *
 *       Filename:  gamemap.hpp
 *
 *    Description: Represents a country
 *                 
 *
 *        Version:  1.0
 *        Created:  03/28/2014 08:59:46 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Jacob Meline  
 *
 * =====================================================================================
 */

#ifndef  gamemap_INC
#define  gamemap_INC

#include <string>
#include <vector>

class Country
{
    public:
        Country(std::string name, int v){
            this->countryName = name;
            this->value = v;
        }

    private:
        int value;
        std::string countryName;
};

#endif   /* ----- #ifndef gamemap_INC  ----- */
