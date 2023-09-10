/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP4                                       *
 *  Eric Beaudry - beaudry.eric@uqam.ca - ericbeaudry.ca *
 *  @author Besma Jabri                                  *
 *  Code permanent : JABB71560008                        *
 *  @author Othmane Azzouzi                              *
 *  Code permanent : AZZO27129703                        */

#include "dateheure.h"
#include <cstdio>
#include <iomanip>
#include <assert.h>

#define MinutesParHeures 60
DateHeure::DateHeure():jours(0),heures(0),minutes(0),secondes(0){

}
// surchage de l'opérateur <
bool DateHeure::operator<(const DateHeure& dh) const{
    int dhUn = (this->jours * 86400) + (this->heures * 3600) + (this->minutes * 60) + this->secondes;
    int dhDeux = (dh.jours * 86400) + (dh.heures * 3600) + (dh.minutes * 60) + dh.secondes;
    if (dhUn == dhDeux || dhUn > dhDeux ) return false;
    return true;
}

// surchage de l'opérateur !=
bool DateHeure::operator!=(const DateHeure& dh) const {
    int dhUn = (this->jours * 86400) + (this->heures * 3600) + (this->minutes * 60) + this->secondes;
    int dhDeux = (dh.jours * 86400) + (dh.heures * 3600) + (dh.minutes * 60) + dh.secondes;
    return dhUn != dhDeux;
}
// surchage de l'opérateur ==
bool DateHeure::operator==(const DateHeure& dh) const {
    int dhUn = (this->jours * 86400) + (this->heures * 3600) + (this->minutes * 60) + this->secondes;
    int dhDeux = (dh.jours * 86400) + (dh.heures * 3600) + (dh.minutes * 60) + dh.secondes;
    return dhUn == dhDeux;
}
// surchage de l'opérateur =
void DateHeure::operator=(const DateHeure& dh) {
    this->jours = dh.jours;
    this->heures = dh.heures;
    this->minutes = dh.minutes;
    this->secondes = dh.secondes;
}
// surchage de l'opérateur <<
std::ostream& operator << (std::ostream& os, const DateHeure& dh){
    int jours=0, heures=0, minutes=0, secondes=0;
    jours = dh.jours;
    heures = dh.heures;
    minutes = dh.minutes;
    secondes = dh.secondes;
    char chaine[40];
    sprintf(chaine, "%d_%02d:%02d:%02d", jours, heures, minutes, secondes);
    os << chaine;
    return os;
}
// surchage de l'opérateur >>
std::istream& operator >> (std::istream& is, DateHeure& dh){
    int jours=0, heures=0, minutes=0, secondes=0;
    char seperateurlu1=0, seperateurlu2=0, seperateurlu3=0;
    is >> jours >> seperateurlu1 >> heures >> seperateurlu2 >> minutes >> seperateurlu3 >> secondes;
    assert(seperateurlu1=='_');
    assert(seperateurlu2==':');
    assert(seperateurlu3==':');
    dh.jours = jours;
    dh.heures = heures;
    dh.minutes = minutes;
    dh.secondes = secondes;
    return is;
}
// fnction qui transforme la date courante en secondes.
int DateHeure::transformerDateEnSecondes() {
    int nbSecondes = 0 ;
    nbSecondes = (this->heures *3600) + (this->minutes * 60 ) + this->secondes;
    return nbSecondes;
}



