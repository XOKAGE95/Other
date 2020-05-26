#include <iostream>
#include <string>
#include <fstream>


int number = 0;

using namespace std;

bool Parser();
int FileNum();

int main()
{
    number = FileNum();
    Parser();
    //string *name = new string[number];
}

bool Parser()
{
    ifstream file;
    file.open("Enter.txt");
    char symbol;
    ofstream outfile;
    outfile.open("Enter1.txt");
    while(!file.eof())
    {
        symbol = file.get();
        if (symbol == ',') symbol = '.';
        outfile << symbol;
    }
    file.close();
    outfile.close();
    return true;
}

int FileNum()
{
    ifstream file;
    file.open("Enter.txt");

    int count = 0;
    char ch;
    while(!file.eof())
    {
        ch = file.get();
        if(ch == '\n') count ++;
    }
    return count;
}
