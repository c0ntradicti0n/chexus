//
// Created by stefan on 06.10.20.
//

#include <iostream>

#include "gtest/gtest.h"

#include "nexus_test.h"
#include "intelligence.cpp"
#include "bewertung.cpp"
#include "spielfeld.cpp"

string get_current_test_name()  {
    return ::testing::UnitTest::GetInstance()->current_test_info()->name();
}

ofstream * init_tree_file()  {
    ofstream * file = new ofstream(get_current_test_name() + "moves.tree", ofstream::binary);
    tree_file = file;
    return file;
}

string make_move_patt ()  {
    init_test_spiel_array();

    spielfeld* spiel = new spielfeld(before_patt_feld, eigene_farbe = 1);

    spiel->makeZugstapel();

    cout<<"zugstapel " << spiel->Farbe << endl;
    spiel->print_zugstapel();

    int wert = run_speaking(4, *spiel);
    cout << "wert =" << wert << endl;
    return grundfeld_bezeichnungen[bester_zug[0].z.pos.pos1] + grundfeld_bezeichnungen[bester_zug[0].z.pos.pos2];
}


vector<string> moves_before = *new vector<string>();

TEST(module_name, test_before_patt_stupid_rand_move )  {

    ofstream * file = init_tree_file();
    tree_file = file;
    string move = make_move_patt();
    string stupid  = "RANDRAND";
    file->close();
    ASSERT_EQ(move != stupid, true);
}



TEST(module_name, test_before_patt_stupid_move )  {
    string move = make_move_patt();
    string stupid  = "c6d7";
    ASSERT_EQ(move != stupid, true);
}


// Google Test test cases are created using a C++ preprocessor macro
// Here, a "test suite" name and a specific "test name" are provided.


TEST(module_name, test_patt_is_white_black_move_perspective) {
    init_test_spiel_array();

    spielfeld* spiel = new spielfeld(patt_feld,

                                     eigene_farbe=1);
    spiel->switch_feld();

    spiel->disp();

    denkpaar * z = spiel->makeZugstapel();
    int end = spiel->check_end(moves_before);
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
    ASSERT_EQ(wert, LOST);
}

TEST(module_name, test_patt_black) {
    init_test_spiel_array();

    spielfeld* spiel = new spielfeld(patt_feld, eigene_farbe=-1);
    denkpaar * z = spiel->makeZugstapel();
    int end = spiel->check_end(moves_before);
    std::cout << end << std::endl;
    ASSERT_EQ(PATT * -1, end);
}


TEST(module_name, test_patt_is_black_white_move_perspective) {
    init_test_spiel_array();

    spielfeld* spiel = new spielfeld(patt_feld, eigene_farbe=1);
    denkpaar * z = spiel->makeZugstapel();
    int end = spiel->check_end(moves_before);
    std::cout << end << std::endl;
    ASSERT_EQ(NORMAL, end);
}

TEST(module_name, test_patt_zuege )  {
    int farbe = 1;
    int max_stufe = 4;

    spielfeld* spiel = new spielfeld(patt_feld, eigene_farbe = farbe);
    spiel->makeZugstapel();

    cout<<"zugstapel " << spiel->Farbe << endl;
    spiel->print_zugstapel();

    ofstream * file = init_tree_file();
    int wert = bp(*spiel, 1,  -MAX_WERT, MAX_WERT, 0, max_stufe, 1);
    for (int i=0; i<=15; i++)  {
        Beam[i] = bester_zug[i].z;
    }
    graph_debug(farbe, 0, 0, max_stufe, wert, "PIVOT");
    file->close();
    ASSERT_EQ(wert, WON);
}



TEST(module_name, test_before_patt_b )  {
    ofstream * file = init_tree_file();

    spielfeld* spiel = new spielfeld(before_patt_feld, eigene_farbe = -1);
    spiel->makeZugstapel();

    cout<<"zugstapel " << spiel->Farbe << endl;
    spiel->print_zugstapel();

    int wert = run_speaking(8, *spiel);
    file->close();
    ASSERT_EQ(wert, LOST);
}



// Google Test can be run manually from the main() function
// or, it can be linked to the gtest_main library for an already
// set-up main() function primed to accept Google Test test cases.
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}