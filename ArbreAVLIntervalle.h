/*
 * TP4 - Été 2022 - Arbre Intervalle
 * @author Besma Jabri
 *      Code permanent : JABB71560008
 * @author Othmane Azzouzi
 *      Code permanent : AZZO27129703
 */


#ifndef TP4_INF3105_ARBREAVLINTERVALLE_H
#define TP4_INF3105_ARBREAVLINTERVALLE_H

#include <iostream>
#include <cassert>
#include "pile.h"

template <class T>
class ArbreAVLIntervalle {
public:
    ArbreAVLIntervalle();
    ArbreAVLIntervalle(const ArbreAVLIntervalle&);
    ~ArbreAVLIntervalle();

    class Iterateur;
    void inserer(const T& debut, const T& fin);
    bool contient(const T& debut,const T& fin) const;
    bool vide() const;
    void vider();
    int  hauteur() const;
    int taille() const;
    void afficher() const;

    bool contientIntervalle (ArbreAVLIntervalle& autre);
    bool intercepteIntervalle (ArbreAVLIntervalle& autre);

    // Fonctions pour obtenir un itérateur sur la position dans un arbre AVL
    Iterateur debut() const;
    Iterateur fin() const;
    Iterateur rechercher(const T& debut) const;
    Iterateur rechercherEgalOuSuivant(const T& debut) const;
    Iterateur rechercherEgalOuPrecedent(const T& debut) const;

    // Accès aux éléments de l'arbre via un intérateur.
    const T& operator[](const Iterateur&) const;
    T& operator[](const Iterateur&);

    // Copie d'un arbre AVL.
    ArbreAVLIntervalle& operator = (const ArbreAVLIntervalle&);

private:
    struct Noeud {
        Noeud(const T& _debut, const T& _fin);
        T debut, fin;
        T min, max;
        int equilibre;
        Noeud* gauche = nullptr;
        Noeud* droite = nullptr;
    };
    Noeud* racine;

    // Fonctions internes
    bool inserer(Noeud*&, const T& debut, const T& fin);
    bool contient(const T& debut,const T& fin, const Noeud*) const;
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
        Iterateur(const ArbreAVLIntervalle& a);
        Iterateur(const Iterateur& a);

        operator bool() const;
        bool operator!() const;
        bool operator==(const Iterateur&) const;
        bool operator!=(const Iterateur&) const;
        const T& operator*() const;
        Iterateur& operator++(); // Pré-incrément ++i
        Iterateur operator++(int); // Post-incrément i++
        Iterateur& operator = (const Iterateur&);

    public:
        const ArbreAVLIntervalle& arbre_associe;
        Noeud* courant;
        Pile<Noeud*> chemin;
        friend class ArbreAVLIntervalle;
    };
    bool operator== (const ArbreAVLIntervalle& autre) const;
};

/** constructeur Noeud */
template <class T>
ArbreAVLIntervalle<T>::Noeud::Noeud(const T& _debut, const T& _fin)
        : debut(_debut), fin(_fin), min(_debut), max(_fin), equilibre(0), gauche(nullptr), droite(nullptr) {
}

/** constructeur d'un arbre AVL */
template <class T>
ArbreAVLIntervalle<T>::ArbreAVLIntervalle()
        : racine(nullptr) {
}

/** constructeur par copie d'un arbre AVL */
template <class T>
ArbreAVLIntervalle<T>::ArbreAVLIntervalle(const ArbreAVLIntervalle<T>& autre)
        : racine(nullptr) {
    copier(autre.racine, racine);
}

/** destructeur d'un arbre AVL */
template <class T>
ArbreAVLIntervalle<T>::~ArbreAVLIntervalle() {
    vider(racine);
}

/** surcharge de l'pérateur == qui permet de verifier si l'arbre courant
  * contient les memes éléments que celle passé en paramètre */
