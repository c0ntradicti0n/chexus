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
#include "io.cpp"
#include "bewertung.cpp"
#include "intelligence.cpp"
#include "spielfeld.cpp"

ofstream spoken("spoken");

string getString(string *names, int index, int index_index);

template <class T, size_t N>
constexpr size_t size(T(&)[N]) {
    return N;
}

int main(int argc, char **argv) {
    srand (time(NULL));
    char *fvalue = NULL;
    string svalue;
    int ladefeld[120] = {RAND};
    opterr = 0;
    vector<string> zuege;
    std::regex rgx_feld("(\\.|[A-Za-y]{3})");
    std::smatch matches;
    string s;
    int c;
    int ii = 0;
    int pos = 0;
    int figur;
    bool geladen = false;
    bool allein = false;
    bool _user = false;
    feldtyp *xbrettchen = new feldtyp;


    bool exit = false;
    int pos1;
    int pos2;
    int wert;
    int zug_nummer = 1;
    timeline = 0;
    time_t t1, t2, t0;

    // Kommandozeilenargumente

    while ((c = getopt(argc, argv, "aus:f:")) != -1)
        switch (c) {
            case 'f': {  // Farbe setzen
                fvalue = optarg;
                eigene_farbe = atoi(fvalue);
            }
                break;
            case 'u': {  // User modus
                _user = true;
            }
                break;
            case 'a': {  // Allein mit sich spielen
                allein = true;
            }
                break;

            case 's': {  // Datei einlesen, aus Kommandozeile kopiert
                *grundfeld = *ladefeld;
                svalue = optarg;
                cout << svalue;
                s = read_file_str(svalue);
                cout << "|" << s << "|";
                // Datei lesen
                cout << s;
                for (std::sregex_iterator i =
                        std::sregex_iterator(s.begin(), s.end(), rgx_feld);
                    // Figuren finden
                     i != std::sregex_iterator(); ++i) {
                    cout << "@";
                    std::smatch m = *i;
                    int x = std::distance(
                            figuren_char,
                            std::find(figuren_char, figuren_char + 27, m[1].str()));
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
                cout << " geladenes spielfeld: \n";
                disp(grundfeld);
                geladen = true;
            }
                break;
            case '?':
                if (optopt == 'c')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                abort();
        }

    {
        init_test_spiel_array();
    }

    spielfeld spiel(grundfeld, +1, 0);

    spiel.to_feldtyp(xbrettchen);

    // UCI Protokoll
    if (!_user)
        do {

            string command;
            cin >> command;

            if (spoken.is_open()) {
                try {
                    spoken << command << flush << "\n";
                } catch (const std::ios_base::failure &e) {
                    std::cout << "Caught std::ios_base::failure: " << typeid(e).name()
                              << std::endl;
                } catch (const std::exception &e) {
                    std::cout << "Caught std::exception: " << typeid(e).name()
                              << std::endl;
                }
            } else {
                cout << "Unable to open file";
            }

            beginning:

            if (command == "-user") {
                status = user;
                break;
            }
            if (command == "-show") {
                spiel.disp();
            }
            if (command == "-show_csv") {
                spiel.disp_cleanest();
            }
            // wichtige Initkommandos - wo man antworten muss

            if (command == "uci") {
                cout << "id name NEXUS 181102\n";  // 750250 6000
                cout << "id author Albrecht Fiebiger & Stefan Werner\n";
                cout << "uciok\n";
            }

            if (command == "isready") {
                cout << "readyok\n";
            }
            if (command == "quit") {
                return 0;
            }

            // Spielkommandos...
            if (command == "position") {
                cin >> command;
                spoken << command << flush << "\n";

                if (command == "startpos") {
                    spiel.setPos(grundfeld, +1, 0, zuege);
                    zug_nummer = 1;

                    cin >> command;
                    spoken << command << flush << "\n";

                    if (command == "moves") {
                        while (true) {
                            cin >> command;
                            spoken << command << flush << "\n";



                            denkpaar _zug;
                            int i;
                            int j;
                            for (i = 0; i <= 7; i++) {
                                if ((command[0] == buchstaben1[i]) ||
                                    (command[0] == buchstaben2[i])) {
                                    for (j = 0; j <= 7; j++) {
                                        if (command[1] == zahlen[j]) {
                                            _zug.z.pos.pos1 = 21 + j * 10 + i;
                                        }
                                    }
                                }
                            }
                            for (i = 0; i <= 7; i++) {
                                if ((command[2] == buchstaben1[i]) ||
                                    (command[2] == buchstaben2[i])) {
                                    for (j = 0; j <= 7; j++) {
                                        if (command[3] == zahlen[j]) {
                                            _zug.z.pos.pos2 = 21 + j * 10 + i;
                                        }
                                    }
                                }
                            }

                            spiel.makeZugstapel();
                            bool falsch = true;
                            for (i = 0; i < spiel.n; i++) {
                                if ((zugstapel[spiel.Stufe][i].z.id == _zug.z.id)) {
                                    spiel.realer_zug(zugstapel[spiel.Stufe][i], zuege);

                                    spiel.zug_reset();
                                    _zug = zugstapel[spiel.Stufe][i];
                                    //	Analysedatei.note (_zug, eigene_farbe * -1, false);
                                    falsch = false;
                                    break;
                                }
                            }
                            //for (auto e : zuege) cout << e << " ";
                            if (falsch == true)
                                goto beginning;  // eine Goto-Anweisung; Wehe dem
                            // Spagettiprogramm!
                            zug_nummer += 1;
                        }
                    }
                }
            }

            if (command == "go") {
                t1 = clock();
                spiel.setStufe(0);

                for (int _stopp = 1;; _stopp++) {
                    make_schema(zugstapel[spiel.getStufe()], spiel.n, 0);
                    move_sort_schema();

                    wert =  run_speaking(_stopp, spiel);
                    if ((clock() - t1 >= 300) && (_stopp >= stopp))
                        break;
                }

                t2 = clock();
                timeline = (double)(timeline * (zug_nummer - 1) / zug_nummer +
                                    (t2 - t1) / zug_nummer);
                int spez;
                denkpaar *zugstapel = new denkpaar[200];

                exit = true;
                switch (spiel.check_end(zuege, 0)) {
                    case MATT: {
                        cout << "Verloren\n";
                        break;
                    }
                    case PATT: {
                        cout << "Patt\n";
                        break;
                    }
                    case REMIS: {
                        cout << "Remis\n";
                        break;
                    }
                    case SCHACHMATT: {
                        cout << "Gewonnen\n";
                        break;
                    }
                    case NORMAL: {
                        exit = false;
                        break;
                    }
                    default: {
                        cout << "Unbekanntes Spielende!";
                        break;
                    }
                }

                zug_nummer += 1;
            }
        } while (true);


    // Benutzermodus

    do {
        disp(spiel.to_feld());
        writ(spiel.to_feld());

        if (!((zug_nummer == 1) && (eigene_farbe == 1))) {  // Benutzerzug
            bool ok = false;
            do {
                if (allein) {
                    eigene_farbe *= -1;
                    break;
                }
                denkpaar *zugstapel = new denkpaar[200];
                int spez;
                int n = spiel.zuggenerator();


                cout << "Zug " << zug_nummer << ", von ";
                pos1 = eingabe();
                if (pos1 == 666 || pos2 == 666) {
                    eigene_farbe *= -1;
                    break;
                }
                cout << "nach ";
                pos2 = eingabe();

                if (pos1 == 666 || pos2 == 666) {
                    eigene_farbe *= -1;
                    break;
                }

                for (int i = 0; i < n; i++) {
                    if ((zugstapel[i].z.pos.pos1 == pos1) &&
                        (zugstapel[i].z.pos.pos2 == pos2)) {
                        ok = true;
                        spiel.realer_zug(zugstapel[i], zuege);
                        //zuege_append(zuege, spiel.hash());
                        //if (zuege_wied(zuege)) exit = true;
                        spiel.zug_reset();
                        break;
                    }
                }
                if (ok == false)
                    cout << "\nUnmoegliche Eingabe, vertippt?\n";
                delete[] zugstapel;
            } while (!ok);
        }


        bewertet = 0;
        t1 = clock();

        spiel.setStufe(0);
        //  int devwert = 0;
        for (int _stopp = 1; ; _stopp++) {

            cout << "Suchtiefe " << _stopp << "\n";
            //   if (_stopp == 0)

            wert = run(_stopp, spiel);
            //     if (_stopp == stopp-2) devwert = wert;
            /*     else {
                     int alpha = wert - 30;
                     int beta = wert + 30;
                     wert = bp(spiel, spiel.Farbe, alpha, beta, 0, _stopp,1 );
                     if (wert <= alpha || wert >= beta) {
                         wert = bp(spiel, spiel.Farbe, -MAX_WERT, MAX_WERT, 0, _stopp, 1);
                     }*/

            //   }
            if ((clock() - t1 >= 300) && (_stopp >= stopp))
                break;
        }

        make_schema(zugstapel[spiel.getStufe()], spiel.n, 0);
        move_sort_schema();
        t2 = clock();
        timeline = (double)(timeline * (zug_nummer - 1) / zug_nummer +
                            (t2 - t1) / zug_nummer);
        int spez;
        denkpaar *zugstapel = new denkpaar[200];

        cout << "\nmove " << (double)zug_nummer / 2 << ": "
             << ((spiel.Farbe > 0) ? "white" : "black") << ", "
             << "time " << (double)(t2 - t1) / CLOCKS_PER_SEC << "s, "
             << "average " << (double)(timeline) / CLOCKS_PER_SEC << "s, "
             << "total " << (double)(clock() - t0) / CLOCKS_PER_SEC << "s, "
             << "evaluations " << bewertet << "\n";

        cout << "result     " << grundfeld_bezeichnungen[bester_zug[0].z.pos.pos1]
             << " => " << grundfeld_bezeichnungen[bester_zug[0].z.pos.pos2] << "\n";

        spiel.realer_zug(bester_zug[0], zuege);

        exit = true;
        switch (spiel.check_end(zuege, 0)) {
            case MATT: {
                cout << "Verloren\n";
                break;
            }
            case PATT: {
                cout << "nn = " << spiel.nn;
                cout << "Patt\n";
                break;
            }
            case REMIS: {
                cout << "Remis\n";
                break;
            }
            case SCHACHMATT: {
                cout << "Gewonnen/n";
                break;
            }
            case NORMAL: {
                //cout << "weiter\n";
                exit = false;
                break;
            }
            default: {
                cout << "Unbekanntes Spielende!";
                break;
            }
        }

        spiel.zug_reset();
        zug_nummer++;

    } while (!exit);

    cout << "\n\n							"
            "ENDE\n";

    spoken.close();
    return 0;
}
