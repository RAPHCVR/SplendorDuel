#include "joueur.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>

using namespace std;

void Joueur::addToken(Token token) {
    string color;
    color=token.getColor();
    tokenSummary.at(color)+=1;
    tokens.insert(tokens.begin(),token);
}

void Joueur::actionReserveCard(){ //Conditions d'activation à penser : moins de 3 cartes en réserve + au moins un or sur plateau
    JewelryCard chosenCard;
    Token jetonOr;
    //choix d'une carte joallerie
    //chosenCard=carte choisie
    reserve.insert(reserve.begin(),chosenCard);
    //afficher "Veuillez choisir un jeton Or"
    //afficher le plateau
    //jetonOr=jeton choisi
    while (jetonOr.getColor()!="Or"){
        //afficher "Ceci n'est pas un jeton Or, veuillez en selectionner un"
        //afficher le plateau
        //jetonOr=jeton choisi
    }
    addToken(jetonOr);
};

void Joueur::addPrivilege() {
    privilege+=1;
}

void Joueur::removePrivilege() {
    privilege-=1;
}

void Joueur::addCrowns() {
    nbCrown+=1;
    RoyalCard chosenCard;
    if (nbCrown==3 || nbCrown==6){
        //afficher les carte royales disponibles
        //en choisir une
        //chosenCard=carte choisie
        prestigePoints+=chosenCard.getPrestige();
        cardsRoyal.insert(cardsRoyal.begin(),chosenCard);
        chosenCard.useAbility();
    }
}

void Joueur::removeToken(Token token) {
    string tokenColor=token.getColor();
    tokenSummary.at(tokenColor)-=1; //retire dans le dico
    vector<Token>::iterator it;
    //it = remove(tokens.begin(), tokens.end(), token); ne marche pas encore
}