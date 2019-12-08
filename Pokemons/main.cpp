#include <iostream>
#include <clocale>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <string>
#include <Windows.h>

#include "Pokemon.h"
#include "Attack.h"
enum ask
{
    finish,
    PokemonName,

};

const int MAXNUM = 10;
void FindPokemon(string Name, int Num, Pokemon* Pokemons);

void SetColor(Color text, Color background)
{
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void ComparisonType(string Type)
{
    if (Type == "Water")
        SetColor(Blue, Black);
    else if (Type == "Ground")
        SetColor(Brown, Black);
    else if (Type == "Poison")
        SetColor(LightMagenta, Black);
    else if (Type == "Fire")
        SetColor(LightRed, Black);
    else if (Type == "Normal")
        SetColor(LightGray , Black);
}

using namespace std;
int main()
{
    Pokemon *Pokemons = new Pokemon[MAXNUM];
    ifstream file;
    file.open("Pokemons.txt");
    int i = 0;
    while (!file.eof())
    {
        file >> Pokemons[i];
        i++;
    }
    Pokemon a;
    for (int z = 0; z < i - 1; z++)
        for (int o = z; o < i - 1; o++)
            if (Pokemons[o].Number > Pokemons[o+1].Number)
            {
                a = Pokemons[o+1];
                Pokemons[o+1] = Pokemons[o];
                Pokemons[o] = a;
            }
    while ( 1 > 0 )
    {
        string Name;
        ask A;
        getline(cin, Name);
        if (Name == "End")
            A = finish;
        else
            A = PokemonName;


        switch (A)
        {
        case finish:
            return 0;
            break;
        case PokemonName:
            FindPokemon(Name, i, Pokemons);
            break;
        }
    }
}
