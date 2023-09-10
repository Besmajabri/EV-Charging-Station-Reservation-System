/*  INF3105 - Structures de données et algorithmes       *
 *  UQAM / Département d'informatique                    *
 *  Été 2022 / TP4                                       *
 *  Eric Beaudry - beaudry.eric@uqam.ca - ericbeaudry.ca *
 *  @author Besma Jabri                                  *
 *  Code permanent : JABB71560008                        *
 *  @author Othmane Azzouzi                              *
 *  Code permanent : AZZO27129703                        */

#if !defined(__DATEHEURE_H__)
#define __DATEHEURE_H__

#include <iostream>

class DateHeure{
private:
    int jours = 0;
    int heures = 0;
    int minutes = 0;
    int secondes = 0;
  public:
    DateHeure();
    int transformerDateEnSecondes();
    bool operator<(const DateHeure& dh) const;
    bool operator==(const DateHeure& dh) const;
    void operator=(const DateHeure& dh);
    bool operator!=(const DateHeure& dh) const;


    friend std::ostream& operator << (std::ostream&, const DateHeure& dh);
    friend std::istream& operator >> (std::istream&, DateHeure& dh);
    friend class Agenda;
};


#endif