template <class T>
bool ArbreAVLIntervalle<T>::operator== (const ArbreAVLIntervalle& autre) const {
    Iterateur iter(*this); // itérer sur l'arbre courant.
    Iterateur iterAutre(autre); // itérer sur l'arbre passé en paramètre.
    iter = debut();
    iterAutre = autre.debut();
    if (this == &autre) return true; // même arbre
    while ( iter && iterAutre ) {
        // avancer tant que les élements sont égaux
        if (iter.courant->debut != iterAutre.courant->debut
        && iter.courant->fin != iterAutre.courant->fin) return false;
        iter++;
        iterAutre++;
    }
    if (iter.courant != fin().courant || iterAutre.courant != autre.fin().courant) return false;
    return true;
}

/** Fonction copier permet de copier les éléments d'un noeud dans source */
template <class T>
void ArbreAVLIntervalle<T>::copier(const Noeud* source, Noeud*& noeud) const {
    if(source != nullptr) {
        noeud = new Noeud(source->debut, source->fin);
        noeud->min = source->min;
        noeud->max = source->max;
        noeud->equilibre = source->equilibre;
        copier(source->gauche, noeud->gauche);
        copier(source->droite, noeud->droite);
    }
}

template <class T>
void ArbreAVLIntervalle<T>::vider(Noeud*& noeud) { // fonction privée
    if(noeud != nullptr) {
        vider(noeud->gauche);
        vider(noeud->droite);
        delete noeud; // libérer le tas
        noeud = nullptr;
    }
}

/** fonction contient : permet de vérifier si l'arbre contienr un élément */
template <class T>
bool ArbreAVLIntervalle<T>::contient(const T& debut,const T& fin) const { // fonction publique
    return contient(debut, fin, racine);
}

template <class T>
bool ArbreAVLIntervalle<T>::contient(const T& debut, const T& fin, const Noeud* noeud) const { // fonction privée
    if(noeud == nullptr)
        return false;
    if(debut == noeud->debut && fin == noeud->fin )
        return true;
    if(debut < noeud->debut)
        return contient(debut, fin, noeud->gauche);
    return contient(debut, fin, noeud->droite);
}

/** fonction contient : permet d'inserer un élément dans un arbre */
template <class T>
void ArbreAVLIntervalle<T>::inserer(const T& debut, const T& fin) { // fonction publique

    inserer(racine, debut, fin);

}

