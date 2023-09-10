/* Squelette pour classe générique ArbreMap<K, V>.
 * Lab8 -- Dictionnaires et Arbres binaires de recherche
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes
 * http://ericbeaudry.uqam.ca/INF3105/lab8/
 *  @author Besma Jabri                                  *
 *  Code permanent : JABB71560008                        *
 *  @author Othmane Azzouzi                              *
 *  Code permanent : AZZO27129703                        */

#if !defined(ARBREMAP_H)
#define ARBREMAP_H
#include "arbreavl.h"

template <class K, class V>
class ArbreMap {

    class Entree
    {
    public:
        Entree(const K&c):cle(c),valeur(){}
        K cle;
        V valeur;
        bool operator<(const Entree& e) const { return cle < e.cle; }
        bool operator!=(const Entree& e) const { return cle != e.cle; }

    };
    ArbreAVL<Entree> entrees;

public:
    bool contient(const K&) const;
    void enlever(const K&);
    void vider();
    bool vide() const;
    const V& operator[] (const K&) const;
    V& operator[] (const K&);

public:
    class Iterateur {
    public:
        Iterateur(const ArbreMap& a)
                :iter(a.entrees.debut())
        {}
        Iterateur(typename ArbreAVL<Entree>::Iterateur i)
                :iter(i)
        {}
        operator bool() const
        {
            return iter.operator bool();
        };

        bool operator!() const {
            return iter.operator!();
        };

        Iterateur& operator++()
        {
            ++iter;
            return *this;
        };


        const K& cle() const
        {
            return (*this).iter.operator*().cle;
        };

        V& valeur() const
        {
            return (V&) (*this).iter.operator*().valeur;
        };

    private:
        typename ArbreAVL<Entree>::Iterateur iter;

    };
    Iterateur debut() const { return Iterateur(*this); };
    Iterateur fin() const { return Iterateur(entrees.fin());};
    Iterateur rechercher(const K& cle) const { return Iterateur( entrees.rechercher(cle));};
    Iterateur rechercherEgalOuSuivant(const K& cle) const { return Iterateur(entrees.rechercherEgalOuSuivant(cle));};
    Iterateur rechercherEgalOuPrecedent(const K& cle) const { return Iterateur(entrees.rechercherEgalOuPrecedent(cle));};
};

template <class K, class V>
void ArbreMap<K,V>::vider(){
    entrees.vider();
}

template <class K, class V>
bool ArbreMap<K,V>::vide() const{
    if (entrees.debut() != NULL) {
        return false;
    }
    return true;
}

template <class K, class V>
void ArbreMap<K,V>::enlever(const K& c)
{
    entrees.enlever(Entree(c));
}

template <class K, class V>
bool ArbreMap<K,V>::contient(const K& c) const
{
    return entrees.contient(Entree(c));

}

template <class K, class V>
const V& ArbreMap<K,V>::operator[] (const K& c)  const
{
    typename ArbreAVL<Entree>::Iterateur iter = entrees.debut();

    while ( iter && entrees[iter].cle != c) {
        iter++;
    }

    if(!iter){
        entrees.inserer(c);
        iter = entrees.rechercher(c);
    }
    return entrees[iter].valeur;
}

template <class K, class V>
V& ArbreMap<K,V>::operator[] (const K& c)
{
    typename ArbreAVL<Entree>::Iterateur iter = entrees.debut();

    while ( iter && entrees[iter].cle != c) {
        iter++;
    }

    if(!iter){
        entrees.inserer(c);
        iter = entrees.rechercher(c);
    }
    return entrees[iter].valeur;
}
#endif
