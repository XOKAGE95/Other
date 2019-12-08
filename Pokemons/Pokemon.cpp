#include "Pokemon.h"

#include <iostream>
#include <clocale>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <string>

void ComparisonType(string Type);

void SetColor(Color text, Color background);

 void Pokemon :: Definition(ostream& file)
{
    SetColor(DarkGray, White);
    file << left << setw(6) << "#";
    file << left << setw(15) << "Name";
    file << left << setw(20) << "Type";
    file << left << setw(10) << "MAX CP" << endl;

}

istream& operator >> (istream& file, Pokemon& Pok)
{
    file >> Pok.Number;
    file.ignore(255, '\n');
    getline(file, Pok.Name);
    getline(file, Pok.Type);
    int number;
    number = Pok.Type.find(" ", 0);
    Pok.Types[0].assign(Pok.Type, 0, number);
    Pok.Type.erase(0, number+1);
    number = Pok.Type.find(" ", 0);
    Pok.Types[1].assign(Pok.Type, 0, number);
    Pok.Type.erase(0, number+1);
    file >> Pok.maxCP;
    file.ignore(255, '\n');
    Pok.Attacks = new Attack[10];
    string Att;
    int o = 0;
    getline(file, Att);
    MakeAttack(Pok.Attacks, Att, &o, "fast");
    getline(file, Att);
    MakeAttack(Pok.Attacks, Att, &o, "charged");
    return file;
}

ostream& operator<< (ostream& file, Pokemon& Pok)
{
    Pok.Definition(file);
    SetColor(White, Black);
    file << left << "#" << setw(5) << Pok.Number;
    file << left << setw(15) << Pok.Name;
    ComparisonType(Pok.Types[0]);
    file << left << setw(10) << Pok.Types[0];
    ComparisonType(Pok.Types[1]);
    file << left << setw(10) << Pok.Types[1];
    SetColor(White, Black);
    file << left << setw (5) << Pok.maxCP << endl;
    SetColor(DarkGray, White);
    file << left << setw(10) << "Attacks:" << endl;
    SetColor(White, Black);
    for (int i = 0; i < 10; i++)
        file << Pok.Attacks[i] << endl;
    return file;

}

void FindPokemon(string Name, int Num, Pokemon* Pokemons)
{
    bool PokemonCount = false;
    for (int i = 0; i < Num; i++)
        if (Pokemons[i].Name == Name)
            cout << Pokemons[i];


}
