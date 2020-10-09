//
// Created by stefan on 04.10.20.
//
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

#include "spielfeld.h"


void zuege_append(vector<string>& zuege, string _hash)  {
    zuege.insert(zuege.end(), (_hash));
}

bool zuege_wied(vector<string>& _zuege)  {
    // outputs a list of uniqe words
    set<string> unique(_zuege.begin(), _zuege.end());

    // for (auto element : unique)
    //	cout << element << ' ' << count(_zuege.begin(), _zuege.end(), element)
    // << endl;

    std::map<std::string, int> count;

    for (auto x : _zuege) ++count[x];

    int  val = 2;
    auto it  = std::find_if(count.begin(), count.end(),
                            [val](const std::pair<std::string, int>& t) -> bool {
                                return t.second > val;
                            }
    );

    if (it == count.end())  {
        return false;
    }
    else {
        cout << "position id " << (it->first)
             << "after moves ";

        int ii = 0;

        for  (auto y : _zuege)  {
            ii += 1;

            if (y == (it->first)) cout << (double)ii / 2 << " ";
        }


        return true;
    }
}

howitends spielfeld::check_end(vector<string> &_zuege) {
    find_kings();
    //print_zugstapel();
    //cout << "STUFE::: "<< this ->getStufe() << " w king: "<< this->wking << " b king: " << this->bking;

    if (this -> wking == 0)  {
        //disp();
        cout << "He took the white king!\n";
        //  test = 1;
        return SCHACHMATT;
    }
    if (this -> bking == 0)  {
        //disp();
        cout << "He took the black king!\n";
        //  test = 1;
        return BLACK_SCHACHMATT;
    }


    if (this->test_drohung(Feld[this->getStufe()], 1, this->wking))  {
        if (Farbe > 0)  {
            return BLACK_SCHACHMATT;
        }
        else {
            return SCHACHMATT;
        }
    }

    if (this->test_drohung(Feld[this->getStufe()], -1, this->bking))  {
        //cout << "schwarz hat schach/n";
        if (Farbe < 0)  {
            return SCHACHMATT;
        }
        else {
            return BLACK_SCHACHMATT;
        }
    }



    //if (bester_zug[0].z.pos.pos1 == 0 && bester_zug[0].z.pos.pos2 == 0)  {
        if (Farbe > 0)  {

            if (this->test_drohung(Feld[this->getStufe()], this->Farbe,
                                   this->wking)) {/*test = 1;*/ return SCHACHMATT;}       // verloren

            if (this->test_drohung(Feld[this->getStufe()], this->Farbe * -1,
                                   this->bking)) {/*test = 1;*/ return BLACK_SCHACHMATT;}  // gewonnen
        }


        if (Farbe < 0)  {
            if (this->test_drohung(Feld[this->getStufe()], this->Farbe * -1,
                                   this->wking)) {/*test = 1;*/
                return SCHACHMATT;}  // gewonnen

            if (this->test_drohung(Feld[this->getStufe()], this->Farbe,
                                   this->bking)) {/*test = 1;*/ return BLACK_SCHACHMATT;}        // verloren
        }

    //}                                                           // moeglich  (was
    // ist mit REMIS
    // bei
    // gefesselten
    if (this->n == 0) {
        return PATT;
    }
    else return NORMAL;
    if (zuege_wied(_zuege)) return REMIS;
    else return NORMAL;
}

int gegner;
howitends spielfeld::last_moves()  {
    // testet, ob, nachdem der Zug gesetzt wurde, noch Schach ist; wenn der
    // Gegner nach seinem Zug noch im Schach steht,
    // hat er einen falschen gemacht
    gegner = this->Farbe;

    find_kings();

    if (Farbe  < 0)  {
        if (this->test_drohung(Feld[this->getStufe()], 1, this->wking))  {
            //cout << "weiss hat schach";

            /* test =1;*/
            return SCHACHMATT; // verloren
        }
    }

    if (Farbe > 0)  {
        if (this->test_drohung(Feld[this->getStufe()], -1, this->bking))  {
            //cout << "schwarz hat schach";
            /*test = 1;*/
            return SCHACHMATT;
        }
    }
    return NORMAL;
}

