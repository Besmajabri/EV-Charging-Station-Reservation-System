/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP4                                       *
 *  Eric Beaudry - beaudry.eric@uqam.ca - ericbeaudry.ca *
 *  @author Besma Jabri                                  *
 *  Code permanent : JABB71560008                        *
 *  @author Othmane Azzouzi                              *
 *  Code permanent : AZZO27129703                        */

#include "agenda.h"
#include <cmath>
/**
 * cette fonction permet de creer les bornes de recharge.
 * @param id identifiant du borne de recharge
 * @param c  Coordonnees de la borne de recharge a creer.
 * @param puissance la puissance en Watt de la borne de recharge a creer.
 */
void Agenda::creer(const std::string& id, const Coordonnees& c, int puissance) {
    bornes[id];
    bornes[id].puissance = puissance;
    bornes[id].coor.longitude = c.longitude;
    bornes[id].coor.latitude = c.latitude;
}

/**
 * cette fonction permet de trouver les bornes dans le rayon donné
 * et ayant une puissance d'au moins puissmin.
 * @param c position a partir de laquelle on effectue la recherche.
 * @param rayon rayon des bornes recherchées.
 * @param puissancemin puissance minimale demandée.
 * @return la listes des bornes dans le rayon demandé et ayant une puissance d'au moins puissmin.
 */
ArbreMap<std::string, int> Agenda::trouver(const Coordonnees& c, int rayon, int puissancemin) const{
    ArbreMap<std::string, int> resultat;
    ArbreMap<std::string, DonneesBornes>::Iterateur i(bornes.debut());
    while ( i ) {
        if (i.valeur().puissance >= puissancemin && c.distance(i.valeur().coor) <= rayon) {
            resultat[i.cle()] = std::round(c.distance(i.valeur().coor));
        }
        ++i;
    }
    return resultat;
}
/**
 * Cette fonction permet de trouver la premiere date disponible dans la borne
 * d'identifiant id a partir d'une date minimale. (sans effectuer la reservation )
 * @param id id de la borne ou il faut effectuer la reservation.
 * @param date date minimale pour reserver.
 * @param duree la duree de la reservation.
 * @return retourne la premiere date possible.
 */
DateHeure Agenda::chercherDateDisponible(const std::string& id, DateHeure date, int duree){
    DateHeure dateDisponible;
    DateHeure Minuit;
    DateHeure dateTrouve;
    Minuit.jours = date.jours + 1;
    Minuit.minutes = 0;
    Minuit.heures = 0;
    Minuit.secondes = 0;
    int debut = date.transformerDateEnSecondes();
    int fin = debut + duree;
    ArbreAVLIntervalle<int>::Iterateur iter(dateReservee[id][date.jours].debut());

    if (dateReservee.vide()) {
        ArbreAVLIntervalle<int>::Iterateur iter(dateReservee[id][date.jours].rechercherEgalOuSuivant(iter.courant->debut));
    }
    while ( iter ) {
        if ((iter.courant->debut <= debut && debut < iter.courant->fin && fin > iter.courant->debut &&
             fin <= iter.courant->fin)
            || (debut >= iter.courant->debut && debut < iter.courant->fin && fin > iter.courant->debut &&
                fin >= iter.courant->fin)
            || (debut <= iter.courant->debut && debut < iter.courant->fin && fin > iter.courant->debut &&
                fin <= iter.courant->fin)
            || (debut <= iter.courant->debut && debut < iter.courant->fin && fin > iter.courant->debut &&
                fin >= iter.courant->fin)) {
            // reajuster de debut et fin a inserer
            debut = iter.courant->fin;
            fin = debut + duree;
        }
        ++iter;
        if (iter && debut < iter.courant->debut && fin <= iter.courant->debut ) {
            break;
        }
    };
    dateDisponible.jours = date.jours;
    dateDisponible.heures = debut / 3600;
    dateDisponible.minutes = (debut % 3600)/ 60;
    dateDisponible.secondes = (debut% 3600) %60;
    if ( debut <= 86400 && fin <= 86400 ) {
        return dateDisponible;
        // cas 2 reservation le lendemain
    } else if(debut >= 86400 && fin >= 86400 ) {
        return chercherDateDisponible(id,Minuit,duree);
        // cas 3 d'une duree avant minuit du jour courant et d'une deuxieme partie du jour suivant a partir du minuit.
    } else if (debut <= 86400 && fin >= 86400 ) {
        dateTrouve = chercherDateDisponible(id,Minuit,duree - (86400-debut));
        if (dateTrouve.transformerDateEnSecondes()== 0) {
            date.heures = debut / 3600;
            date.minutes = (debut % 3600)/ 60;
            date.secondes = (debut% 3600) %60;
            return date;
        } else {
            return chercherDateDisponible(id,Minuit,duree);
        }
    }
    if (debut >= 86400 && debut <=  89999) {
        dateDisponible.jours = date.jours + 1;
        dateDisponible.heures = 0;
        dateDisponible.minutes = (debut % 3600)/ 60;
        dateDisponible.secondes = (debut% 3600) %60;
        return dateDisponible;
    }
    return dateDisponible;
}
/**
 * Cette fonction permet de trouver la premiere date disponible dans la borne
 * d'identifiant id a partir d'une date minimale.
 * cette fonction permet aussi d'effectuer une reservation.
 * @param id id de la borne ou il faut effectuer la reservation.
 * @param date date minimale pour reserver.
 * @param duree la duree de la reservation.
 * @return retourne la date reservee .
 */
