#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <math.h>
#include <cstdlib>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <regex>
using namespace std;


#ifndef IO_H
#define IO_H

#include "intelligence.h"
#include "common_functions.h"



static string read_file_str(string filename) {
    fstream f(filename, fstream::in);
    string s;
    getline(f, s, '\0');
    f.close();
    return s;
}

char buchstabe_zahl_to_pos(char buchstabe, char zahl)  {
    for (int i = 0; i <= 7; i++) {
        if ((buchstabe == buchstaben1[i]) || (buchstabe == buchstaben2[i])) {
            for (int j = 0; j <= 7; j++) {
                if (zahl == zahlen[j]) {
                    return 21 + j * 10 + i;
                }
            }
        }
    }
    return 0;
}

static char eingabe()  {
    char pos = 0;
    string move = "xxxx";
    char buchstabe, zahl;
    cin.clear();
    do {
        cin >> move;
        if ((int) move[0] == (int) '.') return 123;
        buchstabe = move[0];
        zahl = move[1];
        pos = buchstabe_zahl_to_pos(buchstabe, zahl);
    } while (pos == 0);
    return pos;
}


// FUNKTIONEN
static void disp(char feld[120], char form = 0)  {
    cout << "\n";
    int breite = 3;

    cout << "      " << "  >--A--v--B--v--C--v--D--v--E--v--F--v--G--v--H--<\n";
    cout << "      " << "v >-----+-----+-----+-----+-----+-----+-----+-----< v\n" <<
         "      ";

    for (char j = 9; j > 1; j--)  {
        cout << j - 1;

        for (char i = 1; i < 9; i++)  {
            if (feld[j * 10 + i] != RAND)
                switch (form)   {
                    case 0: { cout << setw(breite) << " | " << setw(breite) <<
                                   figuren_char[feld[j * 10 + i] + figurenanzahl]; break; }

                    case 1: { cout << setw(breite) << " | " << setw(breite) <<
                                   feld[j * 10 + i]; break; }
                }
        }
        cout << setw(breite) << "| " << j - 1 << "\n" << "      " <<
             "^ >-----+-----+-----+-----+-----+-----+-----+-----< ^\n" << "      ";
    }
    cout << "  >--A--+--B--+--C--+--D--+--E--+--F--+--G--+--H--<  \n";
    cout << "\n";
}

static void disp_cleanest(char feld[120])  {
    cout << "\n";

    for (char j = 9; j > 1; j--)  {
        for (char i = 1; i < 9; i++)  {
            if (feld[j * 10 + i] !=
                RAND) cout << figuren_char[feld[j * 10 + i] + figurenanzahl];

            if (j * 10 + i != 28) cout << ",";
        }
        cout << "\n";
    }
    cout << "\n";
}

static void writ(char feld[120], int form = 0)  {
    ofstream partie("partie.txt", ios::app);

    partie << "\n{\n";
    partie << "RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,\n"
           << "RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,\n";

    for (char j = 2; j < 10; j++)  {
        partie << "RAND, ";

        for (char i = 1; i < 9; i++)  {
            if (feld[j * 10 + i] != RAND)
                switch (form)   {
                    case 0: { partie << setw(6) <<
                                     figuren_intern[feld[j * 10 + i] + figurenanzahl]
                                     << ", "; break; }
                }
        }
        partie << "  RAND,";
        partie << "\n";
    }
    partie << "RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,\n"
           << "RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND\n";
    partie << "};";
    partie << "\n";
    partie.close();
}

#endif //IO.H