template <class T>
bool ArbreAVLIntervalle<T>::inserer(Noeud*& noeud, const T& debut, const T& fin) { // fonction privée
    if(noeud == nullptr) {
        noeud = new Noeud(debut, fin);
        noeud->min = debut;
        noeud->max = fin;
        return true;
    }
    if(debut < noeud->debut) { // Insertion à gauche
        if(inserer(noeud->gauche, debut, fin)) {
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
    } else if(noeud->debut < debut) { // Insertion à droite
        if(inserer(noeud->droite, debut, fin)) {
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
    noeud->debut = debut;  // Mise à jour
    noeud->fin = fin;
    noeud->min = debut;
    noeud->max = fin;
    return false;
}

/** Rotation de gauche à droite */
template <class T>
void ArbreAVLIntervalle<T>::rotationGaucheDroite(Noeud*& racinesousarbre) {
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
void ArbreAVLIntervalle<T>::rotationDroiteGauche(Noeud*& racinesousarbre) {
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
bool ArbreAVLIntervalle<T>::vide() const {
    return racine == nullptr;
}
/** Fonction vider permet de vider l'arbre */
template <class T>
void ArbreAVLIntervalle<T>::vider() { // fonction publique
    vider(racine);
}

/** Fonction hauteur permet de calculer l'hauteur d'un arbre  */
template <class T>
int  ArbreAVLIntervalle<T>::hauteur() const { // fonction non récursive
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
int  ArbreAVLIntervalle<T>::taille() const { // fonction publique
    return compter(racine);
}

/** Fonction compter permet de compter le nombre de noeud de l'arbre */
template <class T>
int ArbreAVLIntervalle<T>::compter(const Noeud* n) const { // fonction privée
    if(n==nullptr) return 0; // arbre vide
    return 1 + compter(n->gauche) + compter(n->droite);
}

/** debut de l'arbre */
template <class T>
typename ArbreAVLIntervalle<T>::Iterateur ArbreAVLIntervalle<T>::debut() const {
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
typename ArbreAVLIntervalle<T>::Iterateur ArbreAVLIntervalle<T>::fin() const {
    return Iterateur(*this);
}

template <class T>
typename ArbreAVLIntervalle<T>::Iterateur ArbreAVLIntervalle<T>::rechercher(const T& debut) const {
    Iterateur iter = rechercherEgalOuSuivant(debut);
    if(!iter) {
        iter.chemin.vider();
        iter.courant = nullptr;
    }
    return iter;
}

template <class T>
typename ArbreAVLIntervalle<T>::Iterateur ArbreAVLIntervalle<T>::rechercherEgalOuSuivant( const T& debut ) const {
    Iterateur iter(*this);
    iter.courant = this->racine;
    while(iter && iter.courant->debut != debut) {
        if(debut < iter.courant->debut) {
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
typename ArbreAVLIntervalle<T>::Iterateur ArbreAVLIntervalle<T>::rechercherEgalOuPrecedent(const T& debut) const {
    Iterateur iter(*this);
    iter.courant = this->racine;
    Noeud* temp = nullptr;
    while(iter && iter.courant->debut != debut) {
        if(debut < iter.courant->debut) {
            iter.chemin.empiler(iter.courant);
            iter.courant = iter.courant->gauche;
        } else {
            temp = iter.courant;
            iter.courant = iter.courant->droite;
        }
    }
    if(!iter) {
        if(temp != nullptr)
            iter = rechercher(temp->debut);
        else
            iter.chemin.vider();
    }

    return iter;
}

/** surchage de l'opérateur [] const */
template <class T>
const T& ArbreAVLIntervalle<T>::operator[](const Iterateur& iterateur) const {
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}
/** surchage de l'opérateur [] */
template <class T>
T& ArbreAVLIntervalle<T>::operator[](const Iterateur& iterateur) {
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}
/** surchage de l'opérateur= d'affectation  */
template <class T>
ArbreAVLIntervalle<T>& ArbreAVLIntervalle<T>::operator=(const ArbreAVLIntervalle& autre) {
    if(this == &autre)
        return *this;
    vider();
    copier(autre.racine, racine);
    return *this;
}


//----------------------- !!! ITERATEUR !!! -----------------------------

template <class T>
ArbreAVLIntervalle<T>::Iterateur::Iterateur(const ArbreAVLIntervalle& a) : arbre_associe(a), courant(nullptr) {}

template <class T>
ArbreAVLIntervalle<T>::Iterateur::Iterateur(const ArbreAVLIntervalle<T>::Iterateur& a)
        : arbre_associe(a.arbre_associe), courant(a.courant), chemin(a.chemin) {}

// Pré-incrément
template <class T>
typename ArbreAVLIntervalle<T>::Iterateur& ArbreAVLIntervalle<T>::Iterateur::operator++() {
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
typename ArbreAVLIntervalle<T>::Iterateur ArbreAVLIntervalle<T>::Iterateur::operator++(int) {
    Iterateur copie(*this);
    operator++();
    return copie;
}

template <class T>
ArbreAVLIntervalle<T>::Iterateur::operator bool() const {
    return courant != nullptr;
}

template <class T>
bool ArbreAVLIntervalle<T>::Iterateur::operator!() const {
    return courant == nullptr;
}

template <class T>
bool ArbreAVLIntervalle<T>::Iterateur::operator==(const Iterateur& o) const {
    assert(&arbre_associe == &o.arbre_associe);
    return courant == o.courant;
}

template <class T>
bool ArbreAVLIntervalle<T>::Iterateur::operator!=(const Iterateur& o) const {
    assert(&arbre_associe == &o.arbre_associe);
    return courant != o.courant;
}

template <class T>
const T& ArbreAVLIntervalle<T>::Iterateur::operator *() const {
    assert(courant != nullptr);
    return courant->contenu;
}

template <class T>
typename ArbreAVLIntervalle<T>::Iterateur& ArbreAVLIntervalle<T>::Iterateur::operator = (const Iterateur& autre) {
    assert(&arbre_associe == &autre.arbre_associe);
    courant = autre.courant;
    chemin = autre.chemin;
    return *this;
}

#endif //TP4_INF3105_ARBREAVLINTERVALLE_H
