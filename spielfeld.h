//
// Created by stefan on 04.10.20.
//

#ifndef NEXUS_SPIELFELD_H
#define NEXUS_SPIELFELD_H

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


class spielfeld  {

public:

    int         zuggenerator();
    inline void add_zug(const int & pos1,
                        const int & pos2,
                        int & n,
                        const bool& _kill,
                        const int & _figur);
    inline void add_verwandelung(const int& farbe,
                                 const int& pos,
                                 const int& was,
                                 const int& n);


    int n_zuege;

    int nn;
    int Stufe;
    int Farbe;
    int spezial;

    int my_king_pos, op_king_pos;
    int wking = 99;
    int bking = 99;
    int test = 0;

    spielfeld();
    spielfeld(char _feld[120],
              int _farbe,
              int _stufe);
    void      zug_reset();

    // spielfeld (spielfeld & spiel);
    feldtyp * to_feldtyp(feldtyp *neues);
    feldtyp * to_feldtyp();
    char     * to_feld(char _feld[120]);
    char     * to_feld();
    void      copy(spielfeld& _spiel);

    void      setFarbe(int _farbe);

    int       getStufe();
    void      setStufe(int i);

    inline bool test_drohung(const char feld[], const int &farbe, const int &pos, const int &except_pos);
    inline bool look_richtung_td(const char feld [], const int &farbe, const int &pos, const int &except_pos, const int &step);
    inline bool look_richtung_ld(const char feld [], const int &farbe, const int &pos, const int &except_pos, const int &step);
    inline int check_end(vector<string> &_zuege);

    void find_kings();

    void      setPos(char _feld[120],
                     int _farbe,
                     int _stufe,
                     vector<string>& _zuege);
    string    hash();

    void      disp();
    void      print_feld_computable();
    void      disp_cleanest();
    void      write();
    void      print_zugstapel();
    denkpaar* makeZugstapel();

    void zug(denkpaar&);

    void realer_zug(denkpaar&, vector<string>& _zuege);
    void make_move_real(string move);

    bool schach(int _farbe);

    void switch_feld();

    void  add_verwandelung_zug(int farbvorzeichen, char pos2, figuren figuren, int & zuege,
                               char pos1, char pos21, int & zuege1, bool b, int figur);

    bool KILL;
};


static feldtyp testbrett[ende+1]{};

static spielfeld testspiel[ende + 1]{};



static howitends __end = NORMAL;

#endif  // NEXUS_SPIELFELD_H
