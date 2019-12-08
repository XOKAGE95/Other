#ifndef Attack_h
#define Attack_h

#include <iostream>
#include <clocale>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

class Attack
{
public:
    string Name;
    string Type;
    string DPS;
    string Energy;
    string mode;
    friend void MakeAttack (Attack *Att, string Attacks, int* i, string);
    friend ostream& operator<< (ostream&, Attack);

};

#endif
