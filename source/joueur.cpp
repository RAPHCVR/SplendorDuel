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

//Celine
std::vector<int> Player::getBonusSummary() {
    std::vector<int> bonus;
    bonus.push_back(getBlueSummary().getBonusNumber());
    bonus.push_back(getWhiteSummary().getBonusNumber());
    bonus.push_back(getGreenSummary().getBonusNumber());
    bonus.push_back(getBlackSummary().getBonusNumber());
    bonus.push_back(getRedSummary().getBonusNumber());

    return bonus;
}

void Player::addToken(Token &token) {
    tokenSummary.at(token.getColor())+=1;
    tokens.at(token.getColor()).push_back(token);
}

// lise
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
    std::string tokenColor=token.getColor();
    tokenSummary.at(tokenColor)-=1; //retire dans le dico
    std::vector<Token>::iterator it;
    //it = remove(tokens.begin(), tokens.end(), token); ne marche pas encore
}

// lise
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
void Player::spendResources(std::unordered_map<TokenColor, int> tokensToSpend){
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
            Bag::getInstance().addToken(temp);
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
    std::vector<int> bonus = getBonusSummary();
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
// retire et place dans le sac les jetons dépensés lors de l’achat,
// retire la carte de la pyramide 
// et ajoute la carte à la liste des cartes possédées par le joueur. 
void Player::actionBuyCard(JewelryCard &card, int position, std::unordered_map<TokenColor, int> tokensToSpend){
    // Retirer les ressources nécessaires
    spendResources(tokensToSpend);

    // Ajouter la carte au joueur
    addJewelryCard(card);

    // Retirer la carte du plateau de jeu
    // getInstance()
    card = Pyramid_Cards::getInstance().takeCard(card.getLevel(), position);
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

 void actionBuyReservedCard(JewelryCard &card){

 }