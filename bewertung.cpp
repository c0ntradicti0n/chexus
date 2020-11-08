#include "intelligence.h"
#ifndef BEWERTUNG
#define BEWERTUNG
static void print_move(ostream& file, zug z, int stufe);

int wertung = 0;
char    dieses_feld;

inline float entwicklung(char * &feld, int &farbe)    {
    wertung = 0;

    for ( dieses_feld = 21; dieses_feld <= 98; dieses_feld++)  {
        if (feld[dieses_feld] == RAND || abs(feld[dieses_feld])>6) continue;
        if (feld[dieses_feld] == __STARTFELD[dieses_feld]) wertung += 2* __STARTPUNKTE[dieses_feld];        // -kdieses_feldngzone_ich[i]*10;	//4.1
        if (feld[dieses_feld] == __STARTFELDx[dieses_feld]) wertung -= 2 * __STARTPUNKTEx[dieses_feld];                                    // -kingzone_gegner[i]*10;//-kingzone_ich[i]*10;	//4.1
        //	else wertung -= 1 * __STARTPUNKTEx[i];}//-kingzone_gegner[i]*10;*/
        if (feld[dieses_feld] == __STARTFELDx2[dieses_feld]) wertung +=  __STARTPUNKTEx2[dieses_feld];  // +kingzone_ich[i]*10;	//1.17
        if (feld[dieses_feld] == __STARTFELDx3[dieses_feld]) wertung -=  __STARTPUNKTEx3[dieses_feld];  // -kingzone_gegner[i]*10;
        //REST PSQ
/*    if (feld[i] == __STARTFELDx4[i]) wertung += 0.55* __STARTPUNKTEx4[i];  // +kingzone_ich[i]*10;	//1.17
    if (feld[i] == __STARTFELDx5[i]) wertung -=  0.55* __STARTPUNKTEx5[i];//*/
        /* if (feld[i] == __STARTFELDx6[i]) wertung +=  __STARTPUNKTEx6[i];  // +kingzone_ich[i]*10;	//1.17
          if (feld[i] == __STARTFELDx7[i]) wertung -=  __STARTPUNKTEx7[i];
          if (feld[i] == __STARTFELDx8[i]) wertung +=  __STARTPUNKTEx8[i];  // +kingzone_ich[i]*10;	//1.17
          if (feld[i] == __STARTFELDx9[i]) wertung -=  __STARTPUNKTEx9[i];
          if (feld[i] == __STARTFELDx10[i]) wertung +=  __STARTPUNKTEx10[i];  // +kingzone_ich[i]*10;	//1.17
          if (feld[i] == __STARTFELDx11[i]) wertung -=  __STARTPUNKTEx11[i];//*/

    }
    return wertung;
}


inline float material(char * &feld, int & farbe)  {
    double wert = 0;
    int    figur;
    for (int i = 21; i <= 98; i++)     {
        figur = feld[i];

        if ((figur == RAND) || (figur == LEER)) continue;

        wert += farbe * figur;
    }

    return wert;
}

int i;
char pos2;
int figur;
int real_figur;
int farbvorzeichen;
int Attack = 0;
char schlagzone_ich[120] = {0};
char schlagzone_gegner[120] = {0};
int n_Dame      = -5;
int Attack_Dame = 0;
int zielfeld;
int zielfigur;
int n_Turm      = -10;
int Attack_Turm = 0;
int richtung;
int weite;
int n_Laeufer      = -15;
int Attack_Laeufer = 0;
int Attack_Pferd = 0;
int Attack_Bauer = 0;
int Attack_Koenig = 0;