spielfeld::spielfeld()  {
    Farbe = 0;
    Stufe = 0;
    { for (int i = 0; i < 120; i++)  {
            Feld[Stufe][i] = 0;
        } }
}

spielfeld::spielfeld(int _feld[120], int _farbe = 0, int _stufe = 0)  {
    setFarbe(_farbe);
    setStufe(_stufe);
    { for (int i = 0; i < 120; i++)  {
            Feld[Stufe][i] = _feld[i];
        } }
}

string int_array_to_string(int int_array[], int size_of_array) {
    ostringstream oss("");

    for (int temp = 0; temp < size_of_array; temp++) oss << int_array[temp];
    return oss.str();
}

string spielfeld::hash()  {
    Feld[1][0] = Farbe;
    string str = int_array_to_string(Feld[1], 120);
    Feld[1][0] = RAND;
    string str2 = int_array_to_string(Feld[1], 120);

    std::size_t str_hash = std::hash<std::string>{} (str) +
                                                 std::hash<std::string>{} (str2);

    // std::cout << "hash(" << std::quoted(str) << ") = " << str_hash << '\n';

    return to_string(str_hash);
}

void spielfeld::zug_reset()  {
    Stufe = 0;

    for (int i = 0; i < 120; i++)  {
        Feld[Stufe][i] = Feld[Stufe + 1][i];
    }

    for (int j = 0; j < ende; j++)  {
        bester_zug[j].z.pos.pos1 = 0;
        bester_zug[j].z.pos.pos2 = 0;
    }
}

feldtyp * spielfeld::to_feldtyp(feldtyp *neues) {
    { for (int i = 0; i < 120; i++)  {
            neues->feld[i] = Feld[Stufe][i];
        } }
    return neues;
}

feldtyp * spielfeld::to_feldtyp() {
    feldtyp *neues = new feldtyp;
    { for (int i = 0; i < 120; i++)  {
            neues->feld[i] = Feld[Stufe][i];
        } }

    return neues;
}

inline int * spielfeld::to_feld(int _feld[120])  {
    { for (int i = 0; i < 120; i++)  {
            _feld[i] = Feld[Stufe][i];
        } }
    return _feld;
}

inline int * spielfeld::to_feld()  {
    int *_feld = new int [120];
    { for (int i = 0; i < 120; i++)  {
            _feld[i] = Feld[Stufe][i];
        } }

    return _feld;
}

inline void spielfeld::copy(spielfeld& _spiel)  {
    Farbe = _spiel.Farbe;
    Stufe = _spiel.Stufe;
}

inline void spielfeld::setPos(int _feld[], int _farbe, int _stufe, vector<string> & _zuege) {
    setFarbe(_farbe);
    setStufe(_stufe);
    { for (int i = 0; i < 120; i++)  {
            Feld[Stufe][i] = _feld[i];
        } }
    _zuege.clear();
    return;

}

inline void spielfeld::setFarbe(int _farbe)  {
    Farbe = _farbe;
}

inline void spielfeld::setStufe(int _stufe)  {
    Stufe = _stufe;
}

inline int spielfeld::getStufe()  {
    return Stufe;
}

inline void spielfeld::zug(denkpaar& _zug)  {
    setStufe(Stufe + 1);

    for (int i = 0; i < 120; i++)  {
        Feld[Stufe][i] = Feld[Stufe - 1][i];
    }

    Feld[Stufe][_zug.z.pos.pos2] = Feld[Stufe][_zug.z.pos.pos1];
    Feld[Stufe][_zug.z.pos.pos1] = LEER;

    if ((_zug.nw)) {
        int max = _zug.nw;

        for (int j = 0; j < max; j++)    {
            Feld[Stufe][_zug.verwandelung[j].pos1] = _zug.verwandelung[j].fig;
        }
    }

    setFarbe(Farbe * -1);

    Z = false;
}

