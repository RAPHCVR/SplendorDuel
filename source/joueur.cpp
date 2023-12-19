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
    tokenSummary.find(token.getColor())->second+=1;
    nbTokens++;
    tokens.find(token.getColor())->second.push_back(&token);
}

// Celine
// ajout d'une carte dans la reserve
void Player::reserveOneCard(JewelryCard& card){
    getReserve().push_back(&card);
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
    unsigned int nb = getNbPrivilege();
    privileges[nb]=&privilege;
}

unsigned int Player::getNbPrivilege() const {
    unsigned int nb = privileges.size();
    for (auto privilege : privileges) {
        if (privilege == nullptr) {
            nb--;
        }
    }
    return nb;
}

const Privilege& Player::removePrivilege() {
    unsigned int nb = getNbPrivilege();
    if (nb == 0) {
        throw std::runtime_error("Pas de privilège disponible");
    }
    const Privilege& privilege = *privileges[nb-1];
    privileges[nb-1] = nullptr;
    return privilege;
}

void Player::addCrowns(int nbCrowns) {
    nbCrown+=nbCrowns;
}

// celine
// retire un jeton de couleur "color"
// retourne un token pour permettre de le remettre dans le sac si 11 eme jeton 
// ou le donner a un autre joueur si vol
const Token& Player::removeToken(TokenColor color) {
    //retire dans le resume de jetons
    tokenSummary.at(color)-=1;
    //retire dans le total de jetons
    nbTokens--;

    // retire jeton dans inventaire
    const Token* token = tokens.at(color).back();
    // on retire le jeton du dico de jeton
    tokens.at(color).pop_back();
    return *token;
}


