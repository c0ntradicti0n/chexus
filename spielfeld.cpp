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


#ifndef SPIELFELD_CPP
#define SPIELFELD_CPP


#include "bewertung.cpp"
#include "spielfeld.h"
#include "io.cpp"


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

int opponents_color;
int spielfeld::check_end(vector<string> &_zuege) {
    find_kings();
    opponents_color = Farbe*-1;
    //print_zugstapel();
    //cout << "STUFE::: "<< this ->getStufe() << " w king: "<< this->wking << " b king: " << this->bking;

    if (this -> wking == 0)  {
        disp();
        cout << "He took the white king!\n";
        return LOST ;
    }
    if (this -> bking == 0)  {
        cout << "He took the black king!\n";
        disp();
        return LOST ;
    }

    if (this->test_drohung(Feld[this->getStufe()], this->Farbe, my_king_pos, my_king_pos)) {
        // If we have check, we can move
        if (this->n_zuege == 0) {
            return LOST;
        }
    }

    if (this->test_drohung(Feld[this->getStufe()], opponents_color, op_king_pos, op_king_pos)) {
        // If we can take the king, we won
        return WON;
    }

    if (zuege_wied(_zuege))
        return REMIS ;

    if (this->n_zuege == 0) {
        return PATT ;
    }
    else return NORMAL * Farbe;

}

spielfeld::spielfeld()  {
    Farbe = 0;
    Stufe = 0;
    for (int i = 0; i < 120; i++)  {
        Feld[Stufe][i] = 0;
    }
}

spielfeld::spielfeld(char _feld[120], int _farbe = 0, int _stufe = 0)  {
    setFarbe(_farbe);
    setStufe(_stufe);
    { for (int i = 0; i < 120; i++)  {
            Feld[Stufe][i] = _feld[i];
        } }
}

