#ifndef Pokemon_h
#define Pokemon_h
#include "Attack.h"

#include <iostream>
#include <clocale>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <string>


using namespace std;

enum Color
{
    Black         = 0,
    Blue          = 1,
    Green         = 2,
    Cyan          = 3,
    Red           = 4,
    Magenta       = 5,
    Brown         = 6,
    LightGray     = 7,
    DarkGray      = 8,
    LightBlue     = 9,
    LightGreen    = 10,
    LightCyan     = 11,
    LightRed      = 12,
    LightMagenta  = 13,
    Yellow        = 14,
    White         = 15
};

class Pokemon
{
public:
    int Number;
    int maxCP;
    string Name;
    string Type;
    string Types[2];
    bool Shiny;
    Attack *Attacks;
public:
    friend istream& operator>>(istream&, Pokemon&);
    friend ostream& operator<<(ostream&, Pokemon&);
    void Definition(ostream&);

};
#endif
