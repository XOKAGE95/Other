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
    AllPokemons,
    Help,


};

const int MAXNUM = 10;
void FindPokemon(string Name, int Num, Pokemon* Pokemons);
void AllPokemon(Pokemon* Pokemons, int Num);
void Commands();
void WriteCommand(string);
void WriteText(string);

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
    else if (Type == "Grass")
        SetColor(Green , Black);
    else if (Type == "Ice")
        SetColor(LightBlue , Black);
    else if (Type == "Dark")
        SetColor(DarkGray, Black);
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
        for (int o = 0; o < i - 1; o++)
            if (Pokemons[o].Number > Pokemons[o+1].Number)
            {
                a = Pokemons[o+1];
                Pokemons[o+1] = Pokemons[o];
                Pokemons[o] = a;
            }
    cout << "Write help to display a list of commands\n";
    while ( 1 > 0 )
    {
        string Name;
        ask A;
        getline(cin, Name);
        if (Name == "End" || Name == "end")
            A = finish;
        else if (Name == "All" || Name == "all")
            A = AllPokemons;
        else if (Name == "Help" || Name == "help")
            A = Help;
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
        case AllPokemons:
            AllPokemon(Pokemons, i);
            break;
        case Help:
            Commands();
            break;
        }
    }
}


void Commands()
{
    WriteCommand("End");
    WriteText(" or ");
    WriteCommand("end");
    WriteText(" to exit.\n");
    WriteCommand("Help");
    WriteText(" or ");
    WriteCommand("help");
    WriteText(" to display commands.\n");
    WriteText("Write");
    WriteCommand(" Name");
    WriteText(" of pokemon to display him\n");
    WriteCommand("All");
    WriteText(" or ");
    WriteCommand("all");
    WriteText(" to display all pokemons\n");
}
void WriteCommand(string name)
{
    SetColor(LightRed , Black);
    cout << name;
}

void WriteText(string name)
{
    SetColor(LightGray , Black);
    cout << name;
}