inline void spielfeld::norm_zug(denkpaar& _zug)  {
    Feld[0][_zug.z.pos.pos2] = Feld[Stufe][_zug.z.pos.pos1];
    Feld[0][_zug.z.pos.pos1] = LEER;

    if ((_zug.nw)) {
        int max = _zug.nw;

        for (int j = 0; j < max; j++)    {
            Feld[0][_zug.verwandelung[j].pos1] = _zug.verwandelung[j].fig;
        }
    }
    setFarbe(Farbe * -1);
    Z = false;


}

inline void spielfeld::realer_zug(denkpaar& _zug, vector<string>& _zuege)  {
    zug (_zug);
    zuege_append(_zuege, this->hash());
    return;
}

inline void spielfeld::add_zug(const int & pos1,
                               const int & pos2,
                               const int & _n,
                               const bool& _kill,
                               const int & _figur)  {



    zugstapel[Stufe][_n].z.pos.pos1 = pos1;
    zugstapel[Stufe][_n].z.pos.pos2 = pos2;
    zugstapel[Stufe][_n].kill       = _kill;
    zugstapel[Stufe][_n].figur      = _figur;
    n++;
    return;
}

inline void spielfeld::add_verwandelung(const int& farbe,
                                        const int& pos,
                                        const int& was,
                                        const int& n) {
    zugstapel[Stufe][n].verwandelung[zugstapel[Stufe][n].nw].pos1 = pos;
    zugstapel[Stufe][n].verwandelung[zugstapel[Stufe][n].nw].fig  = was * farbe;
    zugstapel[Stufe][n].nw++;
}

inline bool spielfeld::look_richtung_td(const int feld[], const int &farbe, const int &pos, const int &step)  {
    int zielfeld, farbvorzeichen, i;
    for (i = pos + step; 19 < i && i < 100; i += step)  {
        zielfeld = feld[i];
        if (zielfeld == RAND) break;
        if (zielfeld == LEER) continue;

        farbvorzeichen = abs(zielfeld) / zielfeld;

        //if (farbvorzeichen == farbe)
        //    break;

        if ((zielfeld == W_D * farbe * -1) ||
            (zielfeld == W_T * farbe * -1) ||
            (zielfeld == W_Tr * farbe * -1) ||
            (zielfeld == W_K * farbe * -1)||
            (zielfeld == W_Kr * farbe * -1)||
            (zielfeld == W_T * farbe * -1))
            return true;
        else break;
    }
    return false;
}

inline bool spielfeld::look_richtung_ld(const int feld[], const int &farbe, const int &pos, const int &step)  {
    int zielfeld, farbvorzeichen, i;

    for (i = pos + step; 19 < i && i < 100; i += step)  {
        zielfeld = feld[i];
        if (zielfeld == RAND) break;
        if (zielfeld == LEER) continue;

        farbvorzeichen = abs(zielfeld) / zielfeld;

        if (farbvorzeichen == farbe) break;

        if ((zielfeld == W_D * farbe * -1) ||
            (zielfeld == W_L * farbe * -1) )
            return true;
        else break;
    }
    return false;
}

inline bool spielfeld::test_drohung(int feld[], int farbe, int pos)  {
    if ((pos < 0) || (pos > 99)) {
        cout << pos << " ";
        return false;
    }

    return look_richtung_td(feld, farbe, pos, 10) ||
           look_richtung_td(feld, farbe, pos, -10) ||
           look_richtung_td(feld, farbe, pos,  1) ||
           look_richtung_td(feld, farbe, pos, -1) ||
           look_richtung_ld(feld, farbe, pos,  9) ||
           look_richtung_ld(feld, farbe, pos,  -9) ||
           look_richtung_ld(feld, farbe, pos, 11) ||
           look_richtung_ld(feld, farbe, pos, -11) ||

           (feld[pos + 21 * farbe] == W_P * farbe * -1) ||
           (feld[pos + 12 * farbe] == W_P * farbe * -1) ||
           (feld[pos + 19 * farbe] == W_P * farbe * -1) ||
           (feld[pos + 8  * farbe] == W_P * farbe * -1) ||
           (feld[pos + 9  * farbe] == W_B * farbe * -1) ||
           (feld[pos + 11 * farbe] == W_B * farbe * -1) ||

           (feld[pos + 11 * farbe] == W_K * farbe * -1 ||
            feld[pos + -11 * farbe] == W_K * farbe * -1 ||
            feld[pos + 1 * farbe] == W_K * farbe * -1 ||
            feld[pos + -1 * farbe] == W_K * farbe * -1 ||
            feld[pos + 10 * farbe] == W_K * farbe * -1 ||
            feld[pos + -10 * farbe] == W_K * farbe * -1 ||
            feld[pos + 9 * farbe] == W_K * farbe * -1 ||
            feld[pos + -9 * farbe] == W_K * farbe * -1 );
}

