#include "joueur.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>

using namespace std;
// premier commit sur la branche Celine
// second commit

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


// prendre les jetons sur le plateau (demander la rpz des jetons sur le plateau)
void Player::actionAddToken(){
        int nb_jetons = 0;
        Token token1, token2, token3; // token est recopiable
        int ok; 
    while(!ok){
        // affiche les jetons dispo sur le plateau + demander lequel (clique sur le jeton) [griser + non cliquable les cases pas cliquables]
        // if(nb_jetons == O), else if nb_jeton ==1, 2
        // a chaque clic --> incrementer nb_jeton + remplir les info du jeton selectionné + demander si ok ==1 ou 0 pour ajout definitif


        // 
    }
    // ok = 1 --> ajout
    tab_token_to_add
    for(int i =0; i< nb_jeton; i++){
        addToken(tab_token_to_add[i]);
        // retirer le jeton du plateau (methode plateau)
    }
    
}

// méthode pour vérifier si le joueur a les ressources nécessaires pour acheter une carte    
// pour vérifier si le joueur a les ressources nécessaires (tokens, bonus)
// retourne true si le joueur peut acheter la carte, false sinon.
bool Player::canBuyCard(const JewelryCard &card){}

// méthode pour retirer les ressources nécessaires lorsque le joueur achète une carte
void Player::spendResources(const JewelryCard &card) {}

void Player::addPrestige(int n, TokenColor color) {
        prestigePoints += n;
        switch (color) {
        case TokenColor::BLEU:
            blueSummary.addPrestige(n);
            break;
        case TokenColor::ROUGE:
            redSummary.addPrestige(n);
            break;
        case TokenColor::VERT:
            greenSummary.addPrestige(n);
            break;
        case TokenColor::BLANC:
            whiteSummary.addPrestige(n);
            break;
        case TokenColor::NOIR:
            blackSummary.addPrestige(n);
            break;
        default:
        // pas une carte joallerie

    }
        
        //cout << "Le joueur " << name << " a gagné " << n << " points de prestige!" << endl;
}

// On y vérifie que le joueur a les moyens d’acheter la carte sélectionnée 
// puis retire et place dans le sac les jetons dépensés lors de l’achat, 
// retire la carte de la pyramide 
// et ajoute la carte à la liste des cartes possédées par le joueur. 

void Player::actionBuyCard(JewelryCard &card){
    // Vérifier si le joueur a les ressources nécessaires pour acheter la carte
        if (canBuyCard(card)) {
            // Retirer les ressources nécessaires
            spendResources(card);

            // Ajouter la carte au joueur
            addJewelryCard(card);

            // Ajouter des points de prestige si la carte a des points associés
            addPrestige(card.getPrestige(), card.getBonus());
            // Retirer la carte du plateau de jeu
            card = Pyramid_Cards::takeCard(card.getLevel(), card.getPosition());


            //cout << "La carte a été achetée avec succès par le joueur " << name << "!" << endl;
        } else {
            //cout << "Le joueur " << name << " ne peut pas acheter cette carte. Ressources insuffisantes." << endl;
        }
    

    // faire un bouton annuler l'achat dans le fichier interface
    // pour choix du paiement : 
    // avec Qt faire un bouton + / - limité au nombre de jetons max possédés pour chaque couleur du prix
    // avec Qt faire un bouton + / - limité au nombre de bonus max possédés pour chaque couleur du prix
    // au moment du clic sur le bouton acheter, on compte si les jetons+bonus = cout de la carte --> si non on en informe le joueur
    // si oui : 
    // on retire dans chaque tableau de jeton, le nb de jeton de la couleur qui a ete depensé
    //this->tokens[0].pop_back(); // pour chaque jeton utilisé --> dans spendRessources

    

}
