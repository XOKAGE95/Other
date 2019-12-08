#include "Attack.h"
#include "Pokemon.h"

#include <iostream>
#include <clocale>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

void ComparisonType(string Type);

void SetColor(Color text, Color background);

void MakeAttack (Attack *Att, string Attacks, int* o, string mode)
{
    int number, i = *o;
    while (Attacks.length())
    {
        Att[i].mode = mode;
        number = Attacks.find(",", 0);
        Att[i].Name.assign(Attacks, 0, number);
        Attacks.erase(0, number+2);
        number = Attacks.find(",", 0);
        Att[i].Type.assign(Attacks, 0, number);
        Attacks.erase(0, number+2);
        number = Attacks.find(",", 0);
        Att[i].DPS.assign(Attacks, 0, number);
        Attacks.erase(0, number+2);
        number = Attacks.find(";", 0);
        Att[i].Energy.assign(Attacks, 0, number);
        Attacks.erase(0, number+2);
        i++;
    }
    *o = i;
}

ostream& operator<< (ostream& file, Attack Att)
{
    file << setw(25) << left << Att.Name;
    ComparisonType(Att.Type);
    file << setw(10) << left << Att.Type;
        SetColor(White, Black);
    file << setw(10) << left << Att.mode;
    file << setw(10) << left << Att.DPS;
    file<< setw(10) << left << Att.Energy;
    return file;

}
