//
// Created by stefan on 06.10.20.
//

#ifndef NEXUS_NEXUS_TEST_H
#define NEXUS_NEXUS_TEST_H


#include "intelligence.h"

class nexus_test {

};

char schach_feld[120] =
        { RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, W_K,  LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, S_K,  LEER,  LEER, LEER, LEER,  W_T,   LEER,  LEER,   RAND,
          RAND, LEER,  LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND
        };

char patt_feld[120] =
        { RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, W_K,  LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, W_T,  LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, S_K,  LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND
        };

char schach_feld_mittelspiel[120] =
        { RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, W_K,  LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, S_K,  LEER,  LEER, LEER, LEER,  W_T,   LEER,  LEER,   RAND,
          RAND, LEER,  LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND
        };

char before_patt_feld[120] =
        { RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, W_K,  LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  W_T,    RAND,
          RAND, LEER, S_K,  LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND
        };

static char feld_abzug_dame_material[120] = {13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 12, 6, 0, 9, 11, 0, 6, 12, 13, 13, 1, 1, 1, 0, 0, 1, 1, 1, 13, 13, 0, 0, 0, 0, 0, 0, 0, 0, 13, 13, 0, 0, 0, 5, 5, 0, 0, 0, 13, 13, 0, 7, 0, 0, 0, 0, 7, 0, 13, 13, 0, 0, -6, 0, -5, -6, 0, 0, 13, 13, -1, -1, -1, -1, 0, -1, -1, -1, 13, 13, -12, 0, -7, -9, -11, -7, 0, -12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13};
static char schach_rochade_koenig_gabel[120] = {13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 0, 0, 0, 10, 0, 0, 0, 0, 13, 13, 0, 1, 1, 0, 0, 0, 7, 1, 13, 13, 5, 0, 0, 0, 0, 0, 0, 0, 13, 13, 0, 0, 0, 5, 0, 0, -5, 0, 13, 13, -5, -7, 0, 0, 5, -5, 0, -5, 13, 13, 0, -5, 0, 0, -5, 0, 9, 0, 13, 13, 0, 0, 0, 0, 0, 0, 0, -8, 13, 13, 0, 0, 0, 0, -11, 0, 0, 0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13};
static char moves_into_check[120] = {13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 12, 0, 0, 9, 10, 0, 6, -6, 13, 13, 1, 1, 0, 0, 0, 0, 7, 0, 13, 13, 0, 0, 0, 0, 0, 5, 0, 5, 13, 13, 0, 0, 5, 5, 0, 0, 0, 0, 13, 13, 0, 0, 0, -5, 0, -7, 0, 0, 13, 13, 0, 0, -6, -7, -5, 0, 0, -5, 13, 13, -1, -1, 0, -9, 0, -1, 0, 0, 13, 13, 6, 0, -10, 0, -8, 0, 0, 0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13};
static char pointless_move_when_check[120] = {13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 0, 0, 0, 0, 0, 10, 0, 0, 13, 13, 0, 0, 0, 0, -8, 0, 0, 0, 13, 13, 0, 0, 0, 0, 0, -5, 0, 5, 13, 13, 0, 5, 0, 0, 5, 0, 0, 0, 13, 13, 0, 0, 0, 0, 0, 0, 7, 0, 13, 13, -5, 0, -5, 0, 0, 6, 0, 0, 13, 13, 8, 0, 0, 0, 0, 0, 0, 0, 13, 13, 0, 0, 0, 0, 0, -10, 0, 0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13};
static char stupid_move_of_queen[120] = {13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 0, 0, 0, 0, 11, 0, 0, 0, 13, 13, 9, 0, 0, 0, 0, 1, 0, 0, 13, 13, -5, 0, 0, 0, 5, 0, 0, 0, 13, 13, 0, 0, 0, 0, -7, 0, 0, 0, 13, 13, 0, 0, -7, 0, 0, 0, 0, 5, 13, 13, 0, 0, 0, -9, 0, 0, 0, -5, 13, 13, 0, 0, 0, 0, 0, -1, -1, -10, 13, 13, 0, 0, 8, 0, 0, 0, 0, 0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13};

static char feld_schach_in_grundfeld[120] =
        { RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, W_Tr, W_P,  W_L,  S_D,  W_Kr,  W_L,   W_P,   W_Tr,   RAND,
          RAND, W_Bx, W_Bx, W_Bx, W_Bx, W_Bx,  W_Bx,  W_Bx,  W_Bx,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, S_Bx, S_Bx, S_Bx, S_Bx, S_Bx,  S_Bx,  S_Bx,  S_Bx,   RAND,
          RAND, S_Tr, S_P,  S_L,  S_D,  S_Kr,  S_L,   S_P,   S_Tr,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND }; // */

          #endif //NEXUS_NEXUS_TEST_H