//Celine
// méthode pour retirer les ressources nécessaires lorsque le joueur achète une carte
void Player::spendResources(std::unordered_map<TokenColor, int> tokensToSpend){
    unsigned int goldcounter = tokenSummary[TokenColor::OR];
    SummaryCard* bonus;
    for(auto const& [color, tokensNeeded] : tokensToSpend) {
        if (tokensNeeded != 0) {
            bonus = &getColorSummary(color);
            int finalCost = tokensNeeded - bonus->getBonusNumber();
            int playerTokens = tokenSummary[color];
            if(playerTokens < finalCost) {
                int tokenGap = finalCost - playerTokens;
                if (goldcounter >= tokenGap) {
                    goldcounter -= tokenGap;
                    tokenSummary[color] = tokenSummary[color]  - finalCost + tokenGap;
                    tokenSummary[TokenColor::OR] -= tokenGap;
                    nbTokens -= tokenGap;
                    for(int i = 0; i < finalCost - tokenGap; i++) {
                        Bag::getInstance().addToken(*tokens[color].back());
                        tokens[color].pop_back();
                        nbTokens--;
                    }
                    for (int i = 0; i < tokenGap; i++) {
                        Bag::getInstance().addToken(*tokens[TokenColor::OR].back());
                        tokens[TokenColor::OR].pop_back();
                        nbTokens--;
                    }
                }
                else {
                    // If the player does not have enough tokens, throw an exception or return an error
                    throw std::runtime_error("Not enough tokens to spend");
                }
            }
            else {
                if (finalCost>0){
                    // Subtract the tokens needed from the player's tokens
                    tokenSummary[color] -= finalCost;
                    // Return the tokens to the bag
                    for(int i = 0; i < finalCost; i++) {
                        Bag::getInstance().addToken(*tokens[color].back());
                        tokens[color].pop_back();
                        nbTokens--;
                    }
                }
            }
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
    // Retrieve the cost of the card
    std::unordered_map<TokenColor, int> cost = card.getCost();

    // Retrieve the player's tokens and bonuses
    std::unordered_map<TokenColor, int> playerTokens = getTokenSummary();
    SummaryCard* bonus;
    unsigned int playerValue = 0;
    // Check if the player has enough tokens and bonuses to cover the cost of the card
    int goldAvailable = playerTokens[TokenColor::OR];
    for(auto const& [color, costValue] : cost) {
        if (color == TokenColor::PERLE) {
            playerValue = playerTokens[color];
        }
        else {
            bonus = &getColorSummary(color);
            playerValue = playerTokens[color] + bonus->getBonusNumber();
        }
        if(playerValue < costValue) {
            // If the player does not have enough tokens and bonuses, check if they have enough gold tokens to cover the remaining cost
            int goldTokensNeeded = costValue - playerValue;
            if(goldAvailable < goldTokensNeeded) {
                // If the player does not have enough gold tokens, they cannot buy the card
                return false;
            }
            else {
                goldAvailable -= goldTokensNeeded;
            }
        }
    }

    // If the player has enough resources to buy the card, return true
    return true;
}


//Celine
// retire et place dans le sac les jetons dépensés lors de l’achat,
// et ajoute la carte à la liste des cartes possédées par le joueur. 
//void Player::actionBuyCard(JewelryCard &card, int position, std::unordered_map<TokenColor, int> tokensToSpend){
void Player::actionBuyCard(JewelryCard &card){
    // Retirer les ressources nécessaires
    std::unordered_map<TokenColor, int> tokensToSpend = card.getCost();
    spendResources(tokensToSpend);

    // Ajouter la carte au joueur
    addJewelryCard(card);
}


//Celine
// ajout de la carte dans jewelryCards + modif de la carte resumé correspondant (couleur)
void Player::addJewelryCard(JewelryCard &card) {
    // ajout de la carte dans la liste de cartes joaillerie
    getJewelryCards().push_back(&card);

    //ajout des points de prestiges dans summary carte + dans attribut prestigePoints de Player
    if(card.getPrestige()!=0){
        addPrestige(card.getPrestige(), card.getBonus().bonus_color);
    }

    //ajout des bonus dans summary carte
    if(card.getBonus().bonus_number!=0){
        getColorSummary(card.getBonus().bonus_color).addBonusNumber(card.getBonus().bonus_number);
    }

    //ajout des couronnes
    if(card.getCrowns()!=0){
        addCrowns(card.getCrowns());
    }

}

//Celine
// ajout de la carte dans royalCards 
void Player::addRoyalCard(RoyalCard &card, int position){
    // ajout de la carte dans la liste de cartes royales
    getRoyalCards().push_back(&card);
    //delete the card
    Deck_Royal::getInstance()->deleteCard(position);
    // pas de couleur, juste ajout du nb de prestiges
    addPrestige(card.getPrestige(), TokenColor::None);
}

// Celine 
void Player::actionBuyReservedCard(JewelryCard &card){
    // Retirer les ressources nécessaires
    std::unordered_map<TokenColor, int> tokensToSpend = card.getCost();
    spendResources(tokensToSpend);

    // Ajouter la carte au joueur
    addJewelryCard(card);

    // Retirer la carte de la réserve
    auto it = std::find(reserve.begin(), reserve.end(), &card);
    if (it != reserve.end()) {
        reserve.erase(it);
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
    strategy = 0 ;
}

std::ostream& operator<<(std::ostream& f, Player& p) {
    f << "Joueur :" << p.getName() << " : " << std::endl;
    f << "Couronnes : " << p.getCrowns() << std::endl;
    f << "Prestige : " << p.getPrestige() << std::endl;
    f << "Jetons : " << std::endl;
    for (auto const& [color, tokens] : p.getTokenSummary()) {
        f << color << " : " << tokens << std::endl;
    }
    f << "Cartes joaillerie : " << std::endl;
    for (auto const& card : p.getJewelryCards()) {
        f << *card << std::endl;
    }
    f << "Cartes royales : " << std::endl;
    for (auto const& card : p.getRoyalCards()) {
        f << *card << std::endl;
    }
    f << "Cartes en reserve : " << std::endl;
    for (auto const& card : p.getReserve()) {
        f << *card << std::endl;
    }
    return f;
}

std::string toString(Type t) {
    switch (t) {
        case Type::Humain:
            return "Humain";
        case Type::IA:
            return "IA";
    }
}