#include "Jeton.h"
#include "Cards.h"
#include "joueur.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>

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
        if (Board::getInstance()->hasTokenOfColor(TokenColor::OR)){
            return true;
        }
    }
    return false;
}

void Player::addPrivilege(const Privilege& privilege) {
    unsigned int nb = getNbPrivilege();
    privileges[nb]=&privilege;
}

int Player::getNbPrivilege() const {
    int nb = 0;
    for (auto privilege : privileges) {
        if (privilege != nullptr) {
            nb++;
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
void Player::spendResources(std::unordered_map<TokenColor, int> tokensToSpend) {
    unsigned int goldCounter = tokenSummary[TokenColor::OR];

    for (const auto& [color, tokensNeeded] : tokensToSpend) {
        if (tokensNeeded == 0) {
            continue;
        }
        if (color!=TokenColor::PERLE) {
            SummaryCard& bonus = getColorSummary(color);
            int finalCost = tokensNeeded - bonus.getBonusNumber();
            int playerTokens = tokenSummary[color];

            if (finalCost <= 0) {
                continue; // No need to spend tokens if final cost is zero or negative
            }

            if (playerTokens < finalCost) {
                int tokenGap = finalCost - playerTokens;
                if (goldCounter < tokenGap) {
                    throw std::runtime_error("Not enough tokens to spend");
                }
                spendSpecificToken(color, playerTokens);
                spendGoldTokens(tokenGap);
            } else {
                spendSpecificToken(color, finalCost);
            }
        }
        else {
            int playerTokens = tokenSummary[color];
            if (playerTokens < tokensNeeded) {
                int tokenGap = tokensNeeded - playerTokens;
                if (goldCounter < tokenGap) {
                    throw std::runtime_error("Not enough tokens to spend");
                }
                spendSpecificToken(color, playerTokens);
                spendGoldTokens(tokenGap);
            } else {
                spendSpecificToken(color, tokensNeeded);
            }
        }
    }
}

void Player::spendSpecificToken(TokenColor color, int number) {
    for (int i = 0; i < number; ++i) {
        Bag::getInstance()->addToken(*tokens[color].back());
        tokens[color].pop_back();
        nbTokens--;
    }
    tokenSummary[color] -= number;
}

void Player::spendGoldTokens(int number) {
    for (int i = 0; i < number; ++i) {
        Bag::getInstance()->addToken(*tokens[TokenColor::OR].back());
        tokens[TokenColor::OR].pop_back();
        nbTokens--;
    }
    tokenSummary[TokenColor::OR] -= number;
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
    if (type == Type::IA) {
        strategy = new AiStrategy();
    }
    else {
        strategy = new HumanStrategy();
    }
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

Type toType(std::string s) {
    if (s == "Humain") {
        return Type::Humain;
    }
    else if (s == "IA") {
        return Type::IA;
    }
    else {
        throw std::runtime_error("Type de joueur inconnu");
    }
}

bool Player::canbuyreservedcard(){
    bool canbuy = false;
    for (auto card : getReserve()) {
        if (canBuyCard(*card)) {
            canbuy = true;
        }
    }
    return canbuy;
}

/* ------------------------------------------------------------------------------------------------------------------ */

Player::Player(const std::string & databaseSavePath, int id) {

    //cout << pathtodatabase << endl;
    sqlite3* db; //On créer une variable sqlite du nom de db

    int rc = sqlite3_open(databaseSavePath.c_str(), &db); //rc = return code, on ouvre la database

    if (rc) {
        std::cerr << "Erreur lors de l'ouverture de la base de données: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }
/*
    std::ostringstream oss;

    oss << "SELECT * FROM player WHERE idPlayer = " << id;
    std::string queryString = oss.str();  // Stocker la chaîne dans une variable
    const char* query = queryString.c_str();
    */
    const char* query = "SELECT * FROM player WHERE idPlayer = 1"; //requete pour chercher carte lvl 3

    // Le pointeur retourné par c_str() reste valide tant que queryString est en vie

    // Préparer la requête
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Erreur lors de la préparation de la requête: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    int idpl = sqlite3_column_int(stmt, 0);
    const char* nom = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    unsigned int couronnes = sqlite3_column_int(stmt, 5);

    name = std::to_string(*nom);
    type = toType(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    prestigePoints = sqlite3_column_int(stmt, 4);
    nbCrown = couronnes;
    tokenSummary = { {TokenColor::BLEU, sqlite3_column_int(stmt, 4)}, {TokenColor::BLANC, sqlite3_column_int(stmt, 5)}, {TokenColor::VERT, sqlite3_column_int(stmt, 6)}, {TokenColor::NOIR, sqlite3_column_int(stmt, 7)}, {TokenColor::ROUGE, sqlite3_column_int(stmt, 8)}, {TokenColor::OR, sqlite3_column_int(stmt, 10)}, {TokenColor::PERLE, sqlite3_column_int(stmt, 9)} };
    tokens = { {TokenColor::BLEU, std::vector<const Token*>()}, {TokenColor::BLANC, std::vector<const Token*>()}, {TokenColor::VERT, std::vector<const Token*>()}, {TokenColor::NOIR, std::vector<const Token*>()}, {TokenColor::ROUGE, std::vector<const Token*>()}, {TokenColor::OR, std::vector<const Token*>()}, {TokenColor::PERLE, std::vector<const Token*>()} };
    blackSummary = SummaryCard(sqlite3_column_int(stmt, 20), sqlite3_column_int(stmt, 19));
    blueSummary = SummaryCard(sqlite3_column_int(stmt, 14), sqlite3_column_int(stmt, 13));
    greenSummary = SummaryCard(sqlite3_column_int(stmt, 18), sqlite3_column_int(stmt, 17));
    redSummary = SummaryCard(sqlite3_column_int(stmt, 22), sqlite3_column_int(stmt, 21));
    whiteSummary = SummaryCard(sqlite3_column_int(stmt, 16), sqlite3_column_int(stmt, 15));
    nbTokens = 0;
    for (auto const& [color, tokens] : getTokenSummary()) {
        nbTokens += tokens;
    }
}