inline int zuganzahl(char feld[120], int _eigene_farbe)  { // Zaehlt Zuege von
    // Offizieren und
    // Bauern inclusive
    // Deckung und
    // Schlagen,
    // bei Bauern jedoch kein Schlagen
    // , was auch unterschiedlich gewichtet werden kann

    Attack = 0;
    schlagzone_ich[120] = {0};
    schlagzone_gegner[120] = {0};
    int result = 0;
    n_Dame      = -5;
    Attack_Dame = 0;

    // disp(feld);
    for (i = 21; i <= 98; i++)    {
        real_figur = feld[i];

        if ((real_figur == LEER) || (real_figur == RAND))
            continue;

        figur = abs(real_figur);
        farbvorzeichen = (real_figur > 0) - (real_figur < 0);

        if ((figur == W_D)) {
            for (int richtung = 0; richtung <= bewegung[figur][0]; richtung++)  {
                for (int weite = 0; weite <= bewegung[figur][1]; weite++)  {
                    pos2 = i + farbvorzeichen * bewegung[figur][2 + richtung] * (weite + 1);
                    zielfeld = feld[pos2];

                    if (zielfeld != LEER)  {
                        if (zielfeld == RAND) // Aus!
                            break;
                        zielfigur = abs((zielfeld > 0) - (zielfeld < 0));

                        if (farbvorzeichen != _eigene_farbe)  {
                            if (zielfeld / _eigene_farbe > 0) {                // Gegner greift meine Figur an
                                if (schlagzone_gegner[pos2] != 1) schlagzone_gegner[pos2] = 1;
                                else Attack_Dame += KooIch; n_Dame = n_Dame + 1; // 12

                                if ((n_Dame > 8) && (n_Dame < 10))
                                    n_Dame += 0.5;

                                Attack_Dame +=
                                        (zielfigur * materialwert[zielfigur] - 40) * AttackIch;
                            }
                            else { Attack_Dame += DefIch1;
                                if (zielfigur < 6)
                                    Attack_Dame -= DefIch2;
                            }
                            // Gegner
                            // deckt
                            // seine
                            // Figuren
                            //    1
                        } else  {
                            if (zielfeld / _eigene_farbe < 0) {
                                // Ich
                                // greife
                                // Gegner
                                // an
                                if (schlagzone_ich[pos2] != 1) schlagzone_ich[pos2] = 1;
                                else Attack_Dame += KooEr;

                                if (n_Dame < 9) n_Dame = n_Dame + 1;  // 12

                                if ((n_Dame > 8) && (n_Dame < 10)) n_Dame += 0.5;

                                if (zielfigur == W_K || zielfigur == W_Kr) continue;
                                Attack_Dame += (zielfigur * materialwert[zielfigur]) /
                                               AttackEr;
                            }
                            else  if (zielfigur <
                                      10) Attack_Dame +=
                                                  (zielfigur * materialwert[zielfigur] - 40) / DefEr;
                        } // Ich decke meine Figuren
                        break;
                    }

                    if (farbvorzeichen != _eigene_farbe)  {

                        //         Attack_Dame += kingzone_ich[pos2] * Koenigsangriff_Er;

                        if (n_Dame < 9) n_Dame = n_Dame + 1;               // 12

                        if ((n_Dame > 8) && (n_Dame < 10)) n_Dame += 0.5;  // -12.5
                    } else  {
                        //  if (kingzone_gegner[pos2] == 1) cout << "HALLO" << kingzone_gegner[pos2] * Koenigsangriff_Ich << " \n_zuege";
                        //      Attack_Dame += kingzone_gegner[pos2] * Koenigsangriff_Ich;
                        if (n_Dame < 9) n_Dame = n_Dame + 1;               // 12

                        if ((n_Dame > 8) && (n_Dame < 10)) n_Dame += 0.5;
                    }
                }
            }
            n_Dame      *= farbvorzeichen;
            Attack_Dame *= farbvorzeichen;
            //  cout << MobDame * n_Dame + AttDame * Attack_Dame << "\n_zuege";
           result          += MobDame * n_Dame + AttDame * Attack_Dame;

            continue;
        }

        if ((figur == W_T) || (figur == W_Tr)) {
            n_Turm      = -10;
            Attack_Turm = 0;

            for (richtung = 0; richtung <= bewegung[figur][0]; richtung++)  {
                for (weite = 0; weite <= bewegung[figur][1]; weite++)  {
                    pos2 = i + farbvorzeichen * bewegung[figur][2 + richtung] * (weite + 1);
                    zielfeld = feld[pos2];

                    if (zielfeld != LEER)  {
                        if (zielfeld == RAND) // Aus!
                            break;

                        zielfigur = abs(zielfeld);


                        if (farbvorzeichen != _eigene_farbe)  {
                            if (zielfeld / _eigene_farbe > 0) { // Gegner greift meine Figur
                                // an
                                if (schlagzone_gegner[pos2] != 1) schlagzone_gegner[pos2] = 1;
                                else Attack_Turm += KooIch;
                                n_Turm += 3;

                                if ((n_Turm > 6) && (n_Turm < 11)) n_Turm += 2;

                                if ((n_Turm > 10) && (n_Turm < 14)) n_Turm += 1;

                                Attack_Turm +=
                                        (zielfigur * materialwert[zielfigur] - 40) * AttackIch;
                            }
                            else { Attack_Turm += DefIch1;

                                if (zielfigur < 6) Attack_Turm -= DefIch2;
                            }
                            // Gegner
                            // deckt
                            // seine
                            // Figuren
                            //    1
                        } else  {
                            if (zielfeld / _eigene_farbe < 0) {
                                // Ich
                                // greife
                                // Gegner
                                // an
                                if (schlagzone_ich[pos2] != 1) schlagzone_ich[pos2] = 1;
                                else Attack_Turm += KooEr;

                                if (n_Turm < 7) n_Turm += 3;

                                if ((n_Turm > 6) && (n_Turm < 11)) n_Turm += 2;

                                if ((n_Turm > 10) && (n_Turm < 14)) n_Turm += 1;

                                if (zielfigur == W_K || zielfigur == W_Kr) continue;
                                Attack_Turm += (zielfigur * materialwert[zielfigur]) /
                                               AttackEr;
                            }
                            else  if (zielfigur <
                                      10) Attack_Turm +=
                                                  (zielfigur * materialwert[zielfigur] - 40) / DefEr;
                        } // Ich decke meine Figuren
                        break;
                    }

                    if (farbvorzeichen != _eigene_farbe)  {
                        //         Attack_Turm += kingzone_ich[pos2] * Koenigsangriff_Er;

                        if (n_Turm < 7) n_Turm += 3;

                        if ((n_Turm > 6) && (n_Turm < 11)) n_Turm += 2;

                        if ((n_Turm > 10) && (n_Turm < 14)) n_Turm += 1;
                    }
                    else  {
                        //   Attack_Turm += kingzone_gegner[pos2] * Koenigsangriff_Ich;
                        if (n_Turm < 7) n_Turm += 3;

                        if ((n_Turm > 6) && (n_Turm < 11)) n_Turm += 2;

                        if ((n_Turm > 10) && (n_Turm < 14)) n_Turm += 1;
                    }
                }
            }
            n_Turm   *= farbvorzeichen; Attack_Turm *= farbvorzeichen;
           result       += MobTurm * n_Turm + AttTurm * Attack_Turm;

            continue;

        }

        if (figur == W_L) {
            n_Laeufer      = -15;
            Attack_Laeufer = 0;

            for (richtung = 0; richtung <= bewegung[figur][0]; richtung++)  {
                for (weite = 0; weite <= bewegung[figur][1]; weite++)  {
                    pos2 = i + farbvorzeichen * bewegung[figur][2 + richtung] * (weite + 1);
                    zielfeld = feld[pos2];

                    if (zielfeld != LEER)  {
                        zielfigur = abs(zielfeld);

                        if (zielfeld == RAND) // Aus!
                            break;


                        if (farbvorzeichen != _eigene_farbe)  {
                            if (zielfeld / _eigene_farbe > 0) { // Gegner greift meine Figur
                                // an
                                if (schlagzone_gegner[pos2] != 1)
                                    schlagzone_gegner[pos2] = 1;
                                else
                                    Attack_Laeufer += KooIch;

                                if (n_Laeufer == -15)
                                    n_Laeufer += 5;  // 12

                                if ((n_Laeufer > -11) && (n_Laeufer < 10))
                                    n_Laeufer += 4;

                                if ((n_Laeufer > 9) && (n_Laeufer < 16))
                                    n_Laeufer += 3;

                                if ((n_Laeufer > 15) && (n_Laeufer < 22))
                                    n_Laeufer += 2;

                                if (n_Laeufer > 21)
                                    n_Laeufer += 1;

                                Attack_Laeufer +=
                                        (zielfigur * materialwert[zielfigur] - 40) * AttackIch;
                            }
                            else { Attack_Laeufer += DefIch1;

                                if (zielfigur < 6) Attack_Laeufer -= DefIch2; } // Gegner
                            // deckt
                            // seine
                            // Figuren
                            //    1
                        } else  {
                            if (zielfeld / _eigene_farbe < 0) {                        // Ich
                                // greife
                                // Gegner
                                // an
                                if (schlagzone_ich[pos2] != 1) schlagzone_ich[pos2] = 1;
                                else Attack_Laeufer += KooEr;

                                if (n_Laeufer == -15) n_Laeufer += 5;  // 12

                                if ((n_Laeufer > -11) && (n_Laeufer < 10)) n_Laeufer += 4;

                                if ((n_Laeufer > 9) && (n_Laeufer < 16)) n_Laeufer += 3;

                                if ((n_Laeufer > 15) && (n_Laeufer < 22)) n_Laeufer += 2;

                                if (n_Laeufer > 21) n_Laeufer += 1;

                                if (zielfigur == W_K || zielfigur == W_Kr) continue;
                                Attack_Laeufer += (zielfigur * materialwert[zielfigur]) /
                                                  AttackEr;
                            }
                            else  if (zielfigur <
                                      10) Attack_Laeufer +=
                                                  (zielfigur * materialwert[zielfigur] - 40) / DefEr;
                        } // Ich decke meine Figuren
                        break;
                    }

                    if (farbvorzeichen != _eigene_farbe)  {
                        //              Attack_Laeufer += kingzone_ich[pos2] * Koenigsangriff_Er;

                        if (n_Laeufer == -15) n_Laeufer += 5;  // 12

                        if ((n_Laeufer > -11) && (n_Laeufer < 10)) n_Laeufer += 4;

                        if ((n_Laeufer > 9) && (n_Laeufer < 16)) n_Laeufer += 3;

                        if ((n_Laeufer > 15) && (n_Laeufer < 22)) n_Laeufer += 2;

                        if (n_Laeufer > 21) n_Laeufer += 1;
                    }
                    else  {
                        //    Attack_Laeufer += kingzone_gegner[pos2] * Koenigsangriff_Ich;
                        if (n_Laeufer == -15) n_Laeufer += 5;  // 12

                        if ((n_Laeufer > -11) && (n_Laeufer < 10)) n_Laeufer += 4;

                        if ((n_Laeufer > 9) && (n_Laeufer < 16)) n_Laeufer += 3;

                        if ((n_Laeufer > 15) && (n_Laeufer < 22)) n_Laeufer += 2;

                        if (n_Laeufer > 21) n_Laeufer += 1;
                    }
                }
            } n_Laeufer *= farbvorzeichen; Attack_Laeufer *= farbvorzeichen;

           result+= MobLau * n_Laeufer + AttLau * Attack_Laeufer;
        }

        if ((figur == W_P)) {
            Attack_Pferd = 0;

            for (richtung = 0; richtung <= bewegung[figur][0]; richtung++)  {
                for (weite = 0; weite <= bewegung[figur][1]; weite++)  {
                    pos2 = i + farbvorzeichen * bewegung[figur][2 + richtung] * (weite + 1);
                    zielfeld = feld[pos2];

                    /*    for (int richtung = 0; richtung <= bewegung[W_K][0]; richtung++)
                        {
                       for (int weite = 0; weite <= bewegung[W_K][1]; weite++)  {
                       pos2 = i + farbvorzeichen * bewegung[W_K][2+richtung] * (weite+1);
                       int zielfeldk = feld[pos2];
                       if (zielfeld == zielfeldk) n_zuege -= 400*farbvorzeichen;return n_zuege;}}*/

                    if (zielfeld != LEER)  {
                        zielfigur = abs(zielfeld);

                        if (zielfeld == RAND) // Aus!
                            break;

                        /*   if (abs(zielfeld) == W_K)
                            break;*/

                        if (farbvorzeichen != _eigene_farbe)  {
                            if (zielfeld / _eigene_farbe > 0) { // Gegner greift meine Figur
                                // an
                                if (schlagzone_gegner[pos2] != 1) schlagzone_gegner[pos2] = 1;
                                else Attack_Pferd += KooIch;

                                Attack_Pferd +=
                                        (zielfigur * materialwert[zielfigur] - 40) * AttackIch;
                            }
                            else { Attack_Pferd += DefIch1;

                                if (zielfigur < 6) Attack_Pferd -= DefIch2; } // Gegner
                            // deckt
                            // seine
                            // Figuren
                            //    1
                        } else  {
                            if (zielfeld / _eigene_farbe < 0) {                      // Ich
                                // greife
                                // Gegner
                                // an
                                if (schlagzone_ich[pos2] != 1) schlagzone_ich[pos2] = 1;
                                else Attack_Pferd += KooEr;

                                if (zielfigur == W_K || zielfigur == W_Kr) continue;
                                Attack_Pferd += (zielfigur * materialwert[zielfigur]) /
                                                AttackEr;
                            }
                            else  if (zielfigur <
                                      10) Attack_Pferd +=
                                                  (zielfigur * materialwert[zielfigur] - 40) / DefEr;
                        } // Ich decke meine Figuren
                        break;
                    }

                    /*    if (farbvorzeichen != _eigene_farbe)  {
                    //            Attack_Pferd += kingzone_ich[pos2] * Koenigsangriff_Er;

                        }
                        else  {
                   //    Attack_Pferd += kingzone_gegner[pos2] * Koenigsangriff_Ich;
                        }//*/
                }
            } Attack_Pferd *= farbvorzeichen;

           result+= AttSpr * Attack_Pferd;

            continue;
        }

        if (((figur == W_B) || (figur == W_Bx))) {
            Attack_Bauer = 0;
            Attack_Koenig = 0;

            for (richtung = 0; richtung <= bewegung[13][0]; richtung++)  {
                for (weite = 0; weite <= bewegung[13][1]; weite++)  {
                    pos2 = i + farbvorzeichen * bewegung[13][2 + richtung] * (weite + 1);
                    zielfeld = feld[pos2];

                    /*    for (int richtung = 0; richtung <= bewegung[W_K][0]; richtung++)
                        {
                       for (int weite = 0; weite <= bewegung[W_K][1]; weite++)  {
                       pos2 = i + farbvorzeichen * bewegung[W_K][2+richtung] * (weite+1);
                       int zielfeldk = feld[pos2];
                       if (zielfeld == zielfeldk) n_zuege -= 400*farbvorzeichen;return n_zuege;}}_*/

                    if (zielfeld != LEER)  {
                        if (zielfeld == RAND) // Aus!
                            break;

                        zielfigur = abs(zielfeld);

                        if (farbvorzeichen != _eigene_farbe)  {
                            if (zielfeld / _eigene_farbe > 0) { // Gegner greift meine Figur
                                // an
                                if (schlagzone_gegner[pos2] != 1) schlagzone_gegner[pos2] = 1;
                                else Attack_Bauer += KooIch;

                                Attack_Bauer +=
                                        (zielfigur * materialwert[zielfigur] - 40) * AttackIch;
                            }
                            else {
                                Attack_Bauer += DefIch1 / 2;
                            }                                   // Gegner deckt seine Figuren
                            //    1
                        } else  {
                            if (zielfeld / _eigene_farbe < 0) { // Ich greife Gegner an
                                if (schlagzone_ich[pos2] != 1) schlagzone_ich[pos2] = 1;
                                else Attack_Bauer += KooEr;

                                if (zielfigur == W_K || zielfigur == W_Kr) continue;
                                Attack_Bauer += (zielfigur * materialwert[zielfigur]) /
                                                AttackEr;
                            }
                            else {

                                if (zielfigur <
                                    10) Attack_Bauer +=
                                                (zielfigur * materialwert[zielfigur] - 40) /
                                                DefEr;
                            }
                        } // Ich decke meine Figuren
                        break;
                    }

                }
            } Attack_Bauer *= farbvorzeichen;

           result+= AttBau * Attack_Bauer;

            continue;
        }

        if (((figur == W_K) || (figur == W_Kr))) {
            Attack_Koenig = 0;

            // int KSafety = 0;

            for (richtung = 0; richtung <= bewegung[figur][0]; richtung++)  {
                for (weite = 0; weite <= bewegung[figur][1]; weite++)  {
                    pos2 = i + farbvorzeichen * bewegung[figur][2 + richtung] * (weite + 1);
                    zielfeld = feld[pos2];

                    /*    for (int richtung = 0; richtung <= bewegung[W_K][0]; richtung++)
                        {
                       for (int weite = 0; weite <= bewegung[W_K][1]; weite++)  {
                       pos2 = i + farbvorzeichen * bewegung[W_K][2+richtung] * (weite+1);
                       int zielfeldk = feld[pos2];
                       if (zielfeld == zielfeldk) n_zuege -= 400*farbvorzeichen;return n_zuege;}}*/

                    if (zielfeld != LEER)  {
                        zielfigur = abs(zielfeld);

                        if (zielfeld == RAND) // Aus!
                            break;

                        /*   if (abs(zielfeld) == W_K)
                                   break;*/
                        if (farbvorzeichen != _eigene_farbe)  {
                            /*if (OpenLines[i%10-2] == 1) KSafety -= 550;
                                        if (OpenLines[i%10] == 1) KSafety -= 550;
                                        if (OpenLines[i%10-1] == 1) KSafety -= 550;//*/
                            if (zielfeld / _eigene_farbe > 0) { // Gegner greift meine Figur an
                                if (schlagzone_gegner[pos2] != 1) schlagzone_gegner[pos2] = 1;
                                else Attack_Koenig += KooIch;
                                Attack_Koenig += (zielfigur * materialwert[zielfigur] - 40) * AttackIch;
                            }
                            else Attack_Koenig += DefIch1;
                            // Gegner deckt seine Figuren    1
                        } else  {
                            /*if (OpenLines[i%10-2] == 1) KSafety -= 550;
                                     if (OpenLines[i%10] == 1) KSafety -= 550;
                                     if (OpenLines[i%10-1] == 1) KSafety -= 550;//*/
                            if (zielfeld / _eigene_farbe < 0) { // Ich greife Gegner an
                                if (schlagzone_ich[pos2] != 1) schlagzone_ich[pos2] = 1;
                                else Attack_Koenig += KooEr;

                                if (zielfigur == W_K) continue;
                                Attack_Koenig += (zielfigur * materialwert[zielfigur]) / AttackEr;
                            }
                            else  if (zielfigur < 10) Attack_Koenig += (zielfigur * materialwert[zielfigur] - 40) / DefEr;
                        } // Ich decke meine Figuren
                        break;
                    }

                    /*     if (farbvorzeichen != _eigene_farbe)  {
                      Attack_Koenig += kingzone_ich[pos2] * Koenigsangriff_Er;

                         }
                         else  {
                           Attack_Koenig += kingzone_gegner[pos2] * Koenigsangriff_Ich;
                         }//*/
                }
            } Attack_Koenig *= farbvorzeichen;

            // KSafety *= farbvorzeichen;
            // cout << KSafety << "\n_zuege";
           result += AttKoe * Attack_Koenig; // + KSafety;
        }
    }

    return result;
}


