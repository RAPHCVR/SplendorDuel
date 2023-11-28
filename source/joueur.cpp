#include "Jeton.cpp"
#include "Jeton.h"
#include "Partie.cpp"
#include "Partie.h"
#include "Cards.h"
#include "Cards.cpp"
#include "joueur.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>

using namespace std;

//Celine
vector<int> Player::getBonusSummary() {
    vector<int> bonus;
    bonus.push_back(getBlueSummary().getBonusNumber());
    bonus.push_back(getWhiteSummary().getBonusNumber());
    bonus.push_back(getGreenSummary().getBonusNumber());
    bonus.push_back(getBlackSummary().getBonusNumber());
    bonus.push_back(getRedSummary().getBonusNumber());

    return bonus;
}

void Player::addToken(Token token) {
    string color;
    color=token.getColor();
    tokenSummary.at(color)+=1;
    tokens.insert(tokens.begin(),token);
}

void Player::actionReserveCard(){ //Conditions d'activation à penser : moins de 3 cartes en réserve + au moins un or sur plateau
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

void Player::addPrivilege() {
    privilege+=1;
}

void Player::removePrivilege() {
    privilege-=1;
}

void Player::addCrowns(int nbCrowns) {
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

void Player::removeToken(Token &token) {
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

//Celine
// méthode pour retirer les ressources nécessaires lorsque le joueur achète une carte
void Player::spendResources(unordered_map<TokenColor, int> tokensToSpend){
    // remove token from list token + maj token summary et tokens
    // add token to bag
    //bagOfTokens.push_back(tokens.back());

    // parcours le dico
    for(auto cost = tokensToSpend.begin(); cost!=tokensToSpend.end(); cost++){
        // depense des jetons
        for(int i = 0; i< cost->second; i++){
            // utilisation de std::move() pour deplacer l'instance de l'objet plutot que de le supprimer
            auto temp = move(tokens.at(cost->first).back());
            // on met le token dans le sac de jeton
            Bag::getInstance.addToken(temp);
            // on retire le jeton du dico de jeton
            tokens.at(cost->first).pop_back();
            // maj de tokenSummary pour la couleur en cours
            tokenSummary.at(cost->first)--;

        }
    }


}

//Celine
// ajout 'n' points de prestiges de couleur 'color' 
void Player::addPrestige(int n, TokenColor color) {
    //if color not in TokenColor, n < 0 ==> exception 
        prestigePoints += n;
        switch (color) {
        case TokenColor::BLEU:
            blueSummary.addprestigePoints(n);
            break;
        case TokenColor::ROUGE:
            redSummary.addprestigePoints(n);
            break;
        case TokenColor::VERT:
            greenSummary.addprestigePoints(n);
            break;
        case TokenColor::BLANC:
            whiteSummary.addprestigePoints(n);
            break;
        case TokenColor::NOIR:
            blackSummary.addprestigePoints(n);
            break;
        default:
        // pas une carte joallerie

        }
        
        //cout << "Le joueur " << name << " a gagné " << n << " points de prestige!" << endl;
}

//Celine
// méthode pour vérifier si le joueur a les ressources nécessaires pour acheter une carte    
bool Player::canBuyCard(JewelryCard &card){
    // liste des bonus ordre (bleu,blanc,vert,noir,rouge)
    vector<int> bonus = getBonusSummary();
    int goldTokens = tokenSummary[TokenColor::OR];
    unsigned int i = 0;
    // pour chaque type de jeton, on regarde le cout
    for(auto elt = getTokenSummary().begin(); elt!=getTokenSummary().end(); elt++){
        // si cout > tokens du joueur + bonus (= carte trop chere)
        int costGap = card.getCost().at(elt->first) - bonus[i] - elt->second;
        // achat jeton+bonus pas suffisant
        if(costGap > 0){
            // achat de la carte avec or + jetons possible
            if(goldTokens > costGap){
                goldTokens = goldTokens - costGap;
            }
            // pas assez de gold pour acheter carte
            else{
                return false;
            }
        }
        i++;
    } 
    return true;   
}



//Celine
// On y vérifie que le joueur a les moyens d’acheter la carte sélectionnée 
// puis retire et place dans le sac les jetons dépensés lors de l’achat, 
// retire la carte de la pyramide 
// et ajoute la carte à la liste des cartes possédées par le joueur. 
// return int pour dire si ca a marché --> modifier si return + pertinent à faire
int Player::actionBuyCard(JewelryCard &card, int position, unordered_map<TokenColor, int> tokensToSpend){
    // Vérifier si le joueur a les ressources nécessaires pour acheter la carte
        if (canBuyCard(card)) {
            // Retirer les ressources nécessaires
            spendResources(tokensToSpend);

            // Ajouter la carte au joueur
            addJewelryCard(card);

            // Retirer la carte du plateau de jeu
            // getInstance()
            card = Pyramid_Cards::getInstance().takeCard(card.getLevel(), position);
            //cout << "La carte a été achetée avec succès par le joueur " << name << "!" << endl;
            return 1;
        } 
        else {
            //cout << "Le joueur " << name << " ne peut pas acheter cette carte. Ressources insuffisantes." << endl;
            return 0;
        }
    
    // idees qt
    // faire un bouton annuler l'achat dans le fichier interface
    // pour choix du paiement : 
    // avec Qt faire un bouton + / - limité au nombre de jetons max possédés pour chaque couleur du prix
    // avec Qt faire un bouton + / - limité au nombre de bonus max possédés pour chaque couleur du prix
    // au moment du clic sur le bouton acheter, on compte si les jetons+bonus = cout de la carte --> si non on en informe le joueur
    // si oui : 
    // on retire dans chaque tableau de jeton, le nb de jeton de la couleur qui a ete depensé
    //this->tokens[0].pop_back(); // pour chaque jeton utilisé --> dans spendRessources


}


//Celine
// ajout de la carte dans jewelryCards + modif de la carte resumé correspondant (couleur)
void Player::addJewelryCard(JewelryCard &card){
    // ajout de la carte dans la liste de cartes joailleries
    getJewelryCards().push_back(card);

    //ajout des points de prestiges dans summary carte + dans attribut prestigePoints de Player
    addPrestige(card.getPrestige(), card.getBonus().bonus_color);
    

}

//Celine
// ajout de la carte dans royalCards 
void Player::addRoyalCard(RoyalCard &card){
    // ajout de la carte dans la liste de cartes royales
    getRoyalCards().push_back(card);

    // pas de couleur, juste ajout du nb de prestiges
    addPrestige(card.getPrestige(), TokenColor::None);


}