inline void spielfeld::find_kings()  {
    // setzt wking und bking auf entsprechende Werte

    wking = 0;
    bking = 0;
    int farbvorzeichen;
    int figur;

    for (int i = 21; i <= 98; i++) {

        figur = (abs(Feld[Stufe][i]));

        if ((figur == LEER) || (figur == RAND))
            continue;


        farbvorzeichen = figur / Feld[Stufe][i];

        if ((figur == W_K) || (figur == W_Kr)) {
            if (farbvorzeichen > 0)
                wking = i;
            else
                bking = i;
        }
    }
}

int spielfeld::zuggenerator()  {
    int pos1, pos2;
    int enp_l, enp_r;
    int ziel, zielfeld;
    int farbvorzeichen, figur;



    int zugnr = 0;

    n = 0; // Variable der Klasse, wenn man es neu deklarieren wuerde,
    // kommt es zu einem seltsamen Fehler in der Zugsortierung,
    // weil n lokal dann 0 bleibt
    int en_passent_bauer = 0;
    spezial = NICHTS;
    test = 0;

    for (int o = 0; o < 199; o++) zugstapel[Stufe][o].nw = 0;

    for (int i = 21; i <= 98; i++) {
        figur = (abs(Feld[Stufe][i]));


        if ((figur == LEER) || (figur == RAND))

            continue;

        farbvorzeichen = figur / Feld[Stufe][i];


        pos1           = i;

        if (farbvorzeichen == Farbe)      {
            if ((figur == W_Bx) || (figur == W_B) || (figur == W_Bp_r) ||
                (figur == W_Bp_l))        {
                // Bauernschlag fuer Startbauer, normalen Bauern, en passant-Bauer
                if ((figur == W_Bp_r) || (figur == W_Bp_l))  {
                    // en passant bauer, der nicht gezogen wird, kann spaeter nicht mehr
                    en_passent_bauer = pos1;
                }




                ziel = pos2 = pos1 + farbvorzeichen * 9;

                // links vor und schlagen
                zielfeld = Feld[Stufe][ziel];

                if  (figur == W_Bp_l)  {
                    add_verwandelung(farbvorzeichen, pos2 - 10 * farbvorzeichen, LEER, n);
                    add_verwandelung(farbvorzeichen, pos2,      W_B,  n);
                    add_zug(pos1, pos2, n, true, figur);
                }


                if (zielfeld != RAND)   {
                    if (zielfeld != LEER)   {
                        // schraeg schlagen
                        if (zielfeld == -1 * W_K * farbvorzeichen || zielfeld == -1 * W_Kr * farbvorzeichen)      {
                            spezial = SCHACH; /*test = 1;*/
                            return 0;
                        }



                        if (zielfeld / abs(zielfeld) != farbvorzeichen)   {
                            spezial = UNRUH;

                            if (figur == W_Bx) {
                                add_verwandelung(farbvorzeichen, pos2, W_B, n);
                            } else
                            if (figur == W_B)  { // Bauernumwandelung per schlag
                                if (Farbe > 0)  {
                                    if ((91 <= pos2) && (pos2 <= 98))  {
                                        add_verwandelung(farbvorzeichen, pos2, W_D, n);
                                        add_zug(pos1, pos2, n, true, W_D);
                                        add_verwandelung(farbvorzeichen, pos2, W_P, n);
                                        add_zug(pos1, pos2, n, true, W_P);

                                        //		break;
                                    }
                                } else
                                if (Farbe < 0)  {
                                    if ((21 <= pos2) && (pos2 <= 28))  {
                                        add_verwandelung(farbvorzeichen, pos2, W_D, n);
                                        add_zug(pos1, pos2, n, true, W_D);
                                        add_verwandelung(farbvorzeichen, pos2, W_P, n);
                                        add_zug(pos1, pos2, n, true, W_P);

                                        //		break;
                                    }
                                }
                            }
                            add_zug(pos1, pos2, n, true, figur);
                        }
                    }
                }

                // rechts vor
                pos2     = ziel = pos1 + farbvorzeichen * 11;
                zielfeld = Feld[Stufe][ziel];

                if  (figur == W_Bp_r)  {
                    add_verwandelung(farbvorzeichen, pos2 - 10 * farbvorzeichen, LEER, n);
                    add_verwandelung(farbvorzeichen, pos2,      W_B,  n);
                    add_zug(pos1, pos2, n, true, figur);
                } else

                if (((zielfeld = Feld[Stufe][ziel]) != RAND))   {
                    if (zielfeld != LEER)   { // schraeg schlagen
                        if (zielfeld == -1 * W_K * farbvorzeichen)      {
                            spezial = SCHACH; /*test = 1;*/
                            return 0;
                        }

                        if (zielfeld / abs(zielfeld) != farbvorzeichen)   {
                            spezial = UNRUH;

                            if (figur == W_Bx) add_verwandelung(farbvorzeichen, pos2, W_B, n);
                            else
                            if (figur == W_B)  { // Bauernumwandelung per schlag
                                if (Farbe > 0)  {
                                    if ((91 <= pos2) && (pos2 <= 98))  {
                                        add_verwandelung(farbvorzeichen, pos2, W_D, n);
                                        add_zug(pos1, pos2, n, true, figur);
                                        add_verwandelung(farbvorzeichen, pos2, W_P, n);
                                        add_zug(pos1, pos2, n, true, figur);

                                        //		break;
                                    }
                                } else
                                if (Farbe < 0)  {
                                    if ((21 <= pos2) && (pos2 <= 28))  {
                                        add_verwandelung(farbvorzeichen, pos2, W_D, n);
                                        add_zug(pos1, pos2, n, true, figur);
                                        add_verwandelung(farbvorzeichen, pos2, W_P, n);
                                        add_zug(pos1, pos2, n, true, figur);

                                        //		break;
                                    }
                                }
                            }
                            add_zug(pos1, pos2, n, true, figur);
                        }
                    }
                }
            } //Bauer endet hier

            // andere figuren
            for (int richtung = 0; richtung <= bewegung[figur][0]; richtung++)  {
                for (int weite = 0; weite <= bewegung[figur][1]; weite++)  {
                    pos2 =  pos1 + farbvorzeichen * bewegung[figur][2 + richtung] *
                                   (weite + 1);
                    int zielfeld = Feld[Stufe][pos2];

                    // schlagen
                    if (zielfeld != LEER)  {
                        int zielfigur = abs(zielfeld);

                        if (zielfeld == RAND)                       // Aus!
                            break;

                        if (zielfeld / zielfigur == farbvorzeichen) // eigene Figur
                            break;

                        if      ((figur == W_Bx) || (figur == W_Bp_l) || (figur  == W_B) ||
                                 (figur == W_Bp_r)) // kein Bauer schlaegt geradeaus
                            break;



                        if ((zielfeld == -1 * W_K * farbvorzeichen) ||
                            (zielfeld == -1 * W_Kr * farbvorzeichen))        {
                            spezial = SCHACH;

                            return 0;
                        }



                        if ((figur == W_Kr) || (figur == W_K)) {
                            if (test_drohung(Feld[Stufe], Farbe, pos1)) {
                                test = 1;
                                break;
                            }
                            if (test_drohung(Feld[Stufe], Farbe, pos2))  {
                                test = 1;
                                break;
                            }
                        }

                        if (figur == W_Kr) {
                            add_verwandelung(farbvorzeichen, pos2, W_K, n);
                        } else
                        if (figur == W_Tr) {
                            add_verwandelung(farbvorzeichen, pos2, W_T, n);
                        }



                        spezial = UNRUH;
                        add_zug(pos1, pos2, n, true, figur);

                        break;
                    }

                    // gehen
                    if (figur == W_B)  { // Bauernumwandelung
                        if (Farbe > 0)  {
                            if ((91 <= pos2) && (pos2 <= 98))  {
                                add_verwandelung(farbvorzeichen, pos2, W_D, n);
                                add_zug(pos1, pos2, n, false, figur);
                                add_verwandelung(farbvorzeichen, pos2, W_P, n);
                                add_zug(pos1, pos2, n, false, figur);

                                //		break;
                            }
                        } else
                        if (Farbe < 0)  {
                            if ((21 <= pos2) && (pos2 <= 28))  {
                                add_verwandelung(farbvorzeichen, pos2, W_D, n);
                                add_zug(pos1, pos2, n, false, figur);
                                add_verwandelung(farbvorzeichen, pos2, W_P, n);
                                add_zug(pos1, pos2, n, false, figur);

                                //		break;
                            }
                        }
                    } else

                    if ((figur == W_Bp_r) || (figur == W_Bp_l)) add_verwandelung(
                                farbvorzeichen,
                                pos2,
                                W_B,
                                n);
                    else
                    if (figur == W_Bx) {
                        add_verwandelung(farbvorzeichen, pos2, W_B, n);

                        if (weite == 1)  {
                            enp_r = Feld[Stufe][pos2 + farbvorzeichen];
                            enp_l = Feld[Stufe][pos2 - farbvorzeichen];

                            if (enp_r == -1 * W_B * farbvorzeichen)  {
                                add_verwandelung(farbvorzeichen * -1,
                                                 pos2 + farbvorzeichen,
                                                 W_Bp_r,
                                                 n);
                            }

                            if (enp_l == -1 * W_B * farbvorzeichen)  {
                                add_verwandelung(farbvorzeichen * -1,
                                                 pos2 - farbvorzeichen,
                                                 W_Bp_l,
                                                 n);
                            }
                        }
                    }

                    if ((figur == W_Kr) || (figur == W_K) ) {
                        if (test_drohung(Feld[Stufe], Farbe, pos1)) {
                            test = 1;

                        }
                        if (test_drohung(Feld[Stufe], Farbe, pos2))  {
                            //    test = 1;
                            break;
                        }

                    }

                    if (figur == W_Kr) {
                        add_verwandelung(farbvorzeichen, pos2, W_K, n);
                    } else
                    if (figur == W_Tr) {
                        add_verwandelung(farbvorzeichen, pos2, W_T, n);
                    }



                    add_zug(pos1, pos2, n, false, figur);
                }
            }
        }
    }

    // Rochade

    if ((Feld[Stufe][25] == W_Kr) || (Feld[Stufe][95] == S_Kr))  {
        if (Farbe < 0) {
            if ((Feld[Stufe][95] == S_Kr) && (Feld[Stufe][96] == LEER) &&
                (Feld[Stufe][97] == LEER) && (Feld[Stufe][98] == S_Tr))  {
                if (!test_drohung(Feld[Stufe], Farbe,
                                  95) &&
                    !test_drohung(Feld[Stufe], Farbe,
                                  96) && !test_drohung(Feld[Stufe], Farbe, 97))  {
                    add_verwandelung(Farbe, 97, W_K,  n);
                    add_verwandelung(Farbe, 96, W_T,  n);
                    add_verwandelung(Farbe, 98, LEER, n);
                    add_zug(95, 97, n, false, figur);
                }
            }

            if ((Feld[Stufe][95] == S_Kr) && (Feld[Stufe][94] == LEER) &&
                (Feld[Stufe][93] == LEER) && (Feld[Stufe][92] == LEER) &&
                (Feld[Stufe][91] == S_Tr))  {
                if (!test_drohung(Feld[Stufe], Farbe,
                                  95) &&
                    !test_drohung(Feld[Stufe], Farbe,
                                  94) && !test_drohung(Feld[Stufe], Farbe, 93))  {
                    add_verwandelung(Farbe, 93, W_K,  n);
                    add_verwandelung(Farbe, 94, W_T,  n);
                    add_verwandelung(Farbe, 91, LEER, n);
                    add_zug(95, 93, n, false, figur);
                }
            }
        } else
        if (Farbe > 0) {
            if ((Feld[Stufe][25] == W_Kr) && (Feld[Stufe][26] == LEER) &&
                (Feld[Stufe][27] == LEER) && (Feld[Stufe][28] == W_Tr))  {
                if (!test_drohung(Feld[Stufe], Farbe,
                                  25) &&
                    !test_drohung(Feld[Stufe], Farbe,
                                  26) && !test_drohung(Feld[Stufe], Farbe, 27))  {
                    add_verwandelung(Farbe, 27, W_K,  n);
                    add_verwandelung(Farbe, 26, W_T,  n);
                    add_verwandelung(Farbe, 28, LEER, n);
                    add_zug(25, 27, n, false, figur);
                }
            }

            if ((Feld[Stufe][25] == W_Kr) && (Feld[Stufe][24] == LEER) &&
                (Feld[Stufe][23] == LEER) && (Feld[Stufe][22] == LEER) &&
                (Feld[Stufe][21] == W_Tr))  {
                if (!test_drohung(Feld[Stufe], Farbe,
                                  25) &&
                    !test_drohung(Feld[Stufe], Farbe,
                                  24) && !test_drohung(Feld[Stufe], Farbe, 23))  {
                    add_verwandelung(Farbe, 23, W_K,  n);
                    add_verwandelung(Farbe, 24, W_T,  n);
                    add_verwandelung(Farbe, 21, LEER, n);
                    add_zug(25, 23, n, false, figur);
                }
            }
        }
    }

    // wenn ein en passent bauer gefunden wurde, dann musser bei allen zuegen,
    // ausser bei seinen eigenen im naechsten zug umgewandelt werden
    if (en_passent_bauer != 0)  {
        int q;

        for (q = 0; q < n; q++)  {
            figur = zugstapel[Stufe][q].figur;

            if (!((figur  == W_Bp_l) || (figur  == W_Bp_r)))  {
                add_verwandelung(Farbe, en_passent_bauer, W_B, q);
            }
        }
    }
    return n;
}

