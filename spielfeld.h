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
                        const int & n,
                        const bool& _kill,
                        const int & _figur);
    inline void add_verwandelung(const int& farbe,
                                 const int& pos,
                                 const int& was,
                                 const int& n);
    inline bool      test_drohung(int feld[],
                                  int farbe,
                                  int pos);
    inline int check_end(vector<string> &_zuege);
    inline howitends last_moves();

    int n;
    int nn;
    bool Z;
    int Stufe;
    int Farbe;
    int spezial;
    int wking = 99;
    int bking = 99;
    int test = 0;

    spielfeld();
    spielfeld(int _feld[120],
              int _farbe,
              int _stufe);
    void      zug_reset();

    // spielfeld (spielfeld & spiel);
    feldtyp * to_feldtyp(feldtyp *neues);
    feldtyp * to_feldtyp();
    int     * to_feld(int _feld[120]);
    int     * to_feld();
    void      copy(spielfeld& _spiel);

    void      setFarbe(int _farbe);

    int       getStufe();
    void      setStufe(int i);

    bool      look_richtung_td(const int feld [], const int &farbe, const int &pos, const int &step);
    bool      look_richtung_ld(const int feld [], const int &farbe, const int &pos, const int &step);

    void find_kings();

    void      setPos(int _feld[120],
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
    void norm_zug(denkpaar&);
    bool schach(int _farbe);

    void switch_feld();

};


static feldtyp * testbrett[ende+1];

static spielfeld * testspiel[ende + 1];

static howitends __end = NORMAL;

#endif  // NEXUS_SPIELFELD_H
