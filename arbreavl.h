/* Squelette minimal pour classe générique ArbreAVL<T> du TP3.
 * TP3 - Été 2022 - Arbres AVL
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes
 * http://ericbeaudry.uqam.ca/INF31ƒ05/tp3/
 *
 * Suggestions:
 *  - utilisez votre solution des Lab6 et Lab7;
 *  - ajoutez-y les fonctions demandées ci-dessous.
 * @author Besma Jabri
 *      Code permanent : JABB71560008
 * @author Othmane Azzouzi
 *      Code permanent : AZZO27129703
 */

#ifndef ARBREAVL_H
#define ARBREAVL_H

#include <iostream>
#include <cassert>
#include "pile.h"

// Note : Nous avons utilisé la classe pile.h du la lab 7.

// Déclare l'existance de la classe template ArbreAVL (pour déclarer unionn et inter).
template <class K, class V>
class ArbreMap;

template <class T>
class ArbreAVL;

// Déclare l'existance des fonctions unionn et inter (pour les déclarer friend dans ArbreAVL).
template <class T>
ArbreAVL<T> unionn(const ArbreAVL<T>& a, const ArbreAVL<T>& b);
template <class T>
ArbreAVL<T> inter(const ArbreAVL<T>& a, const ArbreAVL<T>& b); // intersection


template <class T>
class ArbreAVL {
public:
    ArbreAVL();
    ArbreAVL(const ArbreAVL&);
    ~ArbreAVL();

    // Annonce l'existance d'une classe Iterateur.
    class Iterateur;

    void inserer(const T&);
    bool contient(const T&) const;
    bool vide() const;
    void vider();
    int  hauteur() const;
    int taille() const;
    void afficher() const;

    // Fonctions pour obtenir un itérateur sur la position dans un arbre AVL
    Iterateur debut() const;
    Iterateur fin() const;
    Iterateur rechercher(const T&) const;
    Iterateur rechercherEgalOuSuivant(const T&) const;
    Iterateur rechercherEgalOuPrecedent(const T&) const;

    // Accès aux éléments de l'arbre via un intérateur.
    const T& operator[](const Iterateur&) const;
    T& operator[](const Iterateur&);

    // Copie d'un arbre AVL.
    ArbreAVL& operator = (const ArbreAVL&);
private:

    struct Noeud {
        Noeud(const T&);
        T contenu;
        int equilibre;
        Noeud* gauche = nullptr;
        Noeud* droite = nullptr;
    };
    Noeud* racine;

    // Fonctions internes
    bool inserer(Noeud*&, const T&);
    bool contient(const T&, const Noeud*) const;

    void rotationGaucheDroite(Noeud*&);
    void rotationDroiteGauche(Noeud*&);
    void vider(Noeud*&);
    void copier(const Noeud*, Noeud*&) const;
    const T& max(Noeud*) const;

    int compter(const Noeud*) const;
    void preparerafficher(const Noeud* n, int profondeur, int& rang, T* elements, int* profondeurs) const;

public:
    class Iterateur {
    public:
        Iterateur(const ArbreAVL& a);
        Iterateur(const Iterateur& a);

        operator bool() const;
        bool operator!() const;
        bool operator==(const Iterateur&) const;
        bool operator!=(const Iterateur&) const;
        const T& operator*() const;
        Iterateur& operator++(); // Pré-incrément ++i
        Iterateur operator++(int); // Post-incrément i++
        Iterateur& operator = (const Iterateur&);

    private:
        const ArbreAVL& arbre_associe;
        Noeud* courant;
        Pile<Noeud*> chemin;
        friend class ArbreAVL;
        template <class K, class V>
        friend class ArbreMap;
        friend ArbreAVL<T> unionn<T>(const ArbreAVL<T>& a, const ArbreAVL<T>& b); // union
        friend ArbreAVL<T> inter<T>(const ArbreAVL<T>& a, const ArbreAVL<T>& b); // intersection
    };
    bool operator== (const ArbreAVL& autre) const;
};