inline denkpaar * spielfeld::makeZugstapel()  {
    zuggenerator();
    Z = true;
    return zugstapel[Stufe];
}

int schach_bewegung[15][15] = // Richtung, Weite, wohin[richtung]
        {
                { 0, 0, 0   },              // NIL!
                { 7, 0, 21, -21, 12, -12, 19, -19, 8, -8},               // Pferd
                { 7, 6, 1, -1, 9, -9, 10, -10, 11, -11},              // Dame;
        };

inline bool spielfeld::schach(int _farbe)  {
    /*	int pos1, pos2, figur, farbvorzeichen;


            for (int i=21; i<=98; i++ )	{  //König finden
                    figur= (abs(Feld[Stufe][i]));
                    if (figur == W_K || figur == W_Kr)  {
                            farbvorzeichen = figur/Feld[Stufe][pos1];
                            if (farbvorzeichen == _farbe)  {
                                    pos1 = i;
                                    break;
                                    }
                            }
                    }
            for (int schach_bewegung_figur = 1; schach_bewegung_figur <= 2;
               schach_bewegung_figur++)  {
                    for (int richtung = 0; richtung
                       <=schach_bewegung[schach_bewegung_figur][0]; richtung++)  {
                            for (int weite = 0; weite <=
                               schach_bewegung[schach_bewegung_figur][1]; weite++)
                                {
                                    pos2 =	pos1 + farbvorzeichen *
                                       schach_bewegung[schach_bewegung_figur][2+richtung]
                                       * (weite+1);
                                    int pos2_figur = Feld[Stufe][pos2];
                                    if (zielfeld != LEER)  {
                                            int zielfigur = abs(zielfeld);
                                            if (zielfeld == RAND) // über den
                                               Tellerrand schauen...
                                                    break;
                                            if (zielfeld/zielfigur ==
                                               farbvorzeichen)   // eigene Figur
                                                    break;
                                            if    ((figur == W_Bx) || (figur ==
                                               W_Bp_l) || (figur  == W_B) ||
                                               (figur == W_Bp_r))	// kein Bauer
                                               schlÃ¤gt geradeaus
                                                    break;
                                            return 0;
                                            }
                                            add_zug(pos1, pos2, n, true);
                                    break;
                                    }
                            add_zug(pos1, pos2, n);
                                    }
                            }
                    }
            return n;*/
    return 0;
}

