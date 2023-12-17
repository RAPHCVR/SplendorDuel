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


// Celine
// permet de choisir le nombre d'actions optionnelles que fait l'ia
// et lesquelles il fait 
std::vector<Controller::OptionalActions> AiStrategy::choseOptionalActions(){
    std::vector<Controller::OptionalActions> AIsOptionalActions;
    // combien d'action optionnelles (entre 0 et 2)
    int nbOptionalAction = random(0,2);

    // lesquelles parmis 
    // utiliser privilege = prendre jeton (pas or) / remplir plateau + prendre un privilege

    // on ne realise pas d'action optionnelle
    if (nbOptionalAction == 0){
        AIsOptionalActions.push_back(OptionalActions::Empty);
    }
    //on realise une action optionnelle
    else if(nbOptionalAction==1){
        // choisir laquelle
        int whichOpAction = random(0,1);
        whichOpAction == 0? AIsOptionalActions.push_back(OptionalActions::UsePrivileges) : AIsOptionalActions.push_back(OptionalActions::FillBoard);
    }
    else{
        // faire les deux actions
        AIsOptionalActions.push_back(OptionalActions::UsePrivileges);
        AIsOptionalActions.push_back(OptionalActions::FillBoard);
    }
    return AIsOptionalActions;
}

// Celine
// permet de choisir l'action optionnelle que fait l'ia
std::vector<Controller::CompulsoryActions> AiStrategy::choseCompulsoryActions(){
    std::vector<Controller::CompulsoryActions> AIsCompulsoryAction;
    int nbCompulsoryActions = random(0,2);
    // prendre entre un et trois jetons
    if(nbCompulsoryActions == 0){
        AIsCompulsoryAction.push_back(CompulsoryActions::TakeCoins);
    }
    // reserver une carte
    else if(nbCompulsoryActions == 1){
        AIsCompulsoryAction.push_back(CompulsoryActions::ReserveCard);
    }
    else{
        AIsCompulsoryAction.push_back(CompulsoryActions::BuyCard);
    }

    return AIsCompulsoryAction;
}