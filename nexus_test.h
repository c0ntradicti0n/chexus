//
// Created by stefan on 06.10.20.
//

#ifndef NEXUS_NEXUS_TEST_H
#define NEXUS_NEXUS_TEST_H


#include "intelligence.h"

class nexus_test {

};

int patt_feld[120] =
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

int before_patt_feld[120] =
        { RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER, W_K,  LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, LEER, LEER,  LEER, LEER, LEER,  LEER,  LEER,  W_T,   RAND,
          RAND, LEER, S_K, LEER, LEER, LEER,  LEER,  LEER,  LEER,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND,
          RAND, RAND, RAND, RAND, RAND, RAND,  RAND,  RAND,  RAND,   RAND
        };
#endif //NEXUS_NEXUS_TEST_H
