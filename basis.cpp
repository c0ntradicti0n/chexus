#include "basis.h"
#include "spielfeld.h"
#include "bewertung.cpp"

int bp(spielfeld & spiel, int farbe, int alpha, double beta, int stufe, int _stopp, /*int devwert, */
       int NullFlag) {    // Bewertung, Planung

    if (NullFlag == 3) {
        spiel.Farbe = farbe;
    } else
        spiel.Farbe = farbe;//*/

    double wertung;


    srand(time(NULL));

    make_schema(zugstapel[spiel.getStufe()], spiel.n, stufe);

    spiel.makeZugstapel();

    if (spiel.spezial == SCHACH) {
        return alpha;
    }

    sort(zugstapel[spiel.getStufe()], spiel.n, stufe);
    int n = spiel.n;  // Anzahl der Zuege
    int nn = 0;       // Anzahl der vom Schach her machbaren Zuege

    for (int i = 0; i < n; i++) {

        testspiel[stufe]->copy(spiel);
        testspiel[stufe]->zug(zugstapel[spiel.getStufe()][i]);

        __end = spiel.last_moves();
        if (__end == NORMAL) {
            nn += 1;
        }

        aktueller_zug[stufe] = zugstapel[spiel.getStufe()][i];


        if (((stufe + 1 >= _stopp) || (stufe + 1 >= ende))) {

            wertung = rand() % 3 - 1;


            wertung += (double) 1.5 * material(Feld[testspiel[stufe]->getStufe()], farbe); //8.75-9		90
            if (alpha < wertung * farbe + 30) {
                wertung += (double) 1.55 *
                           entwicklung(Feld[testspiel[stufe]->getStufe()], farbe);        //0.375-0.4		-->160		1.6
                wertung +=
                                       (double) 0.1 * zuganzahl(Feld[testspiel[stufe]->getStufe()], farbe); //0,8;0.076
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


                wertungn = -bp(*testspiel[stufe], farbe, -beta, -beta + 1, stufe + 1, _stopp - 2, 3);


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


            zugstapel[spiel.getStufe()][i].bewertung = wertung;//}


        }

        if (testspiel[stufe]->spezial == SCHACH) {

            testspiel[stufe]->spezial = NICHTS;
            continue;
        }

        zugstapel[spiel.getStufe()][i].bewertung = wertung;

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

            bester_zug[stufe] = zugstapel[spiel.getStufe()][i];
            best_one[stufe] = zugstapel[spiel.getStufe()][i]; //Aktueller PV-Zug
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