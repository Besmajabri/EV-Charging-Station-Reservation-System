/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP2 et TP4                                *
 *  Eric Beaudry - beaudry.eric@uqam.ca - ericbeaudry.ca *
 *  @author Besma Jabri                                  *
 *  Code permanent : JABB71560008                        *
 *  @author Othmane Azzouzi                              *
 *  Code permanent : AZZO27129703                        */

#if !defined(_COORDONNEES__H_)
#define _COORDONNEES__H_
#include <iostream>

class Coordonnees {
  public:
    Coordonnees(){}
    Coordonnees(double latitude_, double longitude_);
    Coordonnees(const Coordonnees&);
    double distance(const Coordonnees&) const;
  private:
    double latitude;
    double longitude;

  friend std::ostream& operator << (std::ostream&, const Coordonnees&);
  friend std::istream& operator >> (std::istream&, Coordonnees&);
  friend class Agenda;
};

#endif

