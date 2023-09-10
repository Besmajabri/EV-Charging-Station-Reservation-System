/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Été 2022 / TP4
 *  http://ericbeaudry.uqam.ca/INF3105/tp4/
 *
 *  En principe, vous ne devriez pas toucher à ce fichier. Mais, vous demeurez
 *  libre de le modifier si vous juger cela pertinent ou requis.
 *  Dans tous les cas, vous devez remettre ce fichier.
 */
#include <fstream>
#include <iostream>
#include "dateheure.h"
#include "coordonnees.h"
#include "agenda.h"

using namespace std;

int tp4(istream& entree);

int main(int argc, const char** argv){
    if(argc>1){
        // Lecture depuis un fichier.
        std::ifstream entreefichier(argv[1]);
        if(entreefichier.fail()){
            cerr << "Erreur d'ouverture du fichier '" << argv[1] << "'!" << std::endl;
            return 1;
        }
        return tp4(entreefichier);
    }else{
        // Lecture depuis l'entrée standard (stdin).
        return tp4(cin);
    }
}

int tp4(istream& entree){
    Agenda agenda;
    
    while(entree){
        string cmd;
        entree >> cmd;
        if(entree.fail() || cmd=="")
            break;
        
        if(cmd=="creer"){
            string id;
            Coordonnees position;
            float puissance=0;
            char unite=0, pointvirgule=0;
            entree >> id >> position >> puissance >> unite;
            if(unite=='k'){
                puissance*=1000;
                entree >> unite;
            }
            entree >> pointvirgule;
            if(unite!='W' || pointvirgule!=';')
                cout << "Erreur format commande creer" << endl;
            else if(puissance==0)
                cout << "Erreur puissance" << endl;
            else{
                agenda.creer(id, position, (int) puissance);

                cout << "OK" << endl;
            }

        }
        
        else if(cmd=="trouver"){
            Coordonnees position;
            float rayon, puissancemin;
            char unite1=0, unite2=0, pointvirgule=0;
            entree >> position >> rayon >> unite1;
            if(unite1=='k'){
                rayon*=1000;
                entree >> unite1;
            }
            entree >> puissancemin >> unite2;
            if(unite2=='k'){
                puissancemin*=1000;
                entree >> unite2;
            }
            entree >> pointvirgule;
            if(unite1!='m' || unite2!='W' || pointvirgule!=';')
                cout << "Erreur format commande creer" << endl;
            else{
                ArbreMap<string, int> resultat =
                agenda.trouver(position, (int) rayon, (int) puissancemin);
                // Les lignes suivante on besoin du Lab8
                auto iter=resultat.debut();
                while(iter){
                    cout << iter.cle() << " " << iter.valeur() << "m";
                    if(++iter)
                        cout << ", ";
                }
                cout << ";" << endl;
            }
            
        }
        
        else if(cmd=="reserverI"){
            std::string id;
            DateHeure datemin;
            int duree=0;
            char pointvirgule=0;
            entree >> id >> datemin >> duree >> pointvirgule;
            if(pointvirgule!=';')
                cout << "Erreur format reserverI !" << endl;
            else{
                DateHeure date = agenda.reserverI(id, datemin, duree);
                cout << date << endl;
            }
        }
        
        else if(cmd=="reserverC"){
            Coordonnees position;
            float rayon, puissancemin;
            char unite1=0, unite2=0, pointvirgule=0;
            entree >> position >> rayon >> unite1;
            if(unite1=='k'){
                rayon*=1000;
                entree >> unite1;
            }
            entree >> puissancemin >> unite2;
            if(unite2=='k'){
                puissancemin*=1000;
                entree >> unite2;
            }
            DateHeure datemin;
            int duree;
            entree >> datemin >> duree >> pointvirgule;
            if(unite1!='m' || unite2!='W' || pointvirgule!=';')
                cout << "Erreur format commande creer" << endl;
            else{
                std::string borneid;
                DateHeure date = agenda.reserverC(position, rayon, puissancemin, datemin, duree, borneid);
                cout << borneid << " " << date << endl;
            }
        }
                
        else{
            cout << "Commande invalidate : " << cmd << endl;
            return 1;
        }
    }
    
    return 0;
}