string int_array_to_string(char int_array[], int size_of_array) {
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

    // std::cout << "hash(" << std::quoted(str) << ") = " << str_hash << '\n_zuege';

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

inline char * spielfeld::to_feld(char _feld[120])  {
    { for (int i = 0; i < 120; i++)  {
            _feld[i] = (char) Feld[Stufe][i];
        } }
    return _feld;
}

inline char * spielfeld::to_feld()  {
    char *_feld = new char [120];
    for (int i = 0; i < 120; i++)  {
            _feld[i] = Feld[Stufe][i];
        }

    return _feld;
}

inline void spielfeld::copy(spielfeld& _spiel)  {
    Farbe = _spiel.Farbe;
    Stufe = _spiel.Stufe+1;

    for (int i = 0; i < 120; i++)  {
        Feld[Stufe][i] = Feld[_spiel.Stufe][i];
    }

}

inline void spielfeld::setPos(char _feld[], int _farbe, int _stufe, vector<string> & _zuege) {
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
    Beam[Stufe] = _zug.z;

    Feld[Stufe][_zug.z.pos.pos2] = Feld[Stufe][_zug.z.pos.pos1];
    Feld[Stufe][_zug.z.pos.pos1] = LEER;

    if ((_zug.nw)) {
        for (int j = 0; j < _zug.nw; j++)    {
            Feld[Stufe][_zug.verwandelung[j].pos1] = _zug.verwandelung[j].fig;
        }
    }
}


void spielfeld::make_move_real(string move)  {
    int n = zuggenerator();
    bool ok = false;

    int pos1 = buchstabe_zahl_to_pos(move[0], move[1]);
    int pos2 = buchstabe_zahl_to_pos(move[2], move[3]);

    for (int i = 0; i < n; i++) {
        union zug z = zugstapel[Stufe][i].z;
        if ((z.pos.pos1 == pos1) &&
            z.pos.pos2 == pos2) {
            ok = true;
            realer_zug(zugstapel[Stufe][i], *new vector<string>);
            zug_reset();
            break;
        }
    }
    if (!ok)
        throw "move to be made not found in zugstapel!";
}

inline void spielfeld::realer_zug(denkpaar& _zug, vector<string>& _zuege)  {
    zug (_zug);
    zuege_append(_zuege, this->hash());
    Farbe =  Farbe *-1;
    return;
}

inline void spielfeld::add_zug(const int & pos1,
                               const int & pos2,
                               int & _n,
                               const bool& _kill,
                               const int & _figur)  {
    zugstapel[Stufe][_n].z.pos.pos1 = pos1;
    zugstapel[Stufe][_n].z.pos.pos2 = pos2;
    zugstapel[Stufe][_n].z.pos.fig  = _figur;
    zugstapel[Stufe][_n].kill       = _kill;

    if (!valid_move(zugstapel[Stufe][_n].z)){
      cout << " adding invalid move";
    }

    _n+=1;
}

inline void spielfeld::add_verwandelung(const int& farbe,
                                        const int& pos,
                                        const int& was,
                                        const int& n) {
    zugstapel[Stufe][n].verwandelung[zugstapel[Stufe][n].nw].pos1 = pos;
    zugstapel[Stufe][n].verwandelung[zugstapel[Stufe][n].nw].fig  = was * farbe;
    zugstapel[Stufe][n].nw++;
}


inline bool spielfeld::look_richtung_td(const char feld[], const int &farbe, const int &pos, const int &except_pos, const int &step)  {
    int zielfeld, farbvorzeichen, test_pos;

    for (test_pos = pos + step; 19 < test_pos && test_pos < 100; test_pos += step)  {
        zielfeld = feld[test_pos];
        if (zielfeld == RAND) break;

        if (zielfeld == LEER) continue;
        if (test_pos == except_pos) continue;

        if ((zielfeld == W_D * farbe * -1) ||
            (zielfeld == W_T * farbe * -1) ||
            (zielfeld == W_Tr * farbe * -1) ||
            (zielfeld == W_T * farbe * -1))
            return true;
        else break;
    }
    return false;
}

inline bool spielfeld::look_richtung_ld(const char feld[], const int &farbe, const int &pos, const int &except_pos, const int &step)  {
    int zielfeld, farbvorzeichen, test_pos;

    for (test_pos = pos + step; 19 < test_pos && test_pos < 100; test_pos += step)  {
        zielfeld = feld[test_pos];
        if (zielfeld == RAND) break;
        if (zielfeld == LEER) continue;
        if (test_pos == except_pos)
            continue;

        farbvorzeichen = abs(zielfeld) / zielfeld;

        if (farbvorzeichen == farbe) break;

        if ((zielfeld == W_D * farbe * -1) ||
            (zielfeld == W_L * farbe * -1) )
            return true;
        else break;
    }
    return false;
}

inline bool spielfeld::test_drohung(const char feld[], const int &farbe, const int &pos, const int &except_pos)  {
    if ((pos < 0) || (pos > 99)) {
        cout << pos << " ";
        return false;
    }

    return look_richtung_td(feld, farbe, pos, except_pos, 10) ||
           look_richtung_td(feld, farbe, pos, except_pos, -10) ||
           look_richtung_td(feld, farbe, pos, except_pos,  1) ||
           look_richtung_td(feld, farbe, pos, except_pos, -1) ||

           look_richtung_ld(feld, farbe, pos, except_pos,  9) ||
           look_richtung_ld(feld, farbe, pos, except_pos,  -9) ||
           look_richtung_ld(feld, farbe, pos, except_pos, 11) ||
           look_richtung_ld(feld, farbe, pos, except_pos, -11) ||

           (feld[pos + 21 * farbe] == W_P * farbe * -1) ||
           (feld[pos + 12 * farbe] == W_P * farbe * -1) ||
           (feld[pos + 19 * farbe] == W_P * farbe * -1) ||
           (feld[pos + 8  * farbe] == W_P * farbe * -1) ||
            (feld[pos - 21 * farbe] == W_P * farbe * -1) ||
            (feld[pos - 12 * farbe] == W_P * farbe * -1) ||
            (feld[pos - 19 * farbe] == W_P * farbe * -1) ||
            (feld[pos - 8  * farbe] == W_P * farbe * -1) ||
           (feld[pos + 9  * farbe] == W_B * farbe * -1) ||
           (feld[pos + 11 * farbe] == W_B * farbe * -1) ||

           (feld[pos + 11 * farbe] == W_K * farbe * -1 ||
            feld[pos + -11 * farbe] == W_K * farbe * -1 ||
            feld[pos + 1 * farbe] == W_K * farbe * -1 ||
            feld[pos + -1 * farbe] == W_K * farbe * -1 ||
            feld[pos + 10 * farbe] == W_K * farbe * -1 ||
            feld[pos + -10 * farbe] == W_K * farbe * -1 ||
            feld[pos + 9 * farbe] == W_K * farbe * -1 ||
            feld[pos + -9 * farbe] == W_K * farbe * -1 )

            || (feld[pos + 11 * farbe] == W_Kr * farbe * -1 ||
                feld[pos + -11 * farbe] == W_Kr * farbe * -1 ||
                feld[pos + 1 * farbe] == W_Kr * farbe * -1 ||
                feld[pos + -1 * farbe] == W_Kr * farbe * -1 ||
                feld[pos + 10 * farbe] == W_Kr * farbe * -1 ||
                feld[pos + -10 * farbe] == W_Kr * farbe * -1 ||
                feld[pos + 9 * farbe] == W_Kr * farbe * -1 ||
                feld[pos + -9 * farbe] == W_Kr * farbe * -1 )
                    ;
}

inline void spielfeld::find_kings()  {
    // setzt wking und bking auf entsprechende Werte

    wking = 0;
    bking = 0;
    int farbvorzeichen;
    int figur;
    int feld;

    for (int i = 21; i <= 98; i++) {
        feld = Feld[Stufe][i];
        figur = abs(feld);
        farbvorzeichen = (feld > 0) - (feld < 0);

        if ((figur == LEER) || (figur == RAND))
            continue;

        if ((figur == W_K) || (figur == W_Kr)) {
            if (farbvorzeichen > 0)
                wking = i;
            else
                bking = i;
        }
    }

    if (this->Farbe < 0) {
        my_king_pos = this->bking;
        op_king_pos = this->wking;
    } else {
        my_king_pos = this->wking;
        op_king_pos = this->bking;
    }
}


int pos1;
int enp_l, enp_r;
int ziel;
int zugnr = 0;
int en_passent_bauer = 0;
int o,p;
int feld;

int spielfeld::zuggenerator()  {
    char pos1, pos2;
    int enp_l, enp_r;
    char ziel, zielfeld;
    int farbvorzeichen, figur;
    int p;
    en_passent_bauer = 0;
    zugnr = 0;

    this->n_zuege = 0;
    // Variable der Klasse, wenn man es neu deklarieren wuerde,
    // kommt es zu einem seltsamen Fehler in der Zugsortierung,
    // weil n_zuege lokal dann 0 bleibt

    for (o = 0; o < 199; o++) zugstapel[Stufe][o].nw = 0;

    for (p = 21; p <= 98; p++) {
        figur = (abs(Feld[Stufe][p]));


        if ((figur == LEER) || (figur == RAND))

            continue;

        farbvorzeichen = figur / Feld[Stufe][p];

        pos1           = p;

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
                    add_verwandelung(farbvorzeichen, pos2 - 10 * farbvorzeichen, LEER, n_zuege);
                    add_verwandelung(farbvorzeichen, pos2, W_B, n_zuege);
                    add_zug(pos1, pos2, n_zuege, true, figur);
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
                                add_verwandelung(farbvorzeichen, pos2, W_B, n_zuege);
                            } else
                            if (figur == W_B)  { // Bauernumwandelung per schlag
                                if (Farbe > 0)  {
                                    if ((91 <= pos2) && (pos2 <= 98))  {
                                        add_verwandelung(farbvorzeichen, pos2, W_D, n_zuege);
                                        add_zug(pos1, pos2, n_zuege, true, W_D);
                                        add_verwandelung(farbvorzeichen, pos2, W_P, n_zuege);
                                        add_zug(pos1, pos2, n_zuege, true, W_P);

                                        //		break;
                                    }
                                } else
                                if (Farbe < 0)  {
                                    if ((21 <= pos2) && (pos2 <= 28))  {
                                        add_verwandelung(farbvorzeichen, pos2, W_D, n_zuege);
                                        add_zug(pos1, pos2, n_zuege, true, W_D);
                                        add_verwandelung(farbvorzeichen, pos2, W_P, n_zuege);
                                        add_zug(pos1, pos2, n_zuege, true, W_P);

                                        //		break;
                                    }
                                }
                            }
                            add_zug(pos1, pos2, n_zuege, true, figur);
                        }
                    }
                }

                // rechts vor
                pos2     = ziel = pos1 + farbvorzeichen * 11;
                zielfeld = Feld[Stufe][ziel];

                if  (figur == W_Bp_r)  {
                    add_verwandelung(farbvorzeichen, pos2 - 10 * farbvorzeichen, LEER, n_zuege);
                    add_verwandelung(farbvorzeichen, pos2, W_B, n_zuege);
                    add_zug(pos1, pos2, n_zuege, true, figur);
                } else

                if (((zielfeld = Feld[Stufe][ziel]) != RAND))   {
                    if (zielfeld != LEER)   { // schraeg schlagen
                        if (zielfeld == -1 * W_K * farbvorzeichen)      {
                            spezial = SCHACH; /*test = 1;*/
                            return 0;
                        }

                        if (zielfeld / abs(zielfeld) != farbvorzeichen)   {
                            spezial = UNRUH;

                            if (figur == W_Bx) add_verwandelung(farbvorzeichen, pos2, W_B, n_zuege);
                            else
                            if (figur == W_B)  { // Bauernumwandelung per schlag
                                if (Farbe > 0)  {
                                    if ((91 <= pos2) && (pos2 <= 98))  {
                                        add_verwandelung(farbvorzeichen, pos2, W_D, n_zuege);
                                        add_zug(pos1, pos2, n_zuege, true, figur);
                                        add_verwandelung(farbvorzeichen, pos2, W_P, n_zuege);
                                        add_zug(pos1, pos2, n_zuege, true, figur);

                                        //		break;
                                    }
                                } else
                                if (Farbe < 0)  {
                                    if ((21 <= pos2) && (pos2 <= 28))  {
                                        add_verwandelung(farbvorzeichen, pos2, W_D, n_zuege);
                                        add_zug(pos1, pos2, n_zuege, true, figur);
                                        add_verwandelung(farbvorzeichen, pos2, W_P, n_zuege);
                                        add_zug(pos1, pos2, n_zuege, true, figur);

                                        //		break;
                                    }
                                }
                            }
                            add_zug(pos1, pos2, n_zuege, true, figur);
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
                            spezial = SCHACH; /*test = 1;*/

                            return 0;
                        }



                        if ((figur == W_Kr) || (figur == W_K)) {
                            if (test_drohung(Feld[Stufe], Farbe, pos1, pos1)) {
                                test = 1;

                            }
                            if (test_drohung(Feld[Stufe], Farbe, pos2, 0))  {
                                test = 1;
                                continue;
                            }
                        }

                        if (figur == W_Kr) {
                            add_verwandelung(farbvorzeichen, pos2, W_K, n_zuege);
                        } else
                        if (figur == W_Tr) {
                            add_verwandelung(farbvorzeichen, pos2, W_T, n_zuege);
                        }



                        spezial = UNRUH;
                        add_zug(pos1, pos2, n_zuege, true, figur);

                        break;
                    }

                    // gehen
                    if (figur == W_B)  { // Bauernumwandelung
                        if (Farbe > 0)  {
                            if ((91 <= pos2) && (pos2 <= 98))  {
                                add_verwandelung(farbvorzeichen, pos2, W_D, n_zuege);
                                add_zug(pos1, pos2, n_zuege, false, figur);
                                add_verwandelung(farbvorzeichen, pos2, W_P, n_zuege);
                                add_zug(pos1, pos2, n_zuege, false, figur);

                                //		break;
                            }
                        } else
                        if (Farbe < 0)  {
                            if ((21 <= pos2) && (pos2 <= 28))  {
                                add_verwandelung(farbvorzeichen, pos2, W_D, n_zuege);
                                add_zug(pos1, pos2, n_zuege, false, figur);
                                add_verwandelung(farbvorzeichen, pos2, W_P, n_zuege);
                                add_zug(pos1, pos2, n_zuege, false, figur);

                                //		break;
                            }
                        }
                    } else

                    if ((figur == W_Bp_r) || (figur == W_Bp_l)) add_verwandelung(
                                farbvorzeichen,
                                pos2,
                                W_B,
                                n_zuege);
                    else
                    if (figur == W_Bx) {
                        add_verwandelung(farbvorzeichen, pos2, W_B, n_zuege);

                        if (weite == 1)  {
                            enp_r = Feld[Stufe][pos2 + farbvorzeichen];
                            enp_l = Feld[Stufe][pos2 - farbvorzeichen];

                            if (enp_r == -1 * W_B * farbvorzeichen)  {
                                add_verwandelung(farbvorzeichen * -1,
                                                 pos2 + farbvorzeichen,
                                                 W_Bp_r,
                                                 n_zuege);
                            }

                            if (enp_l == -1 * W_B * farbvorzeichen)  {
                                add_verwandelung(farbvorzeichen * -1,
                                                 pos2 - farbvorzeichen,
                                                 W_Bp_l,
                                                 n_zuege);
                            }
                        }
                    }

                    if ((figur == W_Kr) || (figur == W_K) ) {
                        if (test_drohung(Feld[Stufe], Farbe, pos1, pos1)) {
                            test = 1;

                        }
                        if (test_drohung(Feld[Stufe], Farbe, pos2, pos1))  {
                            //    test = 1;
                            continue;
                        }

                    }

                    if (figur == W_Kr) {
                        add_verwandelung(farbvorzeichen, pos2, W_K, n_zuege);
                    } else
                    if (figur == W_Tr) {
                        add_verwandelung(farbvorzeichen, pos2, W_T, n_zuege);
                    }

                    add_zug(pos1, pos2, n_zuege, false, figur);
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
                                  95, 95) &&
                    !test_drohung(Feld[Stufe], Farbe,
                                  96, 95) &&
                    !test_drohung(Feld[Stufe], Farbe,
                                       97, 95))  {
                    add_verwandelung(Farbe, 97, W_K, n_zuege);
                    add_verwandelung(Farbe, 96, W_T, n_zuege);
                    add_verwandelung(Farbe, 98, LEER, n_zuege);
                    add_zug(95, 97, n_zuege, false, figur);
                }
            }

            if ((Feld[Stufe][95] == S_Kr) && (Feld[Stufe][94] == LEER) &&
                (Feld[Stufe][93] == LEER) && (Feld[Stufe][92] == LEER) &&
                (Feld[Stufe][91] == S_Tr))  {
                if (!test_drohung(Feld[Stufe], Farbe,
                                  95, 95) &&
                    !test_drohung(Feld[Stufe], Farbe,
                                  94, 95) &&
                    !test_drohung(Feld[Stufe], Farbe, 93, 95))  {
                    add_verwandelung(Farbe, 93, W_K, n_zuege);
                    add_verwandelung(Farbe, 94, W_T, n_zuege);
                    add_verwandelung(Farbe, 91, LEER, n_zuege);
                    add_zug(95, 93, n_zuege, false, figur);
                }
            }
        } else
        if (Farbe > 0) {
            if ((Feld[Stufe][25] == W_Kr) && (Feld[Stufe][26] == LEER) &&
                (Feld[Stufe][27] == LEER) && (Feld[Stufe][28] == W_Tr))  {
                if (!test_drohung(Feld[Stufe], Farbe,
                                  25, 25) &&
                    !test_drohung(Feld[Stufe], Farbe,
                                  26, 25) &&
                    !test_drohung(Feld[Stufe], Farbe, 27, 25))  {
                    add_verwandelung(Farbe, 27, W_K, n_zuege);
                    add_verwandelung(Farbe, 26, W_T, n_zuege);
                    add_verwandelung(Farbe, 28, LEER, n_zuege);
                    add_zug(25, 27, n_zuege, false, figur);
                }
            }

            if ((Feld[Stufe][25] == W_Kr) && (Feld[Stufe][24] == LEER) &&
                (Feld[Stufe][23] == LEER) && (Feld[Stufe][22] == LEER) &&
                (Feld[Stufe][21] == W_Tr))  {
                if (!test_drohung(Feld[Stufe], Farbe,
                                  25, 25) &&
                    !test_drohung(Feld[Stufe], Farbe,
                                  24, 24) &&
                    !test_drohung(Feld[Stufe], Farbe, 23, 25))  {
                    add_verwandelung(Farbe, 23, W_K, n_zuege);
                    add_verwandelung(Farbe, 24, W_T, n_zuege);
                    add_verwandelung(Farbe, 21, LEER, n_zuege);
                    add_zug(25, 23, n_zuege, false, figur);
                }
            }
        }
    }

    // wenn ein en passent bauer gefunden wurde, dann musser bei allen zuegen,
    // ausser bei seinen eigenen im naechsten zug umgewandelt werden
    if (en_passent_bauer != 0)  {
        int q;

        for (q = 0; q < n_zuege; q++)  {
            figur = zugstapel[Stufe][q].z.pos.fig;

            if (!((figur  == W_Bp_l) || (figur  == W_Bp_r)))  {
                add_verwandelung(Farbe, en_passent_bauer, W_B, q);
            }
        }
    }
    return n_zuege;
}


