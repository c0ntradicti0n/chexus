//
// Created by stefan on 06.10.20.
//


#include "gtest/gtest.h"

#include "nexus_test.h"
#include "intelligence.cpp"
#include "bewertung.cpp"
#include "spielfeld.cpp"


#include <iostream>

vector<string> moves_before = *new vector<string>();

// Google Test test cases are created using a C++ preprocessor macro
// Here, a "test suite" name and a specific "test name" are provided.


TEST(module_name, test_patt_is_white_black_move_perspective) {
    init_test_spiel_array();

    spielfeld* spiel = new spielfeld(patt_feld,

                                     eigene_farbe=1);
    spiel->switch_feld();

    spiel->disp();

    denkpaar * z = spiel->makeZugstapel();
    howitends end = spiel->check_end(moves_before);
    std::cout << end << std::endl;
    ASSERT_EQ(NORMAL, end);
}


TEST(module_name, test_before_patt_w )  {

    init_test_spiel_array();

    spielfeld* spiel = new spielfeld(before_patt_feld, eigene_farbe = -1);
    spiel->disp();
    spiel->switch_feld();
    spiel->disp();

    spiel->makeZugstapel();

    cout<<"zugstapel " << spiel->Farbe << endl;
    spiel->print_zugstapel();

    int wert = run_speaking(8, *spiel);
    ASSERT_EQ(wert, BLACK_SCHACHMATT);
}

TEST(module_name, test_patt_black) {
    init_test_spiel_array();

    spielfeld* spiel = new spielfeld(patt_feld, eigene_farbe=-1);
    denkpaar * z = spiel->makeZugstapel();
    howitends end = spiel->check_end(moves_before);
    std::cout << end << std::endl;
    ASSERT_EQ(PATT, end);
}


TEST(module_name, test_patt_is_black_white_move_perspective) {
    init_test_spiel_array();

    spielfeld* spiel = new spielfeld(patt_feld, eigene_farbe=1);
    denkpaar * z = spiel->makeZugstapel();
    howitends end = spiel->check_end(moves_before);
    std::cout << end << std::endl;
    ASSERT_EQ(NORMAL, end);
}

TEST(module_name, test_patt_zuege )  {
    spielfeld* spiel = new spielfeld(patt_feld, eigene_farbe = 1);
    spiel->makeZugstapel();

    cout<<"zugstapel " << spiel->Farbe << endl;
    spiel->print_zugstapel();

    int wert = bp(*spiel, 1,  -MAX_WERT, MAX_WERT, 0, 9, 1);
    ASSERT_EQ(wert, SCHACHMATT);
}



TEST(module_name, test_before_patt_b )  {

    spielfeld* spiel = new spielfeld(before_patt_feld, eigene_farbe = -1);
    spiel->makeZugstapel();

    cout<<"zugstapel " << spiel->Farbe << endl;
    spiel->print_zugstapel();

    int wert = run_speaking(10, *spiel);
    ASSERT_EQ(wert, BLACK_SCHACHMATT);
}



// Google Test can be run manually from the main() function
// or, it can be linked to the gtest_main library for an already
// set-up main() function primed to accept Google Test test cases.
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}