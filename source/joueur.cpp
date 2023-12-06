#include "Jeton.h"
#include "Cards.h"
#include "joueur.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>

//Celine
// recapitule les bonus totaux pour chaque couleur de carte
std::vector<int> Player::getBonusSummary() {
    std::vector<int> bonus;
    bonus.push_back(getBlueSummary().getBonusNumber());
    bonus.push_back(getWhiteSummary().getBonusNumber());
    bonus.push_back(getGreenSummary().getBonusNumber());
    bonus.push_back(getBlackSummary().getBonusNumber());
    bonus.push_back(getRedSummary().getBonusNumber());

    return bonus;
}

//celine
void Player::addToken(const Token &token) {
    tokenSummary.at(token.getColor())+=1;
    tokens.at(token.getColor()).push_back(&token);
}

// // lise --> plus besoin car on a tout dans Player::buyReservedCard()
// void Player::actionReserveCard(){ //Conditions d'activation à penser : moins de 3 cartes en réserve + au moins un or sur plateau
//     JewelryCard chosenCard;
//     Token jetonOr;
//     //choix d'une carte joallerie
//     //chosenCard=carte choisie
//     reserve.insert(reserve.begin(),chosenCard);
//     //afficher "Veuillez choisir un jeton Or"
//     //afficher le plateau
//     //jetonOr=jeton choisi
//     while (jetonOr.getColor()!="Or"){
//         //afficher "Ceci n'est pas un jeton Or, veuillez en selectionner un"
//         //afficher le plateau
//         //jetonOr=jeton choisi
//     }
//     addToken(jetonOr);
// };

// Celine
// ajout d'une carte dans la reserve
void Player::reserveOneCard(JewelryCard& card){
    getReserve().push_back(card);
}


//verifie que le joueur a moins de 3 cartes dans sa reserve et qu'il y a au moins un or sur le plateau
bool Player::canReserveCard(){
    if (reserve.size()<3){
        if (Board::getInstance().hasTokenOfColor(TokenColor::OR)){
            return true;
        }
    }
    return false;
}

void Player::addPrivilege(const Privilege& privilege) {
    unsigned int nb = getPrivilege();
    privileges[nb]=&privilege;
}

unsigned int Player::getPrivilege() const {
    unsigned int nb = privileges.size();
    for (auto privilege : privileges) {
        if (privilege == nullptr) {
            nb--;
        }
    }
    return nb;
}

const Privilege& Player::removePrivilege() {
    unsigned int size = getPrivilege()-1;
    const Privilege& privilege = *privileges[size];
    privileges[size] = nullptr;
    return privilege;
}

void Player::addCrowns(int nbCrowns) {
    nbCrown+=1;
}

// celine
// retire un jeton de couleur "color"
// retourne un token pour permettre de le remettre dans le sac si 11 eme jeton 
// ou le donner a un autre joueur si vol
const Token& Player::removeToken(TokenColor color) {
    //retire dans le resume de jetons
    tokenSummary.at(color)-=1;

    // retire jeton dans inventaire
    const Token* token = tokens.at(color).back();
    // on retire le jeton du dico de jeton
    tokens.at(color).pop_back();
    return *token;
}

// // lise
// // prendre les jetons sur le plateau (demander la rpz des jetons sur le plateau)
// void Player::actionAddToken(){
//         int nb_jetons = 0;
//         Token token1, token2, token3; // token est recopiable
//         int ok; 
//     while(!ok){
//         // affiche les jetons dispo sur le plateau + demander lequel (clique sur le jeton) [griser + non cliquable les cases pas cliquables]
//         // if(nb_jetons == O), else if nb_jeton ==1, 2
//         // a chaque clic --> incrementer nb_jeton + remplir les info du jeton selectionné + demander si ok ==1 ou 0 pour ajout definitif


//         // 
//     }
//     // ok = 1 --> ajout
//     tab_token_to_add
//     for(int i =0; i< nb_jeton; i++){
//         addToken(tab_token_to_add[i]);
//         // retirer le jeton du plateau (methode plateau)
//     }
    
// }

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
            auto temp = std::move(tokens.at(cost->first).back());
            // on retire le jeton du dico de jeton
            tokens.at(cost->first).pop_back();
            // on met le token dans le sac de jeton
            Bag::getInstance().addToken(*temp);
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
        // pas une carte joallerie

        }
        
        //cout << "Le joueur " << name << " a gagné " << n << " points de prestige!" << endl;
}


