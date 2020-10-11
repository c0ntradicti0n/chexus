#include "intelligence.h"
#include "spielfeld.h"
#include "bewertung.cpp"


basic_ostream<char>  * tree_file = &cout;

static void init_test_spiel_array();
static int run(int _stopp, spielfeld &spiel);
static int run_speaking(int _stopp, spielfeld &spiel);


#ifndef INTELLIGENCE_CPP
#define INTELLIGENCE_CPP


static void init_test_spiel_array() {
    for (int i = 0; i < ende + 2; i++) {
        testbrett[i] = new feldtyp;
        testspiel[i] = new spielfeld();
    }
}

static int bp(spielfeld & spiel, int farbe, int alpha, double beta, int stufe, int _stopp, /*int devwert, */
       int NullFlag) {    // Bewertung, Planung
    if (((stufe + 1 >= _stopp) || (stufe + 1 >= ende))) {
        int wertung = rand() % 3 - 1;

        wertung += (double) 1.5 * material(Feld[stufe], farbe); //8.75-9		90

        if (alpha < wertung * farbe + 30) {
            wertung += (double) 1.55 *
                       entwicklung(Feld[stufe], farbe);        //0.375-0.4		-->160		1.6
            wertung += (double) 0.1 *
                       zuganzahl(Feld[stufe], farbe); //0,8;0.076
        }
        *tree_file <<"\nw="<< wertung << " a=" << alpha<< " b="<< beta << " S="<< stufe << " F="<< farbe<< "|";
        print_moves(Beam, stufe, *tree_file);
        return wertung * farbe;
    }

    spiel.Farbe = farbe;

    double wertung = 0;

    //make_schema(zugstapel[stufe], spiel.n, stufe);

    spiel.makeZugstapel();

    // Todo partien zuvor
    howitends end = spiel.check_end(*new vector<string>);

    if (end != NORMAL)
        return end;

    sort(zugstapel[stufe], spiel.n, stufe);
    int n = spiel.n;  // Anzahl der Zuege
    int nn = 0;       // Anzahl der vom Schach her machbaren Zuege

    for (int i = 0; i < n; i++) {
        denkpaar *move = &zugstapel[stufe][i];
        if (!valid_move(move->z)) {
            cout << "nothing moving";
            print_move(cout, move->z);
        }
        if (!valid_figure(move->z, Feld[stufe])) {
            cout << "invalid figure on move";
        }

        testspiel[stufe]->copy(spiel);
        testspiel[stufe]->zug(*move);

        __end = spiel.last_moves();
        if (__end == SCHACHMATT) {
            throw "illegal move done, check after move remains";
        }

        aktueller_zug[stufe] = zugstapel[stufe][i];


        wertung = -bp(
                *testspiel[stufe],
                farbe * -1,
                -beta,
                -alpha,
                stufe + 1, _stopp,
                1);


        zugstapel[stufe][i].bewertung = wertung;

        if (wertung >= beta) {
            bester_zug[stufe] = zugstapel[stufe][i];
            best_one[stufe] = zugstapel[stufe][i]; //Aktueller PV-Zug
            best_one[stufe].bewertung *= 0.5; //ACHTUNG 5

            *tree_file <<"\nw="<< wertung <<" x=BetaRet" << " a=" << alpha<< " b="<< beta << " S="<< stufe << " F="<< farbe<< "|";
            print_moves(Beam, stufe, *tree_file);
            return beta;   //  fail hard beta-cutoff

        }
        if (wertung > alpha) {
            alpha = wertung; // alpha acts like max in MiniMax
            bester_zug[stufe] = zugstapel[stufe][i];
            best_one[stufe] = zugstapel[stufe][i]; //Aktueller PV-Zug
            best_one[stufe].bewertung *= 0.5; //ACHTUNG 5
            *tree_file <<"\nw="<< wertung <<" x=AlphaAdjust" << " a=" << alpha<< " b="<< beta << " S="<< stufe << " F="<< farbe<< "|";
            print_moves(Beam, stufe, *tree_file);
        }

    }
    *tree_file <<"\nw="<< wertung <<" x=AlphaRet" << " a=" << alpha<< " b="<< beta << " S="<< stufe << " F="<< farbe<< "|";
    print_moves(Beam, stufe, *tree_file);
    return alpha;
}

/*

        if (stufe == 0) {
            cout << ""
                 << grundfeld_bezeichnungen[aktueller_zug[0].z.pos.pos1]
                 << " => " << grundfeld_bezeichnungen[aktueller_zug[0].z.pos.pos2]
                 << " Bewertung: "
                 << setw(5) << wertung
                 << ", Zug-ID "
                 << setw(6) << aktueller_zug[0].z.id << "\n";
            cout.flush();
        }
        //else { if (stufe == 0) cout << "*" << flush;}
        if (wertung <= beta) {
            spiel.nn = nn;

            if (stufe< 5) {
                //cout << endl;
                beta = wertung;
                return beta;
            }
        }
        if (wertung > alpha) {

            bester_zug[stufe] = zugstapel[stufe][i];
            best_one[stufe] = zugstapel[stufe][i]; //Aktueller PV-Zug
            best_one[stufe].bewertung *= 0.5; //ACHTUNG 5



            alpha = wertung;
        }
    }
    spiel.nn = nn;
    return alpha;
}
 */



static int run(int _stopp, spielfeld &spiel) {
    return bp(spiel, spiel.Farbe,  -MAX_WERT, +MAX_WERT, 0, _stopp, /*devwert, */1);
}
static int run_speaking(int _stopp, spielfeld &spiel) {
    int wert =  bp(spiel, spiel.Farbe,  -MAX_WERT, MAX_WERT, 0, _stopp, /*devwert, */1);
    cout << "info depth " << stopp << " score cp " << wert/1.5 << " pv " <<
         " " << grundfeld_bezeichnungen[bester_zug[0].z.pos.pos1]<< grundfeld_bezeichnungen[bester_zug[0].z.pos.pos2]
         <<"(" << bester_zug[0].bewertung <<")" <<
         " " << grundfeld_bezeichnungen[bester_zug[1].z.pos.pos1]<< grundfeld_bezeichnungen[bester_zug[1].z.pos.pos2]
         <<"(" << bester_zug[1].bewertung <<")" <<
         " " << grundfeld_bezeichnungen[bester_zug[2].z.pos.pos1]<< grundfeld_bezeichnungen[bester_zug[2].z.pos.pos2]
         <<"(" << bester_zug[2].bewertung<<")" <<
         " " << grundfeld_bezeichnungen[bester_zug[3].z.pos.pos1]<< grundfeld_bezeichnungen[bester_zug[3].z.pos.pos2]
         <<"(" << bester_zug[3].bewertung <<")" <<
         " " << grundfeld_bezeichnungen[bester_zug[4].z.pos.pos1]<< grundfeld_bezeichnungen[bester_zug[4].z.pos.pos2]
         <<"(" << bester_zug[4].bewertung <<")" <<
         " " << grundfeld_bezeichnungen[bester_zug[5].z.pos.pos1]<< grundfeld_bezeichnungen[bester_zug[5].z.pos.pos2]
         <<"(" << bester_zug[5].bewertung <<")" <<
         "\n";
    cout << "bestmove ->" <<bester_zug[0].z.pos.pos1<< "-< "<< grundfeld_bezeichnungen[bester_zug[0].z.pos.pos1]
         << grundfeld_bezeichnungen[bester_zug[0].z.pos.pos2] << "\n";
    return wert;
}

#endif