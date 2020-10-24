#include <omp.h>
#include "intelligence.h"
#include "spielfeld.h"
#include "bewertung.cpp"


basic_ostream<char>  * tree_file = &cout;

static void init_test_spiel_array();
static int run(int _stopp, spielfeld &spiel);
static int run_speaking(int _stopp, spielfeld &spiel);


void graph_debug(int farbe, int alpha, double beta, int stufe, double wertung, string x);

#ifndef INTELLIGENCE_CPP
#define INTELLIGENCE_CPP


static void init_test_spiel_array() {
    for (int i = 0; i < ende + 2; i++) {
        //testbrett[i] = new feldtyp;
        //testspiel[i] = new spielfeld();
    }
}

static const set<int> ENDS = { WON  ,LOST , PATT, REMIS, MAX_WERT};
char  * bewertungs_feld;
int null_move_value;
const int R = 1;

static int _bp(
        spielfeld & spiel,
        int farbe,
        int alpha,
        double beta,
        int stufe,
        int _stopp,
        int level
) {// Bewertung, Planung

    if (((level + 1 > _stopp) || (level + 1 >= ende))) {

        bewertungs_feld = Feld[stufe];
        int wertung = rand() % 3 - 1;

        wertung += 1.5 * material(bewertungs_feld, farbe); //8.75-9		90


        if (alpha < wertung * farbe + 30) {
            wertung += 1.55 *
                       entwicklung(bewertungs_feld, farbe);        //0.375-0.4		-->160		1.6

            wertung += 0.1 *
                       zuganzahl(bewertungs_feld, farbe); //0,8;0.076

        }

        //graph_debug(farbe, alpha, beta, stufe, wertung, "");
        evaluations +=1;
        return farbe * wertung;
    }

    spiel.Farbe = farbe;

    int wertung = 0;

    //make_schema(zugstapel[stufe], spiel.n_zuege, stufe);

    spiel.makeZugstapel();

    int end = spiel.check_end(*new vector<string>);
    if (ENDS.find(end) != ENDS.end())  {
        //graph_debug(farbe, alpha, beta, stufe, wertung, END_NAMES[end]);
        //
        // spiel.disp();
        //cout << end;

        return end;

    }

    sort(zugstapel[stufe], spiel.n_zuege, stufe);
    const int n = spiel.n_zuege;  // Anzahl der Zuege
    int step;
    denkpaar * move;

    for (int i = 0; i < n; i++) {
        move = & zugstapel[stufe][i];
        step =  move->kill ? 1 : 1;
        testspiel[stufe].copy(spiel);
        testspiel[stufe].zug(*move);

        /*
        if (ENDS.find(alpha)== ENDS.end() &&
            ENDS.find(beta)== ENDS.end() &&
            testspiel[stufe].test_drohung(Feld[stufe], farbe,spiel.my_king_pos, spiel.my_king_pos)){
            null_move_value = -_bp(
            testspiel[stufe],
            -farbe,
            -beta,
            -beta+1,
            stufe + 1,
            _stopp,
            level + R + 1);

            if (ENDS.find(null_move_value)== ENDS.end()  && null_move_value>= beta) {
                // cutoff in case of fail-high
                alpha = null_move_value;
                break;
            }
            testspiel[stufe].copy(spiel);
        }*/


        /*
        if (!valid_move(move->z)) {
            print_move(cout, move->z, stufe);
            print_moves(Beam, stufe);
            cout << "nothing moving\n";
            spiel.makeZugstapel();

            continue;
        }
        if (!valid_figure(move->z,  spiel.to_feld(), stufe)) {
            //spiel.disp();
            print_moves(Beam, stufe);

            cout << "invalid figure on move \n";
        }*/


        aktueller_zug[stufe] = *move;

        // null move (make two moves with the same side and look, if it's good at all)
        /*
        if ((_stopp-stufe)>2) {

            int wertungn = 0;

            wertungn = -_bp(
                    *testspiel[stufe],
                    farbe,
                    alpha,
                    beta,
                    stufe + 1,
                    _stopp-2,
                    level);  //ergibt beta -1
            //    else wertungn = bp(*testspiel[stufe], -farbe, -beta, -beta+1, stufe + 1, _stopp-2, 3);

            if (ENDS.find(abs(wertungn)) != ENDS.end() &&
                ENDS.find(abs(alpha)) != ENDS.end()  &&
                ENDS.find(abs(beta)) != ENDS.end() &&
                wertungn >= beta) {
                return beta;
            }
        }*/

        wertung = -_bp(
                testspiel[stufe],
                -farbe,
                -beta,
                -alpha,
                stufe + 1,
                _stopp,
                level + step);


        zugstapel[stufe][i].bewertung = wertung;


        if (wertung > alpha) {
            alpha = wertung; // alpha acts like max in MiniMax
            bester_zug[stufe] = zugstapel[stufe][i];
            best_one[stufe] = zugstapel[stufe][i]; //Aktueller PV-Zug
            //graph_debug(farbe, alpha, beta, stufe, wertung, "AlphaAdjust");
        }

        if (wertung >= beta) {
            //bester_zug[stufe] = zugstapel[stufe][i];
            //best_one[stufe] = zugstapel[stufe][i]; //Aktueller PV-Zug
            //best_one[stufe].bewertung *= 0.5; //ACHTUNG 5

            //graph_debug(farbe, alpha, beta, stufe, wertung, "BetaReturn");
            break;  //  fail hard beta-cutoff
        }

        if (stufe==0 && _stopp == STOPP) {
            cout << ""
                 << grundfeld_bezeichnungen[aktueller_zug[0].z.pos.pos1]
                 << " => "	<< grundfeld_bezeichnungen[aktueller_zug[0].z.pos.pos2]
                 << " Bewertung: "
                 << setw(5) << wertung
                 <<", Zug-ID "
                 << setw(6) << aktueller_zug[0].z.id << "\n";
            cout.flush(); }



    }
    if (abs(alpha) == MAX_WERT)  {
        spiel.disp();
        cout<< "# alpha="  << alpha << " beta "<< beta<< " wertung " << wertung<< endl;
        throw "returning -max value as best";
    }
    //graph_debug(farbe, alpha, beta, stufe, wertung, "AlphaReturn");

    make_schema(zugstapel[stufe], spiel.n_zuege, stufe);


    switch (stufe)  {
        case 0:
            {
                return alpha;
            }
        case 1:
        {
            return alpha;
        }
        case 2:
        {
            return alpha;
        }
        case 3:
        {
            return alpha;
        }
        case 4:
        {
            return alpha;
        }
        case 5:
        {
            return alpha;
        }
        case 6:
        {
            return alpha;
        }
        case 7:
        {
            return alpha;
        }
        case 8:
        {
            return alpha;
        }
        case 9:
        {
            return alpha;
        }
        case 10:
        {
            return alpha;
        }
        case 11:
        {
            return alpha;
        }
        case 12:
        {
            return alpha;
        }
        case 13:
        {
            return alpha;
        }
        case 14:
        {
            return alpha;
        }
        case 15:
        {
            return alpha;
        }
    }
    return alpha;
}


