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

#include "intelligence.h"
#include "spielfeld.h"

const char *fen_char[] = {  // bei writ()
        "S_Tr",   "S_Kr",   "S_K",    "S_D",    "S_T",  "S_L",  "S_P",
        "S_B",    "S_Bu",   "S_Bp_r", "S_Bp_l", "S_Bx", "LEER", "W_Bx",
        "W_Bp_l", "W_Bp_r", "W_Bu",   "W_B",    "W_P",  "W_L",  "W_T",
        "W_D",    "W_K",    "W_Kr",   "W_Tr",   "RAND"
};

void read_fen(spielfeld sp, string s) {
    std::regex rgx_fen("(\\\\d|[A-Za-y0-9])");

    int ii = 0;
    int x;
    int pos;
    int figur;
    for (std::sregex_iterator i =
            std::sregex_iterator(s.begin(), s.end(), rgx_fen);
        // Figuren finden
         i != std::sregex_iterator();

         ++i) {
        cout << "@";
        std::smatch m = *i;
        // if (i != s.end())
        //  x = std::distance(figuren_char, std::find(fen_char, fen_char + 27,
        //  m[1].str()));
        // else
        //  atoi (
        cout << ii << ":" << x << " " << flush;
        // Positionen in der Liste der Figurennamen finden
        pos = 120 - (21 + ((int)(ii) / 8) * 10 + 8 - ii % 8);
        // Positionen auf dem Feld errechnen, mehrere Spiegelungen

        figur = x - figurenanzahl;
        // Tatsaechliche Zahl fuer die Figuren errechnen
        grundfeld[pos] = x - figurenanzahl;
        // Auf Grundfeld setzen

        ii++;
    }
    return;
}