void spielfeld::disp()  {
    cout << "\n";
    int breite = 3;

    cout << COLOR_NAMES[Farbe]
              << " " << "  >--A--v--B--v--C--v--D--v--E--v--F--v--G--v--H--<\n";
    cout << "      " << "v >-----+-----+-----+-----+-----+-----+-----+-----< v\n" <<
         "      ";

    for (int j = 9; j > 1; j--)  {
        cout << j - 1;

        for (int i = 1; i < 9; i++)  {
            if (Feld[Stufe][j * 10 + i] != RAND) cout << setw(breite) << " | " << setw(
                        breite) << figuren_char[Feld[Stufe][j * 10 + i] + figurenanzahl];
        }
        cout << setw(breite) << "| " << j - 1 << "\n" << "      " <<
             "^ >-----+-----+-----+-----+-----+-----+-----+-----< ^\n" << "      ";
    }
    cout << "  >--A--+--B--+--C--+--D--+--E--+--F--+--G--+--H--< ^\n";
    cout << "\n";
}

void spielfeld::disp_cleanest()  {
    cout << "\n";

    for (int j = 9; j > 1; j--)  {
        for (int i = 1; i < 9; i++)  {
            if (Feld[Stufe][j * 10 + i] !=
                RAND) cout << figuren_char[Feld[Stufe][j * 10 + i] + figurenanzahl];

            if (j * 10 + i != 28) cout << ",";
        }
        cout << "\n";
    }
    cout << "\n";
}