static int bp(
        spielfeld & spiel,
        int farbe,
        int alpha,
        double beta,
        int stufe,
        int _stopp,
        int level
) {
    evaluations = 0;
    return _bp(spiel, farbe, alpha, beta, stufe, _stopp, level);
}


void graph_debug(int farbe, int alpha, double beta, int stufe, double wertung, string x) {
    *tree_file
    << "\nw="  << setw(  1 )   << wertung << " x=" << x
    << " a=" << alpha << " b=" << beta
    << " S=" << stufe << " F=" << farbe << "|";
    print_moves(Beam, stufe, *tree_file);
}

/*

        if (stufe == 0) {
            cout << ""
                 << grundfeld_bezeichnungen[aktueller_zug[0].z.pos.pos1]
                 << " => " << grundfeld_bezeichnungen[aktueller_zug[0].z.pos.pos2]
                 << " Bewertung: "
                 << setw(5) << wertung
                 << ", Zug-ID "
                 << setw(6) << aktueller_zug[0].z.id << "\n_zuege";
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
    return bp(spiel, spiel.Farbe,  -MAX_WERT, +MAX_WERT, 0, _stopp, /*devwert, */0);
}
static int run_speaking(int _stopp, spielfeld &spiel) {
    int wert =  bp(spiel, spiel.Farbe,  -MAX_WERT, MAX_WERT, 0, _stopp, /*devwert, */0);
    cout << "info depth " << STOPP << " score cp " << wert / 1.5 << " pv " <<
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
    cout << "evaluations " << evaluations << endl;
    return wert;
}

#endif