/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP4                                       *
 *  Eric Beaudry - beaudry.eric@uqam.ca - ericbeaudry.ca *
 *  @author Besma Jabri                                  *
 *  Code permanent : JABB71560008                        *
 *  @author Othmane Azzouzi                              *
 *  Code permanent : AZZO27129703                        */

#if !defined(__AGENDA_H__)
#define __AGENDA_H__
#include <string>
#include "coordonnees.h"
#include "dateheure.h"
#include "arbremap.h"
#include "ArbreAVLIntervalle.h"
class Agenda{
  public:
    void creer(const std::string& id, const Coordonnees& c, int puissance);
    ArbreMap<std::string, int> trouver(const Coordonnees& c, int rayon=0, int puissancemin=0) const;
    DateHeure reserverI(const std::string& id, DateHeure date, int duree);
    DateHeure reserverC(const Coordonnees& c, int rayon, int puissancemin, DateHeure datemin, int duree, std::string& borneid);
    DateHeure chercherDateDisponible(const std::string& id, DateHeure date, int duree);
    DateHeure chercherDateDisponibleReserverC(const std::string& id, DateHeure date, int duree);


        private:
    struct DonneesBornes
    {
        int puissance;
        Coordonnees  coor;
        friend class Agenda;
        friend class main;
    };


public:
    ArbreMap<std::string,DonneesBornes> bornes;
    ArbreMap<std::string, ArbreMap<int,ArbreAVLIntervalle<int>>> dateReservee;
    friend class DateHeure;
};

#endif

