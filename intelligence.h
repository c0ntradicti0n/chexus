#include <set>
#include "spielfeld.h"
#pragma once

using namespace std;


#ifndef BASIS_H
#define BASIS_H

// TYPEN
struct wandel {
    int pos1;
    int fig;
};
union zug {
    struct {
        char pos1 : 8;
        char pos2 : 8;
        char fig  : 8;
    }   pos;
    int id : 32;
};
struct denkpaar {
    zug    z;
    int    farbe;
    int    figur;
    int    spezial;
    int    bewertung;
    int    i;
    wandel verwandelung[20];
    int    nw;
    bool   reckoning;
    bool   kill;

    int finds[120]; // Find
};

struct figurtyp {
    int  pos1;
    int  pos2;
    bool schach;
    bool bewegt;
};
struct feldtyp  {
    int feld[120];
    int x;
};

int eigene_farbe = 1;  // Sicht des Computers --1-0
int stopp        = 5;  // normale Suchtiefe, wird ver‚Ä∞ndert
const int ende   = 15; // maximale Suchtiefe
int sortiertiefe = 15; // Sortiertiefe - wieviele Z¬∏ge werden sortiert
//int figurenwert_weiss = 0;
//int figurenwert_schwarz = 0;
int figurenwert = 0;

int KooIch       = 300;
static int KooEr        = 50;
static double AttackIch = 5;
static double AttackEr  = 6; //HIER
static int    DefIch1   = 40;
static int    DefIch2   = 80;
static double DefEr     = 17; //
static int    MobDame   = 5; //
static double AttDame   = 0.11;
static int    MobTurm   = 8;
static double AttTurm   = 0.5;
static double MobLau    = 8;
static double AttLau    = 0.75;
static double AttSpr    = 0.75;
static double AttBau    = 3.5; ////
static double AttKoe    = 0.4;
//int Koenigsangriff_Ich = 25;
//int Koenigsangriff_Er = 25;  //25 Koenigsangriff_Ich


enum state { user, uci, gone, position };
static state status = uci;

// const char _DEFAULT_PATH[] = "partie";

static const int figurenanzahl = 12; // bei disp()
static bool killFlag           = true;
static const int MAX_WERT      = 99999999;

static vector<zug> Beam = vector<zug>(ende, zug());

enum howitends      {
    WON =  + 666666,
    PATT = -1000,
    REMIS = -999,
    NORMAL = 0,
    LOST = - 666666
};
map<int, string> END_NAMES = {{WON, "w_won"}, {LOST, "w_lost"},{PATT, "w_patt"}, {NORMAL, "normal"},{REMIS, "w_remis"},
                              {-WON, "b_won"}, {-LOST, "b_lost"},{-PATT, "b_patt"}, {REMIS, "b_remis"}};


enum espezial           { NICHTS = 0, SCHACH = 1, UNRUH = 2 };
enum spiel_status   { Eroeffnung, Mittelspiel, Spaetspiel, Endspiel };
static const char *spiel_status_namen[] = {
        "Eroeffnung", "Mittelspiel", "Mittel-Endspiel", "Endspiel"
};
enum figuren            { // bei disp()
    S_Tr = -12, S_Kr = -11,
    S_K  = -10, S_D, S_T, S_L,
    S_P, S_B, S_Bu, S_Bp_r, S_Bp_l, S_Bx,
    LEER                                                     = 0,
    W_Bx, W_Bp_l, W_Bp_r, W_Bu, W_B, W_P, W_L, W_T, W_D, W_K = 10, W_Kr = 11,
    W_Tr                                                     = 12,
    RAND                                                     = 13
};

