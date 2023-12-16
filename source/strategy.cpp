#include "Jeton.h"
#include "Cards.h"
#include "joueur.h"
#include "strategy.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>


int AiStrategy::random(int min, int max){
    int random;
    // générateur de nombres aléatoires avec le temps actuel
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // nombre aléatoire dans l'intervalle [min, max]
    random = std::rand() % (max - min + 1) + min;
    return random;
}


std::vector<Controller::OptionalActions> AiStrategy::choseOptionalActions(){
    std::vector<Controller::OptionalActions> optionalActions;
    // combien d'action optionnelles (entre 0 et 2)
    int nbOptionalAction = random(0,2);

    // lesquelles parmis 
    // utiliser privilege = prendre jeton (pas or) / remplir plateau + prendre un privilege

    // on ne realise pas d'action optionnelle
    if (nbOptionalAction == 0){
        optionalActions.push_back(OptionalActions::Empty);
    }
    //on realise une action optionnelle
    else if(nbOptionalAction==1){
        // choisir laquelle
        int whichOpAction = random(0,1);
        whichOpAction == 0? optionalActions.push_back(OptionalActions::UsePrivileges) : optionalActions.push_back(OptionalActions::FillBoard);
    }
    else{
        // faire les deux actions
        optionalActions.push_back(OptionalActions::UsePrivileges);
        optionalActions.push_back(OptionalActions::FillBoard);
    }
    return optionalActions;
}