//-----------------------------------------------------------------------------

/** constructeur Noeud */
template <class T>
ArbreAVL<T>::Noeud::Noeud(const T& c)
        : contenu(c), equilibre(0), gauche(nullptr), droite(nullptr) {
}

/** constructeur d'un arbre AVL */
template <class T>
ArbreAVL<T>::ArbreAVL()
        : racine(nullptr) {
}

/** constructeur par copie d'un arbre AVL */
template <class T>
ArbreAVL<T>::ArbreAVL(const ArbreAVL<T>& autre)
        : racine(nullptr) {
    copier(autre.racine, racine);
}

/** destructeur d'un arbre AVL */
template <class T>
ArbreAVL<T>::~ArbreAVL() {
    vider(racine);
}
/** surcharge de l'pérateur == qui permet de verifier si l'arbre courant
  * contient les memes éléments que celle passé en paramètre */
template <class T>
bool ArbreAVL<T>::operator== (const ArbreAVL& autre) const {
    Iterateur iter(*this); // itérer sur l'arbre courant.
    Iterateur iterAutre(autre); // itérer sur l'arbre passé en paramètre.
    iter = debut();
    iterAutre = autre.debut();
    if (this == &autre) return true; // même arbre
    while ( iter && iterAutre ) {
        // avancer tant que les élements sont égaux
        if (iter.courant->contenu != iterAutre.courant->contenu) return false;
        iter++;
        iterAutre++;
    }
    if (iter.courant != fin().courant || iterAutre.courant != autre.fin().courant) return false;
    return true;
}
/** fonction unionn qui retourne le résultat d'union de
 * deux arbres AVL passés en paramètre */
template <class T>
ArbreAVL<T> unionn(const ArbreAVL<T>& a, const ArbreAVL<T>& b){
    ArbreAVL<T> resultat;
    typename ArbreAVL<T>::Iterateur iterA(a.debut()); // iterateur sur l'arbre a
    typename ArbreAVL<T>::Iterateur iterB(b.debut()); // iterateur sur l'arbre b
    // inserer les elements de l'arbre a
    while( iterA ) {
        resultat.inserer(iterA.courant->contenu);
        ++iterA;
    }
    // inserer les elements de l'arbre b
    while( iterB ) {
        resultat.inserer(iterB.courant->contenu);
        ++iterB;
    }
    return resultat;
}
/** fonction inter qui retourne le résultat d'intersection de
 * deux arbres AVL passés en paramètre */
template <class T>
ArbreAVL<T> inter(const ArbreAVL<T>& a, const ArbreAVL<T>& b){
    ArbreAVL<T> resultat;
    typename ArbreAVL<T>::Iterateur iterA(a.debut());
    // iérer sur la première arbre a
    while ( iterA ) {
        // si l'élément existe dans l'arbre b
        if (b.contient(iterA.courant->contenu)) {
            //inserer élément dans arbre résulant
            resultat.inserer(iterA.courant->contenu);
        }
        ++iterA;
    }
    return resultat;
}
/** fonction contient : permet de vérifier si l'arbre contienr un élément */
template <class T>
bool ArbreAVL<T>::contient(const T& element) const { // fonction publique
    return contient(element, racine);
}
template <class T>
bool ArbreAVL<T>::contient(const T& element, const Noeud* noeud) const { // fonction privée
    if(noeud == nullptr)
        return false;
    if(element == noeud->contenu)
        return true;
    if(element < noeud->contenu)
        return contient(element, noeud->gauche);
    return contient(element, noeud->droite);
}
/** fonction contient : permet d'inserer un élément dans un arbre */
template <class T>
void ArbreAVL<T>::inserer(const T& element) { // fonction publique
    inserer(racine, element);
}