map<int, char> MAP_BW = {{RAND, RAND}, {LEER, LEER},
                    {S_T, W_T}, {S_Tr, W_Tr}, 
                    {S_K, W_K}, {S_D, W_D}, 
                    {S_Kr, W_Kr}, {S_L, W_L}, 
                    {S_P, W_P}, {S_B, W_B}, 
                    {S_Bu, W_Bu}, {S_Bp_r, W_Bp_r}, 
                    {S_Bp_l, W_Bp_l}, {S_Bx, W_Bx},
                 
                    {W_T, S_T}, {W_Tr, S_Tr}, 
                    {W_K, S_K}, {W_D, S_D}, 
                    {W_Kr, S_Kr}, {W_L, S_L}, 
                    {W_P, S_P}, {W_B, S_B}, 
                    {W_Bu, S_Bu}, {W_Bp_r, S_Bp_r}, 
                    {W_Bp_l, S_Bp_l}, {W_Bx, S_Bx},
                    };

map<int, string> COLOR_NAMES = {{-1, "WHITE"}, {1, "BLACK"}};

static const char *figuren_char[] = { // bei disp()
        "tmr", "knr", "kon", "dam", "tum", "laf", "pdf", "bau", "baU", "bar", "bal",
        "bax",
        ".",
        "BAX", "BAL", "BAR", "BAu", "BAU", "PDF", "LAF", "TUM", "DAM", "KON", "KNR",
        "TMR", "RAND" };

// bei eingabe()
static const char *figuren_intern[] =       { // bei writ()
        "S_Tr",   "S_Kr",     "S_K",      "S_D",      "S_T",      "S_L",      "S_P",
        "S_B",
        "S_Bu",
        "S_Bp_r", "S_Bp_l",   "S_Bx",
        "LEER",
        "W_Bx",   "W_Bp_l",   "W_Bp_r",   "W_Bu",     "W_B",      "W_P",      "W_L",
        "W_T",
        "W_D",
        "W_K",    "W_Kr",     "W_Tr",
        "RAND" };