static bool valid_move(zug z)  {
    return (z.pos.pos1>0) && (z.pos.pos1<120) && (z.pos.pos2>0) && (z.pos.pos2<120);
}

int zid;
int m = 0; // Wieviele Z¬∏ge sind umsortiert?
denkpaar temp;

static int sort(denkpaar _zugstapel[200], int _n, int _stufe)  { // Sortiert Zugstapel neu nach Schema
    m = 0;
    for (int j = 0; j < 200; j++)  {
        zid = sort_schema_bewertung[_stufe][j].z.id; // Was ist mein j.ter Zug im Schema?

        if (zid == 0) {
            break;
        }

        for (int i = 0; i < _n && m < _n; i++) {      // vergleichen mit zugstapel
            if (m==i)  {
                continue;
            }
            if (!valid_move(_zugstapel[i].z))
                cout << "sorting invalid move";
            if (_zugstapel[i].z.id == zid)  { // An welcher Position ist der im Zugstapel?
                temp          = _zugstapel[i];  // Vertausche den Zug
                _zugstapel[i] = _zugstapel[m];
                _zugstapel[m] = temp;
                m++;
                break; // und weiter...
            }
        }
    }
    return 0;
}

static inline int make_schema(denkpaar _zugstapel[200], int _n, int _stufe)  {
    for (int j = 0; j < (sortiertiefe - 1); j++)  {
        // Sortiertiefe == wieviele Züge sollen sortiert werden?
        if (best_one[_stufe].z.id == 0)
            // Gibt es
            // keinen
            // PV-Zug?
            break;

        if (sort_schema_bewertung[_stufe][j].z.id == 0)  {                // wenn es noch keinen vorsortieren Zug gibt:
            sort_schema_bewertung[_stufe][j]          = best_one[_stufe]; // nimm den PV-Zug
            sort_schema_bewertung[_stufe][j + 1].z.id = 0;
            // setze den
            // nächstsortierten
            // Zug auf 0
            break;
        }

        if (best_one[_stufe].z.id == sort_schema_bewertung[_stufe][j].z.id)  {
            // wenn
            // der
            // PV-Zug
            // mit
            // dem
            // vorsortierten
            // Zug
            // identisch
            // ist
            sort_schema_bewertung[_stufe][j].bewertung += 10;
            break;
        }

        /*cout << "Vorsortierung:" <<
           grundfeld_bezeichnungen[best_one[_stufe].z.pos.pos1] <<
           grundfeld_bezeichnungen[best_one[_stufe].z.pos.pos2] << "\n_zuege*/
        if (best_one[_stufe].bewertung >
            sort_schema_bewertung[_stufe][j].bewertung)  {
            // Wenn Bewertung des best_one besser --> einordnen
            temp                             = sort_schema_bewertung[_stufe][j];
            sort_schema_bewertung[_stufe][j] = best_one[_stufe];
            int i;

            for (i = 1; i < (sortiertiefe - j); i++)  {
                // Nach hinten schieben
                if (!sort_schema_bewertung[_stufe][j + i].z.id == 0)  {
                    sort_schema_bewertung[_stufe][j + i] = temp;
                    temp                                 =
                            sort_schema_bewertung[_stufe][j + i + 1];
                } else  {
                    sort_schema_bewertung[_stufe][j + i] = temp;
                    break;
                }
            }
            sort_schema_bewertung[_stufe][j + 1 + i].z.id = 0;
            break; // Abrechen, nachdem  e i n_zuege  zug eingeordnet ist.
            //	cout << best_one[_stufe].z.id << "\n_zuege";
        }        // */
    }

    for (int l = 0; l < 200; l++)  {
        if (sort_schema_bewertung[_stufe][l].z.id == 0)  {
            sort_schema[_stufe][l] = 0;
            break;
        }
        sort_schema[_stufe][l] = sort_schema_bewertung[_stufe][l].z.id;
    }
    return 0;
}