template <class T>
bool ArbreAVL<T>::inserer(Noeud*& noeud, const T& element) { // fonction privée
    if(noeud == nullptr) {
        noeud = new Noeud(element);
        return true;
    }
    if(element < noeud->contenu) { // Insertion à gauche
        if(inserer(noeud->gauche, element)) {
            ++(noeud->equilibre);
            if(noeud->equilibre == 0)
                return false;
            if(noeud->equilibre == 1)
                return true;
            assert(noeud->equilibre == 2);
            if(noeud->gauche->equilibre == -1)
                rotationDroiteGauche(noeud->gauche);
            rotationGaucheDroite(noeud);
        }
        return false;
    } else if(noeud->contenu < element) { // Insertion à droite
        if(inserer(noeud->droite, element)) {
            --(noeud->equilibre);
            if(noeud->equilibre == 0)
                return false;
            if(noeud->equilibre == -1)
                return true;
            assert(noeud->equilibre == -2);
            if(noeud->droite->equilibre == 1)
                rotationGaucheDroite(noeud->droite);
            rotationDroiteGauche(noeud);
        }
        return false;
    }
    // element == noeud->contenu
    noeud->contenu = element;  // Mise à jour
    return false;
}
/** Rotation de gauche à droite */
template <class T>
void ArbreAVL<T>::rotationGaucheDroite(Noeud*& racinesousarbre) {
    Noeud* temp = racinesousarbre->gauche;
    int  ea = temp->equilibre;
    int  eb = racinesousarbre->equilibre;
    int  neb = -(ea > 0 ? ea : 0) - 1 + eb;
    int  nea = ea + (neb < 0 ? neb : 0) - 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->gauche = temp->droite;
    temp->droite = racinesousarbre;
    racinesousarbre = temp;
}
/** Rotation de droite à gauche */
template <class T>
void ArbreAVL<T>::rotationDroiteGauche(Noeud*& racinesousarbre) {
    Noeud* temp = racinesousarbre->droite;
    int  ea = racinesousarbre->equilibre;
    int  eb = temp->equilibre;
    int  eap = 1 + ea - (0 < eb ? 0 : eb);
    int  ebp = 1 + eb + (0 > eap ? 0 : eap);

    racinesousarbre->equilibre = eap;
    temp->equilibre = ebp;
    racinesousarbre->droite = temp->gauche;
    temp->gauche = racinesousarbre;
    racinesousarbre = temp;
}
/** Fonction vide permet de vérifier si l'arbre courant est vide ou non.
 * la racine est null */
template <class T>
bool ArbreAVL<T>::vide() const {
    return racine == nullptr;
}
/** Fonction vider permet de vider l'arbre */
template <class T>
void ArbreAVL<T>::vider() { // fonction publique
    vider(racine);
}