static char buchstaben1[] =       { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
static char buchstaben2[] =       { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
static char zahlen[]      =       { '1', '2', '3', '4', '5', '6', '7', '8' };

static const string grundfeld_bezeichnungen[120] = {
        // Ausgabe der Felder
        "RAND", "RAND", "RAND", "RAND", "RAND", "RAND", "RAND", "RAND", "RAND", "RAND",
        "RAND", "RAND", "RAND", "RAND", "RAND", "RAND", "RAND", "RAND", "RAND", "RAND",
        "RAND", "a1",   "b1",   "c1",   "d1",   "e1",   "f1",   "g1",   "h1",   "RAND",
        "RAND", "a2",   "b2",   "c2",   "d2",   "e2",   "f2",   "g2",   "h2",   "RAND",
        "RAND", "a3",   "b3",   "c3",   "d3",   "e3",   "f3",   "g3",   "h3",   "RAND",
        "RAND", "a4",   "b4",   "c4",   "d4",   "e4",   "f4",   "g4",   "h4",   "RAND",
        "RAND", "a5",   "b5",   "c5",   "d5",   "e5",   "f5",   "g5",   "h5",   "RAND",
        "RAND", "a6",   "b6",   "c6",   "d6",   "e6",   "f6",   "g6",   "h6",   "RAND",
        "RAND", "a7",   "b7",   "c7",   "d7",   "e7",   "f7",   "g7",   "h7",   "RAND",
        "RAND", "a8",   "b8",   "c8",   "d8",   "e8",   "f8",   "g8",   "h8",   "RAND",
        "RAND", "RAND", "RAND", "RAND", "RAND", "RAND", "RAND", "RAND", "RAND", "RAND",
        "RAND", "RAND", "RAND", "RAND", "RAND", "RAND", "RAND", "RAND", "RAND",
        "RAND" };

/*int grundfeld[120] =
{ RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
RAND, LEER, LEER, W_K,  LEER, LEER,  LEER,  LEER,  LEER,   RAND,
RAND, LEER, W_T,  LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
RAND, S_K,  LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND };
/*int grundfeld[120] =
{ RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
  RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
  RAND, W_Tr, LEER, LEER, LEER, W_Kr,  LEER,  LEER,  W_Tr,   RAND,
  RAND, W_Bx, W_Bx, W_Bx, LEER, LEER,  LEER,  W_Bx,  LEER,   RAND,
  RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  W_B,   RAND,
  RAND, LEER, LEER, LEER, LEER, W_B,    W_B,  LEER,  LEER,   RAND,
  RAND, LEER, LEER, S_B,  W_P,  S_P,  LEER,  W_L,  LEER,   RAND,
  RAND, S_B, S_D, LEER, S_B, LEER,  LEER,  LEER,  S_B,   RAND,
  RAND, LEER, S_Bx, LEER, LEER, LEER,  S_Bx,  S_Bx,  LEER,   RAND,
  RAND, S_Tr, LEER,  LEER,  LEER,  S_Kr,  S_L,  LEER,   S_Tr,   RAND,
  RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
  RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND };//*/
static int grundfeld[120] =
        { RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, W_Tr, W_P,  W_L,  W_D,  W_Kr,  W_L,   W_P,   W_Tr,   RAND,
          RAND, W_Bx, W_Bx, W_Bx, W_Bx, W_Bx,  W_Bx,  W_Bx,  W_Bx,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, S_Bx, S_Bx, S_Bx, S_Bx, S_Bx,  S_Bx,  S_Bx,  S_Bx,   RAND,
          RAND, S_Tr, S_P,  S_L,  S_D,  S_Kr,  S_L,   S_P,   S_Tr,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND }; // */

/*
   bool in_array(const std::string &value, const std::vector<string> &array)
   {
    return std::find(array.begin(), array.end(), value) != array.end();
   }

   void readCSV(istream &input, vector< vector<string> > &output)
   {
   string csvLine;
    // read every line from the stream
    while( getline(input, csvLine) )
    {
            istringstream csvStream(csvLine);
           vector<string> csvColumn;
            string csvElement;
            // read every element from the line that is seperated by commas
            // and put it into the vector or strings
            while( getline(csvStream, csvElement, ',') )
            {
                    csvColumn.push_back(csvElement);
            }
            output.push_back(csvColumn);
    }
   }
 */

/*int grundfeld[120] =
   {RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
                         RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
                         RAND,	W_Tr,	LEER,	W_L,	W_D,	W_Kr,	W_L,	LEER,	W_Tr,	RAND,
                         RAND,	W_Bx,	W_Bx,	W_Bx,	LEER,	LEER,	W_Bx,	W_Bx,	W_Bx,	RAND,
                         RAND,	LEER,	LEER,	W_P,	LEER,	LEER,	W_P,	LEER,	LEER,	RAND,
                         RAND,	LEER,	S_P,	LEER,	LEER,	LEER,	LEER,	LEER,	LEER,	RAND,
                         RAND,	LEER,	LEER,	LEER,	W_B,	LEER,	LEER,	LEER,	LEER,	RAND,
                         RAND,	LEER,	LEER,	LEER,	LEER,	LEER,	S_P,	LEER,	LEER,	RAND,
                         RAND,	S_Bx,	S_Bx,	S_Bx,	S_Bx,	S_D,	S_Bx,	S_Bx,	S_Bx,	RAND,
                         RAND,	S_Tr,	LEER,	S_L,	LEER,	S_Kr,	S_L,	LEER,	S_Tr,	RAND,
                         RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
                         RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND};
*/
   /*                BEWERTUNG                 */
static int __STARTFELD[120] = // Wei√üer Bauer
        { RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
          RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
          RAND, LEER, LEER, LEER, LEER, LEER, LEER, LEER, LEER, RAND,
          RAND, W_Bx, W_Bx, W_Bx, W_Bx, W_Bx, W_Bx, W_Bx, W_Bx, RAND,
          RAND, W_B,  W_B,  W_B,  W_B,  W_B,  W_B,  W_B,  W_B,  RAND,
          RAND, W_B,  W_B,  W_B,  W_B,  W_B,  W_B,  W_B,  W_B,  RAND,
          RAND, W_B,  W_B,  W_B,  W_B,  W_B,  W_B,  W_B,  W_B,  RAND,
          RAND, W_B,  W_B,  W_B,  W_B,  W_B,  W_B,  W_B,  W_B,  RAND,
          RAND, W_B,  W_B,  W_B,  W_B,  W_B,  W_B,  W_B,  W_B,  RAND,
          RAND, LEER, LEER, LEER, LEER, LEER, LEER, LEER, LEER, RAND,
          RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
          RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND };
static  int __STARTFELDx[120] = // Schwarzer Bauer
        { RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
          RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
          RAND, LEER, LEER, LEER, LEER, LEER, LEER, LEER, LEER, RAND,
          RAND, S_B,  S_B,  S_B,  S_B,  S_B,  S_B,  S_B,  S_B,  RAND,
          RAND, S_B,  S_B,  S_B,  S_B,  S_B,  S_B,  S_B,  S_B,  RAND,
          RAND, S_B,  S_B,  S_B,  S_B,  S_B,  S_B,  S_B,  S_B,  RAND,
          RAND, S_B,  S_B,  S_B,  S_B,  S_B,  S_B,  S_B,  S_B,  RAND,
          RAND, S_B,  S_B,  S_B,  S_B,  S_B,  S_B,  S_B,  S_B,  RAND,
          RAND, S_Bx, S_Bx, S_Bx, S_Bx, S_Bx, S_Bx, S_Bx, S_Bx, RAND,
          RAND, LEER, LEER, LEER, LEER, LEER, LEER, LEER, LEER, RAND,
          RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
          RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND };
static int __STARTFELDx2[120] = // Wei√üer Springer
        { RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
          RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
          RAND, W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  RAND,
          RAND, W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  RAND,
          RAND, W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  RAND,
          RAND, W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  RAND,
          RAND, W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  RAND,
          RAND, W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  RAND,
          RAND, W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  RAND,
          RAND, W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  W_P,  RAND,
          RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
          RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND };
static int __STARTFELDx3[120] = // Schwarzer Springer
        { RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
          RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
          RAND, S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  RAND,
          RAND, S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  RAND,
          RAND, S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  RAND,
          RAND, S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  RAND,
          RAND, S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  RAND,
          RAND, S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  RAND,
          RAND, S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  RAND,
          RAND, S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  S_P,  RAND,
          RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
          RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND };
/*int __STARTFELDx4[120] = // Wei√üer L√§ufer
{ RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  RAND,
  RAND, W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  RAND,
  RAND, W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  RAND,
  RAND, W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  RAND,
  RAND, W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  RAND,
  RAND, W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  RAND,
  RAND, W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  RAND,
  RAND, W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  W_L,  RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND };
int __STARTFELDx5[120] = // Schwarzer L√§ufer
{ RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  RAND,
  RAND, S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  RAND,
  RAND, S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  RAND,
  RAND, S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  RAND,
  RAND, S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  RAND,
  RAND, S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  RAND,
  RAND, S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  RAND,
  RAND, S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  S_L,  RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND };*/
/*int __STARTFELDx6[120] = // Wei√üer Turm
{ RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  RAND,
  RAND, W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  RAND,
  RAND, W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  RAND,
  RAND, W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  RAND,
  RAND, W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  RAND,
  RAND, W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  RAND,
  RAND, W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  RAND,
  RAND, W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  W_T,  RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND };
static int __STARTFELDx7[120] = // Schwarzer Turm
{ RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  RAND,
  RAND, S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  RAND,
  RAND, S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  RAND,
  RAND, S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  RAND,
  RAND, S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  RAND,
  RAND, S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  RAND,
  RAND, S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  RAND,
  RAND, S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  S_T,  RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND };
int __STARTFELDx8[120] = // Wei√üe Dame
{ RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  RAND,
  RAND, W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  RAND,
  RAND, W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  RAND,
  RAND, W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  RAND,
  RAND, W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  RAND,
  RAND, W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  RAND,
  RAND, W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  RAND,
  RAND, W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  W_D,  RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND };
int __STARTFELDx9[120] = // Schwarze Dame
{ RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  RAND,
  RAND, S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  RAND,
  RAND, S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  RAND,
  RAND, S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  RAND,
  RAND, S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  RAND,
  RAND, S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  RAND,
  RAND, S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  RAND,
  RAND, S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  S_D,  RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND,
  RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND, RAND };
static int __STARTFELDx10[120] = // Wei√üer K√∂nig
{ RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,  RAND,
  RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,  RAND,
  RAND, W_K,  W_K,  W_K,  W_K,  W_Kr,  W_K,   W_K,   W_K,   RAND,
  RAND, W_K,  W_K,  W_K,  W_K,  W_K,   W_K,   W_K,   W_K,   RAND,
  RAND, W_K,  W_K,  W_K,  W_K,  W_K,   W_K,   W_K,   W_K,   RAND,
  RAND, W_K,  W_K,  W_K,  W_K,  W_K,   W_K,   W_K,   W_K,   RAND,
  RAND, W_K,  W_K,  W_K,  W_K,  W_K,   W_K,   W_K,   W_K,   RAND,
  RAND, W_K,  W_K,  W_K,  W_K,  W_K,   W_K,   W_K,   W_K,   RAND,
  RAND, W_K,  W_K,  W_K,  W_K,  W_K,   W_K,   W_K,   W_K,   RAND,
  RAND, W_K,  W_K,  W_K,  W_K,  W_K,   W_K,   W_K,   W_K,   RAND,
  RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,  RAND,
  RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,  RAND };
static int __STARTFELDx11[120] = // Schwarzer K√∂nig
{ RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,  RAND,
  RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,  RAND,
  RAND, S_K,  S_K,  S_K,  S_K,  S_K,   S_K,   S_K,   S_K,   RAND,
  RAND, S_K,  S_K,  S_K,  S_K,  S_K,   S_K,   S_K,   S_K,   RAND,
  RAND, S_K,  S_K,  S_K,  S_K,  S_K,   S_K,   S_K,   S_K,   RAND,
  RAND, S_K,  S_K,  S_K,  S_K,  S_K,   S_K,   S_K,   S_K,   RAND,
  RAND, S_K,  S_K,  S_K,  S_K,  S_K,   S_K,   S_K,   S_K,   RAND,
  RAND, S_K,  S_K,  S_K,  S_K,  S_K,   S_K,   S_K,   S_K,   RAND,
  RAND, S_K,  S_K,  S_K,  S_K,  S_K,   S_K,   S_K,   S_K,   RAND,
  RAND, S_K,  S_K,  S_K,  S_K,  S_Kr,  S_K,   S_K,   S_K,   RAND,
  RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,  RAND,
  RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,  RAND };//*/


static int __STARTPUNKTEx[120] = // Wei√üe bauern
        { RAND, RAND,  RAND,  RAND,   RAND,  RAND,  RAND,  RAND, RAND,  RAND,
          RAND, RAND,  RAND,  RAND,   RAND,  RAND,  RAND,  RAND, RAND,  RAND,
          RAND, 0,     0,     0,      0,     0,  	0,     0,    0,     RAND,
          RAND, 8,     16,    24,     32,    32,	24,    16,   8,     RAND,
          RAND, 3,     12,    20,     28,    28,    20,    12,   3,     RAND,
          RAND, -5,    4,     10,     20,    20,    10,    4,    -5,    RAND,
          RAND, -6,    4,     5,      16,    16,    5,     4,    -6,    RAND,
          RAND, -6,    4,     2,      5,     5,     2,     4,    -6,    RAND,
          RAND, -6,    4,     4,      -15,  -15,    4,     4,    -6,    RAND,
          RAND, 0,     0,     0,      0,     0,     0,     0,    0,     RAND,
          RAND, RAND,  RAND,  RAND,   RAND,  RAND,  RAND,  RAND, RAND,  RAND,
          RAND, RAND,  RAND,  RAND,   RAND,  RAND,  RAND,  RAND, RAND,  RAND };
static double __STARTPUNKTE[120] =
        { RAND, RAND, RAND,  RAND,   RAND,  RAND, RAND, RAND,  RAND,   RAND,
          RAND, RAND, RAND,  RAND,   RAND,  RAND, RAND, RAND,  RAND,   RAND,
          RAND, 0,    0,     0,      0,     0,    0,    0,     0,      RAND,
          RAND, -6,   4,     4,      -15,   -15,  4,    4,     -6,     RAND,
          RAND, -6,   4,     2,      5,     5,    2,    4,     -6,     RAND,
          RAND, -6,   4,     5,      16,    16,   5,    4,     -6,     RAND,
          RAND, -5,   4,     10,     20,    20,   10,   4,     -5,     RAND,
          RAND, 3,    12,    20,     28,    28,   20,   12,    3,      RAND,
          RAND, 8,    16,    24,     32,    32,   24,   16,    8,      RAND,
          RAND, 0,    0,     0,      0,     0,    0,    0,     0,      RAND,
          RAND, RAND, RAND,  RAND,   RAND,  RAND, RAND, RAND,  RAND,   RAND,
          RAND, RAND, RAND,  RAND,   RAND,  RAND, RAND, RAND,  RAND,   RAND };
static double __STARTPUNKTEx2[120] = // Weiße Springer
        { RAND, RAND, RAND, RAND,  RAND,   RAND,   RAND,    RAND,  RAND,  RAND,
          RAND, RAND, RAND, RAND,  RAND,   RAND,   RAND,    RAND,  RAND,  RAND,
          RAND, -53,  -42,  -32,   -21,    -21,    -32,     -42,   -53,   RAND,
          RAND, -42,  -32,  -10,   0,      0,      -10,     -32,   -42,   RAND,
          RAND, -21,  -10,  0,     0,      0,      0,       -10,   -21,   RAND,
          RAND, -18,  0,    3,     21,     21,     3,        0,    -18,   RAND,
          RAND, -18,  0,    10,    21,     21,     10,       0,    -18,   RAND,
          RAND, -21,  5,    10,    16,     16,     10,       5,    -21,   RAND,
          RAND, -42,  -32,  -10,   0,      0,      -10,      -32,  -42,   RAND,
          RAND, -53,  -42,  -32,   -21,    -21,    -32,      -42,  -53,   RAND,
          RAND, RAND, RAND, RAND,  RAND,   RAND,   RAND,     RAND, RAND,  RAND,
          RAND, RAND, RAND, RAND,  RAND,   RAND,   RAND,     RAND, RAND,  RAND };

static double __STARTPUNKTEx3[120] =
        { RAND, RAND, RAND, RAND,  RAND,   RAND,   RAND,     RAND, RAND, RAND,
          RAND, RAND, RAND, RAND,  RAND,   RAND,   RAND,     RAND, RAND, RAND,
          RAND, -53,  -42,  -32,   -21,    -21,    -32,      -42,  -53,  RAND,
          RAND, -42,  -32,  -10,   0,      0,      -10,      -32,  -42,  RAND,
          RAND, -21,  5,    10,    16,     16,     10,       5,    -21,  RAND,
          RAND, -18,  0,    10,    21,     21,     10,       0,    -18,  RAND,
          RAND, -18,  0,    3,     21,     21,     3,        0,    -18,  RAND,
          RAND, -21,  -10,  0,     0,      0,      0,        -10,  -21,  RAND,
          RAND, -42,  -32,  -10,   0,      0,      -10,      -32,  -42,  RAND,
          RAND, -53,  -42,  -32,   -21,    -21,    -32,      -42,  -53,  RAND,
          RAND, RAND, RAND, RAND,  RAND,   RAND,   RAND,     RAND, RAND, RAND,
          RAND, RAND, RAND, RAND,  RAND,   RAND,   RAND,     RAND, RAND, RAND };
/*double __STARTPUNKTEx4[120] = //Wei√üe L√§ufer
{RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,-5, -5, -7, -5, -5, -7, -5, -5,	RAND,
RAND,0,  4,  2,  2,  2,  2,  4,  0,RAND,
RAND,0,  3,  4,  4,  4,  4,  3,  0,RAND,
RAND,0,  2,  4,  4,  4,  4,  2,  0,RAND,
RAND,0,  2,  4,  4,  4,  4,  2,  0,RAND,
RAND,0,  2,  4,  4,  4,  4,  2,  0,RAND,
RAND,0,  4,  2,  2,  2,  2,  4,  0,RAND,
RAND,	0,  0,  0,  0,  0,  0,  0,  0,RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND};
double __STARTPUNKTEx5[120] =
{RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	0,  0,  0,  0,  0,  0,  0,  0,RAND,
RAND,0,  4,  2,  2,  2,  2,  4,  0,RAND,
RAND,0,  2,  4,  4,  4,  4,  2,  0,RAND,
RAND,0,  2,  4,  4,  4,  4,  2,  0,RAND,
RAND,0,  2,  4,  4,  4,  4,  2,  0,RAND,
RAND,0,  3,  4,  4,  4,  4,  3,  0,RAND,
RAND,0,  4,  2,  2,  2,  2,  4,  0,RAND,
RAND,-5, -5, -7, -5, -5, -7, -5, -5,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND};*/
/*double __STARTPUNKTEx6[120] = //T√ºrme
{RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	0,  3,  5,  5,  5,  5,  3,  0,		RAND,
RAND,	-3,  2,  5,  5,  5,  5,  2, -3,RAND,
RAND,	-2,  0,  0,  2,  2,  0,  0, -2,RAND,
RAND,	-2,  0,  0,  0,  0,  0,  0, -2,RAND,
RAND,	0,  0,  0,  0,  0,  0,  0,  0,RAND,
RAND,	0,  0,  0,  0,  0,  0,  0,  0,RAND,
RAND,	22, 27, 27, 27, 27, 27, 27, 22,RAND,
RAND,	8, 11, 13, 13, 13, 13, 11,  8,RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND};
double __STARTPUNKTEx7[120] =
{RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	8, 11, 13, 13, 13, 13, 11,  8,RAND,
RAND,	22, 27, 27, 27, 27, 27, 27, 22,RAND,
RAND,	0,  0,  0,  0,  0,  0,  0,  0,RAND,
RAND,	0,  0,  0,  0,  0,  0,  0,  0,RAND,
RAND,	-2,  0,  0,  0,  0,  0,  0, -2,RAND,
RAND,	-2,  0,  0,  2,  2,  0,  0, -2,RAND,
RAND,	-3,  2,  5,  5,  5,  5,  2, -3,RAND,
RAND,	0,  3,  5,  5,  5,  5,  3,  0,		RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND};
double __STARTPUNKTEx8[120] = //Dame
{RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,-10, -5,  0,  0,  0,  0, -5,-10,	RAND,
RAND,-5,  0,  5,  5,  5,  5,  0, -5,RAND,
RAND,0,  5,  5,  6,  6,  5,  5,  0,RAND,
RAND,0,  5,  6,  6,  6,  6,  5,  0,RAND,
RAND,0,  5,  6,  6,  6,  6,  5,  0,RAND,
RAND, 0,  5,  5,  6,  6,  5,  5,  0,RAND,
RAND,-5,  0,  5,  5,  5,  5,  0, -5,RAND,
RAND,10, -5,  0,  0,  0,  0, -5,-10,RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND};
double __STARTPUNKTEx9[120] =
{RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,10, -5,  0,  0,  0,  0, -5,-10,RAND,
RAND,-5,  0,  5,  5,  5,  5,  0, -5,RAND,
RAND, 0,  5,  5,  6,  6,  5,  5,  0,RAND,
RAND,0,  5,  6,  6,  6,  6,  5,  0,RAND,
RAND,0,  5,  6,  6,  6,  6,  5,  0,RAND,
RAND,0,  5,  5,  6,  6,  5,  5,  0,RAND,
RAND,-5,  0,  5,  5,  5,  5,  0, -5,RAND,
RAND,-10, -5,  0,  0,  0,  0, -5,-10,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND};
static double __STARTPUNKTEx10[120] = //K√∂nig
{RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	24, 24,  9,  0,  0,  9, 24, 24,		RAND,
RAND,	16, 14,  7, -3, -3,  7, 14, 16,RAND,
RAND,	4, -2, -5,-15,-15, -5, -2,  4,RAND,
RAND,	-10,-15,-20,-25,-25,-20,-15,-10,RAND,
RAND,	-15,-30,-35,-40,-40,-35,-30,-15,RAND,
RAND,	 -25,-35,-40,-45,-45,-40,-35,-25,RAND,
RAND,	-22,-35,-40,-40,-40,-40,-35,-22,RAND,
RAND,	-22,-35,-40,-40,-40,-40,-35,-22,RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND};
static double __STARTPUNKTEx11[120] =
{RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	-22,-35,-40,-40,-40,-40,-35,-22,RAND,
RAND,	-22,-35,-40,-40,-40,-40,-35,-22,RAND,
RAND,	 -25,-35,-40,-45,-45,-40,-35,-25,RAND,
RAND,	-15,-30,-35,-40,-40,-35,-30,-15,RAND,
RAND,	-10,-15,-20,-25,-25,-20,-15,-10,RAND,
RAND,	4, -2, -5,-15,-15, -5, -2,  4,RAND,
RAND,	16, 14,  7, -3, -3,  7, 14, 16,RAND,
RAND,	24, 24,  9,  0,  0,  9, 24, 24,		RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,
RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND};*/

static double materialwert[15] = // Materialwert
        {
                0,                      // NIL! 0
                100,                    // Startbauer 1
                500,                    // durchgestarteter Startbauer 2
                33.3,                  // en passant Bauer 3
                2250,                   // Superbauer 4
                20,                     // Bauer 5
                66.666666666667,        // Pferd 6
                57.142857,              // Läufer 7
                75,                     // Turm 8
                133.3333333,            // Dame 9
                1000,                   // König 10
                909.0909091,            // Rochade König 11
                50,                     // Rochade Turm 12
        };


// Zahlen fuer den Zuggenerator, Sprenklereinstellungen
static int bewegung[15][15] = // Anzahl, Richtung, Weite, wohin[richtung]
        {
                { 0, 0, 0   },       // -- unsinn, ignorieren --
                { 0, 1, 10  },       // Bauer vor seinem Start
                { 0, 0, 10  },       // durchgestarteter Startbauer (kann kein en passant)
                { 0, 0, 10  },       // en passant Bauer in Aktion, muss sich umverwandelungn
                { 0, 0, 0   },       // Umwandelungsbauer auf der anderen Grundlinie
                { 0, 0, 10  },       // Bauer
                { 7, 0, 21, 12, 19, 8, -21, -12, -19, -8}, // Pferd
                { 3, 6, 9, 11, -9, -11},             // Laeufer
                { 3, 6, 10, 1, -1, -10},             // Turm
                { 7, 6, 9, 10, 11, 1, -1, -9, -10, -11}, // Dame
                { 7, 0, 9, 10, 11, 1, -1, -9, -10, -11}, // Koenig vor Rochade
                { 7, 0, 9, 10, 11, 1, -1, -9, -10, -11}, // Koenig
                { 3, 6, 10, 1, -1, -10},             // Turm vor Rochade
                { 1, 0, 9, 11},                      // Bauer nach seinem Start
        };


// VARIABLEN
static int zug_nummer;
static denkpaar aktueller_zug[ende];
static denkpaar bester_zug[ende];
static int __FELD[ende + 2][120];
static int Feld[ende + 2][200];
static denkpaar zugstapel[ende + 2][200];
static denkpaar best_one[ende + 2];
static int sort_schema[ende][200];
static denkpaar sort_schema_bewertung[ende][200];
static int bewertet;               // MaÔ¨Ç f¬∏r die Partieeinheit
static int timeline;               // Entscheidung Endspiel oder ErÀÜffnung, Einfluss
// auf Bewertung und Suchtiefe
static spiel_status partie_status; // Ereoffnung, Mittelspiel....
static double				  kingzone_ich[120];
static double				  kingzone_gegner[120];
// double					OpenLines[8] =
// {1,1,1,1,1,1,1,1};


struct endofinput : public exception {
    const char* what() const throw() {
        return "end of file in input stream!";
    }
};



#endif //BASIS_H