// VERIFIER SI CARTE COLOR NONE (cameleon) SI A DEJA
//Celine
// méthode pour vérifier si le joueur a les ressources nécessaires pour acheter une carte    
bool Player::canBuyCard(JewelryCard &card){
    std::vector<int> bonus = getBonusSummary();
    int goldTokens = tokenSummary[TokenColor::OR];
    unsigned int i = 0;
    for(auto elt = getTokenSummary().begin(); elt!=getTokenSummary().end(); elt++){
        int cost = 0;
        if (card.getCost().find(elt->first) != card.getCost().end()) {
            cost = card.getCost().at(elt->first);
        }
        int costGap = cost - bonus[i] - elt->second;
        if(costGap > 0){
            if(goldTokens > costGap){
                goldTokens = goldTokens - costGap;
            }
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
//void Player::actionBuyCard(JewelryCard &card, int position, std::unordered_map<TokenColor, int> tokensToSpend){
void Player::actionBuyCard(JewelryCard &card){
    // Retirer les ressources nécessaires
    std::unordered_map<TokenColor, int> tokensToSpend = card.getCost();
    spendResources(tokensToSpend);

    // Ajouter la carte au joueur
    addJewelryCard(card);

    // Retirer la carte du plateau de jeu
    
    // recup decks
    //Deck_level_one* Deck1 = Deck_level_one::getInstance();
    //Deck_level_two* Deck2 = Deck_level_two::getInstance();
    //Deck_level_three* Deck3 = Deck_level_three::getInstance();
    //Pyramid_Cards* pyramide = Pyramid_Cards::getInstance(Deck_level_one::getInstance(), Deck_level_two::getInstance(), Deck_level_three::getInstance()).takeCard(card.getLevel(), position);
    //Pyramid_Cards* pyramide = Pyramid_Cards::getInstance(Deck1, Deck2, Deck3);
    //pyramide.takeCard(card.getLevel(), position);
}


//Celine
// ajout de la carte dans jewelryCards + modif de la carte resumé correspondant (couleur)
void Player::addJewelryCard(JewelryCard &card){
    // ajout de la carte dans la liste de cartes joailleries
    getJewelryCards().push_back(card);

    //ajout des points de prestiges dans summary carte + dans attribut prestigePoints de Player
    if(int nbPrestiges = card.getPrestige()){
        addPrestige(nbPrestiges, card.getBonus().bonus_color);  
    }

    //ajout des couronnes
    if(int nbCrowns = card.getCrowns()){
        addCrowns(nbCrowns);
    }
}

//Celine
// ajout de la carte dans royalCards 
void Player::addRoyalCard(RoyalCard &card){
    // ajout de la carte dans la liste de cartes royales
    getRoyalCards().push_back(card);

    // pas de couleur, juste ajout du nb de prestiges
    addPrestige(card.getPrestige(), TokenColor::None);


}

// Celine 
void Player::actionBuyReservedCard(JewelryCard &card, std::unordered_map<TokenColor, int> tokensToSpend){
    auto it = std::find(getReserve().begin(), getReserve().end(), card);
    if (it != getReserve().end()) {
        // achat de la carte
        spendResources(tokensToSpend);
        // on met la carte dans jewelryCards
        addJewelryCard(*it);
        // on retire la carte de la reserve
        getReserve().erase(it);

    }

}

Player::Player(std::string& n, Type t) {
    name = n;
    type = t;
    prestigePoints = 0;
    nbCrown = 0;
    nbTokens = 0;
    tokenSummary = {{TokenColor::BLEU, 0}, {TokenColor::BLANC, 0}, {TokenColor::VERT, 0}, {TokenColor::NOIR, 0}, {TokenColor::ROUGE, 0}, {TokenColor::OR, 0}, {TokenColor::PERLE, 0}};
    tokens = {{TokenColor::BLEU, std::vector<const Token*>()}, {TokenColor::BLANC, std::vector<const Token*>()}, {TokenColor::VERT, std::vector<const Token*>()}, {TokenColor::NOIR, std::vector<const Token*>()}, {TokenColor::ROUGE, std::vector<const Token*>()}, {TokenColor::OR, std::vector<const Token*>()}, {TokenColor::PERLE, std::vector<const Token*>()}};
    blackSummary = SummaryCard(0, 0);
    blueSummary = SummaryCard(0, 0);
    greenSummary = SummaryCard(0, 0);
    redSummary = SummaryCard(0, 0);
    whiteSummary = SummaryCard(0, 0);
}
