#include "intelligence.h"
#include "spielfeld.h"
#include "bewertung.cpp"

void init_test_spiel_array();
int run(int _stopp, spielfeld &spiel);
int run_speaking(int _stopp, spielfeld &spiel);



void init_test_spiel_array() {
    for (int i = 0; i < ende + 2; i++) {
        testbrett[i] = new feldtyp;
        testspiel[i] = new spielfeld();
    }
}

int bp(spielfeld & spiel, int farbe, int alpha, double beta, int stufe, int _stopp, /*int devwert, */
       int NullFlag) {    // Bewertung, Planung

    spiel.Farbe = farbe;

    double wertung;


    srand(time(NULL));
    if(stufe!=spiel.getStufe())
        cout <<"nicht stufe gleich";

    make_schema(zugstapel[spiel.getStufe()], spiel.n, stufe);

    spiel.makeZugstapel();

    howitends end = spiel.check_end(*new vector<string>());

    if (end != NORMAL)
        return end;

    sort(zugstapel[spiel.getStufe()], spiel.n, stufe);
    int n = spiel.n;  // Anzahl der Zuege
    int nn = 0;       // Anzahl der vom Schach her machbaren Zuege

    for (int i = 0; i < n; i++) {

        testspiel[stufe]->copy(spiel);
        int __stufe = testspiel[stufe]->getStufe();
        testspiel[stufe]->zug(zugstapel[__stufe][i]);

        __end = spiel.last_moves();
        if (__end == SCHACHMATT) {
            throw "illegal move done, check after move remains";
        }

        aktueller_zug[stufe] = zugstapel[__stufe][i];

        if (((stufe + 1 >= _stopp) || (stufe + 1 >= ende))) {

            wertung = rand() % 3 - 1;

            wertung += (double) 1.5 * material(Feld[__stufe], farbe); //8.75-9		90
            
            if (alpha < wertung * farbe + 30) {
                wertung += (double) 1.55 *
                           entwicklung(Feld[__stufe], farbe);        //0.375-0.4		-->160		1.6
                wertung += (double) 0.1 * 
                           zuganzahl(Feld[__stufe], farbe); //0,8;0.076
            }

            if (((wertung * farbe > alpha && wertung * -1 * farbe <= beta + 150 - figurenwert / 20) &&
                 aktueller_zug[stufe].kill && stufe < _stopp + 5)) {
                wertung = -bp(*testspiel[stufe], farbe * -1, -beta, -alpha, stufe + 1, _stopp, 1);
            } else {
                wertung = wertung * (farbe);

                if (farbe == 1) {
                    wertung -= 50;
                }
                bewertet++;;
            }
        } else {


            if ((NullFlag == 1) && (_stopp - stufe) > 2) {

                int wertungn = 0;


                wertungn = -bp(
                        *testspiel[stufe], farbe,
                        -beta, -beta + 1,
                        stufe + 1, _stopp - 2,
                        3);


                if (wertungn >= beta && abs(beta) != MAX_WERT) {
                    return beta;
                }


            }


            if (NullFlag == 1) {
                if ((_stopp - stufe) > 2) {
                    wertung = -bp(*testspiel[stufe], -farbe, -beta, -alpha, stufe + 1, _stopp, 4);
                } else
                    wertung = -bp(*testspiel[stufe], farbe * -1, -beta, -alpha, stufe + 1, _stopp, 1);
            } else
                wertung = -bp(*testspiel[stufe], farbe * -1, -beta, -alpha, stufe + 1, _stopp, 2);


            zugstapel[__stufe][i].bewertung = wertung;//}


        }

        if (testspiel[stufe]->spezial == SCHACH) {

            testspiel[stufe]->spezial = NICHTS;
            continue;
        }

        zugstapel[__stufe][i].bewertung = wertung;

        if (stufe == 0 && _stopp == stopp) {
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

        if (wertung > alpha) {

            bester_zug[stufe] = zugstapel[__stufe][i];
            best_one[stufe] = zugstapel[__stufe][i]; //Aktueller PV-Zug
            best_one[stufe].bewertung *= 0.5; //ACHTUNG 5

            if (wertung >= beta) {
                spiel.nn = nn;
                return beta;
            }

            alpha = wertung;
        }
    }
    spiel.nn = nn;
    return alpha;
}



int run(int _stopp, spielfeld &spiel) {
    return bp(spiel, spiel.Farbe,  -MAX_WERT, MAX_WERT, 0, _stopp, /*devwert, */1);
}
int run_speaking(int _stopp, spielfeld &spiel) {
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