void spielfeld::switch_feld()  {
    Farbe *= -1;
    feldtyp neues = feldtyp();
    for (int i = 21; i <= 98; i++)  {
        neues.feld[119-i] = MAP_BW[Feld[Stufe][i]];
    }
    for (int i = 21; i <= 98; i++) {
        Feld[Stufe][i] = neues.feld[i];
    }
}

void spielfeld::print_zugstapel()  {
    cout << "n=" << n << endl;
    for (int i = 0; i < n; i++)  {
        cout << figuren_char[zugstapel[Stufe][i].figur  + figurenanzahl] << ": "
             << int(zugstapel[Stufe][i].z.pos.pos1) << "(" <<
             grundfeld_bezeichnungen[zugstapel[Stufe][i].z.pos.pos1] << ")  => "
             << int(zugstapel[Stufe][i].z.pos.pos2) << "(" <<
             grundfeld_bezeichnungen[zugstapel[Stufe][i].z.pos.pos2] << ")";

        if (zugstapel[Stufe][i].nw) {
            cout << " | ";
            int max = zugstapel[Stufe][i].nw;

            for (int j = 0; j < max; j++)    {
                cout << int(zugstapel[Stufe][i].verwandelung[j].pos1)
                     << "(" <<
                     grundfeld_bezeichnungen[zugstapel[Stufe][i].verwandelung[j].pos1] <<
                     ")  <= "
                     << figuren_char[zugstapel[Stufe][i].verwandelung[j].fig  +
                                     figurenanzahl];

                if (j < max - 1) cout << ", ";
            }
        }

        cout << "\n";
    }
}

void spielfeld::write()  {
    ofstream partie("partie.txt", ios::app);

    partie << "\n{\n";
    partie << "RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,\n"
           << "RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,	RAND,\n";

    for (int j = 2; j < 10; j++)  {
        partie << "RAND, ";

        for (int i = 1; i < 9; i++)  {
            if (Feld[Stufe][j * 10 + i] != RAND)
                partie
                        << setw(6)
                        << figuren_intern[Feld[Stufe][j * 10 + i] + figurenanzahl]
                        << ", ";
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