static int move_sort_schema(int c = 6)  {
    // Schema 2 Z¬∏ge nach vorn schieben
    for (int i = 0; i < c; i++)  {
        for (int e = 1; e < ende; e++)  {
            for (int j = 0; j < sortiertiefe; j++)  {
                sort_schema_bewertung[e - 1][j] = sort_schema_bewertung[e][j];
                // übernimm sort_schema-bewertung eine Stufe niedriger
                sort_schema[e - 1][j] = sort_schema[e][j];
            }
        }
    }
    return 0;
}


static bool valid_figure(zug z, char * feld, int stufe)  {
    bool result =
        Feld[stufe][z.pos.pos1] != LEER && Feld[stufe][z.pos.pos1] != RAND &&
                Feld[stufe][z.pos.pos2]!=RAND;
    if (!result)  {
        cout << "invalid:";
        cout<<  (int) z.pos.pos1 <<  (int) z.pos.pos2 << grundfeld_bezeichnungen[ Feld[stufe][z.pos.pos1] + figurenanzahl ]<< grundfeld_bezeichnungen[Feld[stufe][z.pos.pos2]+figurenanzahl];
        print_move(cout, z, stufe);
    }
    return result;
}


static void print_move(ostream& file, zug z, int stufe)  {
    if (valid_move(z))
        file << " " << figuren_intern[z.pos.fig + figurenanzahl] << grundfeld_bezeichnungen[z.pos.pos1] << grundfeld_bezeichnungen[z.pos.pos2];
    else
        file << "|";
}

template< class Ret, class Arg1, class ...Args >
auto curry(  Ret f(Arg1,Args...), Arg1 arg )
-> std::function< Ret(Args...) >
{
    return [=]( Args ...args ) { return f( arg, args... ); };
}

static void print_moves(vector<zug> beam, int stufe, ostream& file = cout)  {
    for_each(beam.begin(), beam.begin() + stufe, [&file, stufe](auto y){print_move(file, y, stufe);});
}


#endif
