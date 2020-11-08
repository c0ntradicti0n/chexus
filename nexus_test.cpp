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

tuple<string, int> compute_move(spielfeld  * spiel)  {
    spiel->makeZugstapel();
    cout<<"Zugstapel for " << spiel->Farbe << endl;
    spiel->print_zugstapel();

    int wert = run_speaking(STOPP, *spiel);
    return make_tuple(
            grundfeld_bezeichnungen[bester_zug[0].z.pos.pos1] + grundfeld_bezeichnungen[bester_zug[0].z.pos.pos2],
            wert);
}

string make_move(char * feld, bool switch_farbe, int farbe)  {
    init_test_spiel_array();
    ofstream * file = init_tree_file();

    spielfeld* spiel = new spielfeld(feld, farbe);
    if (switch_farbe) {
        spiel->switch_feld();
    }
    spiel->disp();
    string move;
    int wert;
    tie(move, wert) = compute_move(spiel);
    cout << "wert =" << wert << endl;
    //graph_debug(-farbe, 0, 0, level, wert, "PIVOT");

    file->close();
    spiel->disp();
    return move;
}

string play_to_end(char * feld, int farbe)  {
    init_test_spiel_array();
    ofstream * file = init_tree_file();
    spielfeld* spiel = new spielfeld(feld, farbe);

    string move;
    int wert;
    while (true) {

        tie(move, wert) = compute_move(spiel);
        spiel->make_move_real(move);
        spiel->disp();
        if (ENDS.find(wert) != ENDS.end())  {
            break;
        }
    }

    cout << "# finished game with " << wert << "(" << END_NAMES[wert] << ")" << endl;
}



string make_move_patt (bool switch_farbe=false)  {
    return make_move(before_patt_feld, switch_farbe, 1);
}

vector<string> moves_before = *new vector<string>();

TEST(module_name, test_before_patt_stupid_rand_move )  {

    ofstream * file = init_tree_file();
    tree_file = file;
    string move = make_move_patt(false);
    string stupid  = "RANDRAND";
    file->close();
    ASSERT_EQ(move != stupid, true);
}



TEST(module_name, test_before_patt_stupid_move )  {
    string move = make_move_patt();
    ASSERT_EQ(move[0] != 'c', true);
}

TEST(module_name, test_before_patt_stupid_move_white )  {
    string move = make_move_patt(true);
    ASSERT_EQ(move[0] != 'f', true);
}

TEST(module_name, test_schach_abzug_material )  {
    string move = make_move(feld_abzug_dame_material, false, -1);
    ASSERT_EQ(move != "f6e4", true);
}

TEST(module_name, test_stupid_queen_move )  {
    string move = make_move(stupid_move_of_queen, false, 1);
    ASSERT_EQ(move != "a2d2", true);
}

TEST(module_name, test_stupid_queen_move2 )  {
    string move = make_move(stupid_move_of_queen2, false, -1);
    ASSERT_EQ(move != "g2f1", true);
}
TEST(module_name, test_stupid_queen_move3 )  {
    string move = make_move(stupid_move_of_queen3, false, 1);
    ASSERT_EQ(move != "d4f6", true);
}

TEST(module_name, test_schach_pointless_move_when_check )  {
    string move = make_move(pointless_move_when_check, false, -1);
    ASSERT_EQ(move != "e2e1", true);
}



TEST(module_name, test_move_into_check )  {
    string move = make_move(moves_into_check, false, 1);
    ASSERT_EQ(move[0] != 'e', true);
}

/*
TEST(module_name, play_some_game_to_end )  {
    int save_stopp = STOPP;
    STOPP = 5;
    play_to_end(moves_into_check, 1);
    STOPP = save_stopp;
}
*/

TEST(module_name, test_schach_rochade_koenig_gabel )  {
    string move = make_move(schach_rochade_koenig_gabel, false, -1);
    ASSERT_EQ(move != "b5e2", true);
    ASSERT_EQ(move[0] == 'h', true);
}


TEST(module_name, test_schach_schach_in_grundfeld )  {
    string move = make_move(feld_schach_in_grundfeld, false, 1);
    ASSERT_EQ(move == string("e1d1"), true);
}


TEST(module_name, test_schach_schach_in_grundfeld_stufe_4 )  {
    string move = make_move(feld_schach_in_grundfeld, false, 1);
    ASSERT_EQ(move == string("e1d1"), true);
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
    ofstream * file = init_tree_file();


    spielfeld* spiel = new spielfeld(before_patt_feld, eigene_farbe = -1);
    spiel->disp();
    spiel->switch_feld();
    spiel->disp();

    spiel->makeZugstapel();

    cout<<"zugstapel " << spiel->Farbe << endl;
    spiel->print_zugstapel();

    int wert = run_speaking(STOPP, *spiel);
    file->close();

    ASSERT_EQ(wert, LOST);
}

TEST(module_name, test_patt_black) {
    init_test_spiel_array();

    spielfeld* spiel = new spielfeld(patt_feld, eigene_farbe=-1);
    denkpaar * z = spiel->makeZugstapel();
    int end = spiel->check_end(moves_before);
    std::cout << end << std::endl;
    ASSERT_EQ(PATT, end);
}



TEST(module_name, test_patt_is_black_white_move_perspective) {
    init_test_spiel_array();

    spielfeld* spiel = new spielfeld(patt_feld, eigene_farbe=1);
    denkpaar * z = spiel->makeZugstapel();
    int end = spiel->check_end(moves_before);
    std::cout << end << std::endl;
    ASSERT_EQ(NORMAL, end);
}


TEST(module_name, test_schach_black) {
    init_test_spiel_array();

    spielfeld* spiel = new spielfeld(schach_feld, eigene_farbe=-1);
    denkpaar * z = spiel->makeZugstapel();
    int end = spiel->check_end(moves_before);
    std::cout << end << std::endl;
    ASSERT_EQ(NORMAL, end);
}


TEST(module_name, test_patt_zuege )  {
    init_test_spiel_array();

    int farbe = 1;

    spielfeld* spiel = new spielfeld(patt_feld, eigene_farbe = farbe);
    spiel->makeZugstapel();

    cout<<"zugstapel " << spiel->Farbe << endl;
    spiel->print_zugstapel();

    ofstream * file = init_tree_file();
    spiel->disp();
    int wert = bp(*spiel, 1,  -MAX_WERT, +MAX_WERT, 0, STOPP, 0);
    for (int i=0; i<=15; i++)  {
        Beam[i] = bester_zug[i].z;
    }
    //graph_debug(-farbe, 0, 0, max_stufe, wert, "PIVOT");
    file->close();
    ASSERT_EQ(wert, WON);
}



TEST(module_name, test_before_patt_b )  {
    ofstream * file = init_tree_file();

    spielfeld* spiel = new spielfeld(before_patt_feld, eigene_farbe = -1);
    spiel->makeZugstapel();

    cout<<"zugstapel " << spiel->Farbe << endl;
    spiel->print_zugstapel();
    spiel->disp();

    int wert = run_speaking(STOPP, *spiel);
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