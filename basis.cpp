#include "basis.h"
#include "spielfeld.h"
#include "bewertung.cpp"

static int bp (spielfeld & spiel, int farbe, int alpha, double beta, int stufe, int _stopp, /*int devwert, */int NullFlag) { 	// Bewertung, Planung

    if (NullFlag == 3) {
        spiel.Farbe = farbe; }
    else
        spiel.Farbe = farbe;//*/

    double wertung;


    srand (time(NULL));

    make_schema(zugstapel[spiel.getStufe()], spiel.n, stufe);

    spiel.makeZugstapel();
//HIER NICHT
    if (spiel.spezial == SCHACH) {
        return alpha; }

    sort(zugstapel[spiel.getStufe()], spiel.n, stufe);
    int n = spiel.n;  // Anzahl der Zuege
    int nn = 0;       // Anzahl der vom Schach her machbaren Zuege

    for (int i=0; i < n; i++) {

        testspiel[stufe]->copy(spiel);
        testspiel[stufe]->zug(zugstapel[spiel.getStufe()][i]);

        __end = spiel.last_moves();
        if ( __end  == nothing) {
            nn += 1; }

        aktueller_zug[stufe] = zugstapel[spiel.getStufe()][i];

        /* if (aktueller_zug[stufe].kill)
         // if ((spiel.spezial == UNRUH)&& (stufe==_stopp))
         stopp_summant = 2;
         else
         stopp_summant = 0;  // SchlagzÌge, weiterrechnen*/


        if (((stufe +1>= _stopp)||(stufe+1 >= ende))) {
            //    cout << "Stufe" << stufe << "\n";
            wertung = rand() % 3 - 1;

            // wertung = 0;
            /*	for (int o=0;o < 8;o++)
            {OpenLines[o]= 1;}*/

            wertung += (double) 1.5 *    material   (Feld[testspiel[stufe]->getStufe()], farbe); //8.75-9		90
            if (alpha < wertung*farbe+30) { //Lazy EVAL: 130 verlustfrei; 60 Speed
                //	cout << OpenLines[3] << "\n";
                wertung += (double) 1.55 *      entwicklung(Feld[testspiel[stufe]->getStufe()], farbe);		//0.375-0.4		-->160		1.6
                //	if (beta > wertung*-1*farbe-205){//Lazy EVAL
                wertung += (double) 0.065 *  zuganzahl  (Feld[testspiel[stufe]->getStufe()], farbe); //0,8;0.076
                //  		}//} //6.75

                //   wertung += abs(-devwert*farbe-wertung)*farbe*4;
            }

            //   int pruning = (beta-alpha)/5.5;

            if (((wertung*farbe > alpha  &&  wertung*-1*farbe <= beta-340/*405/(stopp+7-stufe)*/) &&aktueller_zug[stufe].kill && stufe < _stopp+5)/*&&NullFlag<3*/) { //TEST

                wertung = - bp(*testspiel[stufe], farbe*-1, -beta, -alpha, stufe + 1, _stopp, /*devwert, */1);

            }
            else {
                wertung = wertung * (farbe);
                if (farbe == 1 ) {
                    wertung -= 50; } //*/
                bewertet++;; } }



        else {




            if ((NullFlag==1) && (_stopp-stufe)>2) {

                int wertungn = 0;


                wertungn = - bp(*testspiel[stufe], farbe, -beta, -beta+1, stufe + 1, _stopp-2, /*devwert, */3);  //ergibt beta -1
                //    else wertungn = bp(*testspiel[stufe], -farbe, -beta, -beta+1, stufe + 1, _stopp-2, 3);

                if (wertungn >= beta && abs(beta)!=MAX_WERT) {
                    return beta; }


            }



            if (NullFlag==1) {
                if ((_stopp-stufe)>2) {
                    wertung = - bp(*testspiel[stufe], -farbe, -beta, -alpha, stufe + 1, _stopp, /*devwert, */4); }
                else
                    wertung = - bp(*testspiel[stufe], farbe*-1, -beta, -alpha, stufe + 1, _stopp, /*devwert, */1); }
            else
                wertung = - bp(*testspiel[stufe], farbe*-1, -beta, -alpha, stufe + 1, _stopp, /*devwert, */2);


            zugstapel[spiel.getStufe()][i].bewertung = wertung;//}



            /*  if (stufe == _stopp -3 &!(aktueller_zug[stufe+1].kill) ) {  //FUTILITY
           int wertungf = (double) 1.5 * material( Feld[testspiel[stufe]->getStufe()]); //8.75-9		90
           if (alpha > wertungf * farbe+145) { //190/440 Tiefe 3 vollständig, 155 Tiefe 3 Speed
               return alpha; }
       }*/
        }

        if (testspiel[stufe]->spezial == SCHACH) {

            testspiel[stufe]->spezial = NICHTS;
            continue; }

        zugstapel[spiel.getStufe()][i].bewertung = wertung;

        if (stufe==0 && _stopp==stopp) {
            cout << ""
                 << grundfeld_bezeichnungen[aktueller_zug[0].z.pos.pos1]
                 << " => "	<< grundfeld_bezeichnungen[aktueller_zug[0].z.pos.pos2]
                 << " Bewertung: "
                 << setw(5) << wertung
                 <<", Zug-ID "
                 << setw(6) << aktueller_zug[0].z.id << "\n";
            cout.flush(); }
        //else { if (stufe == 0) cout << "*" << flush;}

        if (wertung > alpha) {

            bester_zug[stufe] = zugstapel[spiel.getStufe()][i];
            best_one[stufe] = zugstapel[spiel.getStufe()][i]; //Aktueller PV-Zug
            best_one[stufe].bewertung *= 0.5; //ACHTUNG 5

            if (wertung >= beta) {
                // best_one[stufe].bewertung *= 5;

                spiel.nn = nn;
                return beta; }

            alpha = wertung; } }
    spiel.nn = nn;
    return alpha; }