template <class T>
void ArbreAVL<T>::vider(Noeud*& noeud) { // fonction privée
    if(noeud != nullptr) {
        vider(noeud->gauche);
        vider(noeud->droite);
        delete noeud; // libérer le tas
        noeud = nullptr;
    }
}
/** Fonction copier permet de copier les éléments d'un noeud dans source */
template <class T>
void ArbreAVL<T>::copier(const Noeud* source, Noeud*& noeud) const {
    if(source != nullptr) {
        noeud = new Noeud(source->contenu);
        noeud->equilibre = source->equilibre;
        copier(source->gauche, noeud->gauche);
        copier(source->droite, noeud->droite);
    }
}
/** Fonction hauteur permet de calculer l'hauteur d'un arbre  */
template <class T>
int  ArbreAVL<T>::hauteur() const { // fonction non récursive
    Noeud* temp = racine;
    int h = 0;
    while(temp != nullptr) {
        ++h;
        temp = temp->equilibre > 0 ? temp->gauche : temp->droite;
    }
    return h;
}
/** Fonction taille permet de compter le nombre de noeud de l'arbre */
template <class T>
int  ArbreAVL<T>::taille() const { // fonction publique
    return compter(racine);
}
/** Fonction compter permet de compter le nombre de noeud de l'arbre */
template <class T>
int ArbreAVL<T>::compter(const Noeud* n) const { // fonction privée
    if(n==nullptr) return 0; // arbre vide
    return 1 + compter(n->gauche) + compter(n->droite);
}
/** debut de l'arbre */
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const {
    Iterateur iter(*this);
    iter.courant = this->racine;
    while(iter && iter.courant->gauche != nullptr) {
        iter.chemin.empiler(iter.courant);
        iter.courant = iter.courant->gauche;
    }
    return iter;
}
/** fin de l'arbre */
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::fin() const {
    return Iterateur(*this);
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercher(const T& e) const {
    Iterateur iter = rechercherEgalOuSuivant(e);
    if(!iter) {
        iter.chemin.vider();
        iter.courant = nullptr;
    }
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuSuivant( const T& e) const {
    Iterateur iter(*this);
    iter.courant = this->racine;
    while(iter && iter.courant->contenu != e) {
        if(e < iter.courant->contenu) {
            iter.chemin.empiler(iter.courant);
            iter.courant = iter.courant->gauche;
        } else
            iter.courant = iter.courant->droite;
    }

    if(!iter && !iter.chemin.vide())
        iter.courant = iter.chemin.depiler();
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuPrecedent( const T& e) const {
    Iterateur iter(*this);
    iter.courant = this->racine;
    Noeud* temp = nullptr;
    while(iter && iter.courant->contenu != e) {
        if(e < iter.courant->contenu) {
            iter.chemin.empiler(iter.courant);
            iter.courant = iter.courant->gauche;
        } else {
            temp = iter.courant;
            iter.courant = iter.courant->droite;
        }
    }
    if(!iter) {
        if(temp != nullptr)
            iter = rechercher(temp->contenu);
        else
            iter.chemin.vider();
    }

    return iter;
}
/** surchage de l'opérateur [] const */
template <class T>
const T& ArbreAVL<T>::operator[](const Iterateur& iterateur) const {
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}
/** surchage de l'opérateur [] */
template <class T>
T& ArbreAVL<T>::operator[](const Iterateur& iterateur) {
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}
/** surchage de l'opérateur= d'affectation  */
template <class T>
ArbreAVL<T>& ArbreAVL<T>::operator=(const ArbreAVL& autre) {
    if(this == &autre)
        return *this;
    vider();
    copier(autre.racine, racine);
    return *this;
}
//----------------------- !!! ITERATEUR !!! -----------------------------

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL& a) : arbre_associe(a), courant(nullptr) {}

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL<T>::Iterateur& a)
        : arbre_associe(a.arbre_associe), courant(a.courant), chemin(a.chemin) {}

// Pré-incrément
template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator++() {
    if(!*this)
        return *this;

    courant = courant->droite;
    while(*this) {
        chemin.empiler(courant);
        courant = courant->gauche;
    }
    if(!chemin.vide())
        courant = chemin.depiler();

    return *this;
}

// Post-incrément
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::Iterateur::operator++(int) {
    Iterateur copie(*this);
    operator++();
    return copie;
}

template <class T>
ArbreAVL<T>::Iterateur::operator bool() const {
    return courant != nullptr;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!() const {
    return courant == nullptr;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator==(const Iterateur& o) const {
    assert(&arbre_associe == &o.arbre_associe);
    return courant == o.courant;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!=(const Iterateur& o) const {
    assert(&arbre_associe == &o.arbre_associe);
    return courant != o.courant;
}

template <class T>
const T& ArbreAVL<T>::Iterateur::operator *() const {
    assert(courant != nullptr);
    return courant->contenu;
}

template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator = (const Iterateur& autre) {
    assert(&arbre_associe == &autre.arbre_associe);
    courant = autre.courant;
    chemin = autre.chemin;
    return *this;
}
#endif