DateHeure Agenda::reserverI(const std::string& id, DateHeure date, int duree) {
    DateHeure dateZero; // l'heure zero de la journée suivante
    DateHeure dateTrouvee;
    dateZero.jours = date.jours + 1;
    dateZero.heures = 0;
    dateZero.minutes = 0;
    dateZero.secondes = 0;
    int debut = date.transformerDateEnSecondes();
    int fin = debut + duree;

    ArbreAVLIntervalle<int>::Iterateur iter(dateReservee[id][date.jours].debut());
    if (!iter) {
        // cas ou le jour ne contient aucune reservation
        dateReservee[id][date.jours].inserer(debut, fin);
    } else {
        // cas ou le jour contient des reservations
        while (iter) {
            if ((iter.courant->debut <= debut && debut < iter.courant->fin && fin > iter.courant->debut &&
                 fin <= iter.courant->fin)
                || (debut >= iter.courant->debut && debut < iter.courant->fin && fin > iter.courant->debut &&
                    fin >= iter.courant->fin)
                || (debut <= iter.courant->debut && debut < iter.courant->fin && fin > iter.courant->debut &&
                    fin <= iter.courant->fin)
                || (debut <= iter.courant->debut && debut < iter.courant->fin && fin > iter.courant->debut &&
                    fin >= iter.courant->fin)) {
                // reajuster de debut et fin a inserer
                debut = iter.courant->fin;
                fin = debut + duree;
            }
            ++iter;
            if (iter && debut < iter.courant->debut && fin <= iter.courant->debut) {
                break;
            }
        };
    }
    // cas 1 reservation dans le meme jour
    if ( debut <= 86400 && fin <= 86400 ) {
        dateReservee[id][date.jours].inserer(debut, fin);
        // cas 2 reservation le lendemain
    } else if(debut >= 86400 && fin >= 86400 ) {
        return reserverI(id, dateZero, duree);
        // cas 3 d'une duree avant minuit du jour courant et d'une deuxieme partie du jour suivant a partir du minuit.
    } else if (debut <= 86400 && fin >= 86400 ) {
        dateTrouvee = chercherDateDisponible(id,dateZero, duree - (86400-debut));
        if (dateTrouvee.transformerDateEnSecondes()== dateZero.transformerDateEnSecondes()) {
            dateReservee[id][dateZero.jours].inserer(0, fin - 86400);
            dateReservee[id][date.jours].inserer(debut, 86400);
            date.heures = debut / 3600;
            date.minutes = (debut % 3600)/ 60;
            date.secondes = (debut% 3600) %60;
            return date;
        } else {
            return reserverI(id, dateZero, duree);
        }
    }
    date.heures = debut / 3600;
    date.minutes = (debut % 3600)/ 60;
    date.secondes = (debut% 3600) %60;
    return date;
}
/**
 * cette fonction permet de reserver la borne située dans un rayon donné,
 * et ayant la puissance minimale donnée, qui est la première disponible
 * à partir de datemin pour la durée donnée.
 * @param c Coordonnees a partir de laquelle il faut trouver la borne
 * @param rayon rayon  donné
 * @param puissancemin puissance minimale cherchée
 * @param datemin date minimale de reservation;
 * @param duree duree de la reservation
 * @param borneid borne de recharge a reserver
 * @return retourne la borne reservee pour la duree donnee
 */
DateHeure Agenda::reserverC(const Coordonnees& c, int rayon, int puissancemin, DateHeure datemin, int duree, std::string& borneid){
    borneid = "?";
    DateHeure date;
    // 1. trouver les bornes ayant la puisssance min et situee dans un rayon donne
    ArbreMap<std::string, int> bornesDisponibles;
    bornesDisponibles = trouver(c,rayon,puissancemin);
    // 2. trouver la borne avec la date la plus proche disponible à partir de datemin pour la duree donnee.
    ArbreMap<std::string,DateHeure > datesDisponibles;
    ArbreMap<std::string, int>::Iterateur iter (bornesDisponibles.debut());
    while(iter) {
        date = chercherDateDisponible(iter.cle(),datemin,duree);
        datesDisponibles[iter.cle()].jours = date.jours;
        datesDisponibles[iter.cle()].heures = date.heures;
        datesDisponibles[iter.cle()].minutes = date.minutes;
        datesDisponibles[iter.cle()].secondes = date.secondes;
        ++iter;
    }
    ArbreMap<std::string, int>::Iterateur iter1 (bornesDisponibles.debut());
    ArbreMap<std::string,DateHeure>::Iterateur iter2 (datesDisponibles.debut());
    borneid = iter2.cle();
    date = iter2.valeur();
    int rayonProche = iter1.valeur();
    //trouver la borne située dans un rayon donné
    while (iter2 && iter1) {
        if ( iter2.valeur() < date ) {
            borneid = iter2.cle();
            date = iter2.valeur();
            rayonProche = iter1.valeur();
            //En cas d'égalité de date de disponibilité
        } else if (iter2.valeur() ==  date ) {
            if ( rayon - iter1.valeur() > rayon - rayonProche ) {
                borneid = iter2.cle();
                date = iter2.valeur();
                rayonProche = iter1.valeur();
            }
        }
        ++iter2;
        ++iter1;
    }
    // effectuer la reservation de cette date
    return reserverI(borneid,date,duree);
}



