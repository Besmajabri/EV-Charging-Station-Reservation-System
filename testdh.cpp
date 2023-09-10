/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Été 2022 / TP4
 *  Programme pour tester votre classe DateHeure
 */
#include <sstream>
#include <vector>
#include "dateheure.h"

int main(){
    std::cout << "Vérification de votre classe DateHeure" << std::endl;
    std::stringstream str("0_00:00:00 0_00:00:01 0_00:00:30 0_00:00:59 0_00:01:00 0_00:01:01 0_00:01:10 0_00:01:59 0_00:02:00 0_00:02:01 0_00:02:10 0_00:02:25 0_00:40:00 0_00:40:05 0_00:40:50 0_00:59:00 0_00:59:59 0_01:00:00 0_01:01:00 0_01:02:00 0_02:00:00 0_02:01:00 0_02:03:00 0_02:10:00 0_02:12:00 1_00:00:00 1_01:01:00 2_00:00:00");
    std::vector<DateHeure> dateheures;
    while(!str.eof()){
        DateHeure dh;
        str >> dh >> std::ws;
        std::cout << dh << std::endl;
        if(str)
            dateheures.push_back(dh);
    }
    int nbErreurs=0;
    // Test de l'opérateur DateHeure::operator< (strictement plus petit)
    for(unsigned int i=0;i<dateheures.size();i++)
        for(unsigned int j=0;j<dateheures.size();j++){
            if(i<j){
                if(!(dateheures[i]<dateheures[j])){
                    std::cout << "Erreur : dateheures[" << i << "]<dateheures[" << j << "] : "
                      << dateheures[i] << "<" << dateheures[j] << " retourne faux!" << std::endl;
                    nbErreurs++;
                }
            }else if(j<i){
                if(!(dateheures[j]<dateheures[i])){
                    std::cout << "Erreur : dateheures[" << j << "]<dateheures[" << i << "] : "
                      << dateheures[j] << "<" << dateheures[i] << " retourne faux!" << std::endl;
                    nbErreurs++;
                }
            }else{ // i==j
                if(dateheures[j]<dateheures[i]){
                    std::cout << "Erreur : dateheures[" << j << "]<dateheures[" << i << "] : "
                      << dateheures[j] << "<" << dateheures[i] << " retourne faux!" << std::endl;
                    nbErreurs++;
                }
            }
        }
    std::cout << "Nombre d'erreurs détectée(s): " << nbErreurs << std::endl;
    std::cout << " --- FIN ---" << std::endl;
    return 0;
}