inline denkpaar * spielfeld::makeZugstapel()  {
    find_kings();
    zuggenerator();
    return zugstapel[Stufe];
}

int schach_bewegung[15][15] = // Richtung, Weite, wohin[richtung]
        {
                { 0, 0, 0   },              // NIL!
                { 7, 0, 21, -21, 12, -12, 19, -19, 8, -8},               // Pferd
                { 7, 6, 1, -1, 9, -9, 10, -10, 11, -11},              // Dame;
        };

inline bool spielfeld::schach(int _farbe)  {
    /*	int pos1, pos2, farbvorzeichen;


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
                                            add_zug(pos1, pos2, n_zuege, true);
                                    break;
                                    }
                            add_zug(pos1, pos2, n_zuege);
                                    }
                            }
                    }
            return n_zuege;*/
    return 0;
}


void spielfeld::print_feld_computable()  {
    cout << "{";
    for (int i=0; i<120; i++) {
        cout << (int) Feld[Stufe][i];
        if (i != 119) {
            cout << ", ";
        }
    }
    cout << "}"<< "\n";
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
    print_feld_computable();
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
    cout << "n_zuege=" << n_zuege << endl;
    for (int i = 0; i < n_zuege; i++)  {
        cout << figuren_char[zugstapel[Stufe][i].z.pos.fig  + figurenanzahl] << ": "
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

#endif