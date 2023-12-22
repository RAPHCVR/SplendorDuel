#ifndef TEST_JOUEUR_H
#define TEST_JOUEUR_H
#include "Jeton.h"
#include "Cards.h"
#include "strategy.h"

#include <iostream>
#include <map>
//#include <qminmax.h>
#include <vector>
#include <unordered_map>
//#include les fichiers qui appelent les autres classes


enum class Type {IA, Humain};
std::string toString(Type t);
Type toType(std::string s);

class Player {
private:
    std::string name;
    std::array<const Privilege*, 3> privileges{};
    Type type;
    int nbCrown;
    int prestigePoints;
    int nbTokens;
    std::vector<JewelryCard*> jewelryCards; //Pas sur de l utilisation de vector
    std::vector<JewelryCard*> reserve;
    std::vector<RoyalCard*> royalCards;
    std::unordered_map<TokenColor, int> tokenSummary;
    //vector<vector<Token *>> tokens; //tokens[0] : liste des redTokens; 1 : goldTokens; 2 : blueTokens; 3 : pearlTokens; 4 : greenTokens; 5 : blackTokens; 6 : whiteTokens
    // autre idee pour rpz de token :
    std::unordered_map<TokenColor, std::vector<const Token*>> tokens;
    SummaryCard blueSummary;
    SummaryCard greenSummary;
    SummaryCard redSummary;
    SummaryCard whiteSummary;
    SummaryCard blackSummary;
    
    Strategy* strategy;

public:
    std::string getName() const {return name;};
    int getNbPrivilege() const;
    int getNbTokens() const {return nbTokens;};
    unsigned int getPrestige() const {return prestigePoints;};
    unsigned int getCrowns() const {return nbCrown;};

    Type getType() const {return type;}
    SummaryCard& getBlueSummary(){return blueSummary;}
    SummaryCard& getWhiteSummary(){return whiteSummary;}
    SummaryCard& getGreenSummary(){return greenSummary;}
    SummaryCard& getBlackSummary(){return whiteSummary;}
    SummaryCard& getRedSummary(){return redSummary;}
    unsigned int getMaxPrestigeColor(){return std::max({blueSummary.getPrestigePoints(),whiteSummary.getPrestigePoints(),greenSummary.getPrestigePoints(),blackSummary.getPrestigePoints(),redSummary.getPrestigePoints()});}

    std::vector<int> getBonusSummary();
    SummaryCard& getColorSummary(TokenColor color) {return color == TokenColor::BLEU ? blueSummary : color == TokenColor::BLANC ? whiteSummary : color == TokenColor::VERT ? greenSummary : color == TokenColor::NOIR ? blackSummary : redSummary;}
 
    std::unordered_map<TokenColor, int> getTokenSummary(){ return tokenSummary;}
    std::vector<JewelryCard*>& getJewelryCards(){ return jewelryCards;}
    std::vector<RoyalCard*>& getRoyalCards(){ return royalCards;}
    std::vector<JewelryCard*>& getReserve(){ return reserve;}
    //Strategy& getStrategy(){return strategy;}
    // ostream


    const Token& removeToken(TokenColor color); // appelé quand on achete une carte ou se fait voler un jeton ou au bout de 10 jetons
    const Privilege& removePrivilege(); // decrementer le nb de priviliege --> en cas de vol, de remplissage de plateau, de dépense du privilège
    //int prestigePerColor(); // retourne le total de prestige pour une couleur du joueur
    void addCrowns(int nbCrowns); // compter mes couronnes + prendre une carte couronne si crown = 3 ou 6 (--> appeler )
    void addPrestige(int points, TokenColor color); // compteur de tous mes prestiges (pour condition de victoire sur 20)
    void addPrivilege(const Privilege& privilege); // appelee en debut de partie si l'autre commence, si l'autre rempli le plateau, si j'achete une carte avec cette capacité
    void addJewelryCard( JewelryCard &card); //Pour simplifier buyCard  ajout de ma carte achetér au tas de mes cartes
    void addRoyalCard(RoyalCard &card, int position);// ajout d'une carte royale a mon inventaire
    void addToken(const Token &token); // ajout d'un jeton a mon inventaire

    // action obligatoires (acheter une carte et/ou prendre des jetons et/ou reserver une carte)
    void actionAddToken(); // prendre les jetons sur le plateau
    void reserveOneCard(JewelryCard& card); // ajout d'une carte dans la reserve
    bool canReserveCard();//verifie qu'on peut reserver une carte

    void actionBuyCard(JewelryCard &card); //Peut-etre besoin d'une carte ? prix, utilisation de la capacité... + retirer la carte du jeu (voir si on la fait nous ou dans la classe carte)
    bool canBuyCard(JewelryCard &card); 
    void spendResources(std::unordered_map<TokenColor, int> tokensToSpend);
    void spendSpecificToken(TokenColor color, int number);
    void spendGoldTokens(int number);
    void actionBuyReservedCard(JewelryCard &card);
    bool canbuyreservedcard();
    // actions optionnelles (remplir plateau, utiliser un priviliege pour acheter un jeton)
    //void usePrivilege(); Impmémenté dans controller

    ~Player()= default; // regarder si besoin
    Player(const Player& j)=delete; //interdire constructeur de recopie
    Player&& operator=(const Player& j)=delete; //interdire opérateur d'affectation
    Player(std::string& n, Type t);


    // voler jeton
    //
    bool operator==(const Player& other) const {
        return name == other.name &&
               privileges == other.privileges &&
               type == other.type &&
               nbCrown == other.nbCrown &&
               prestigePoints == other.prestigePoints &&
               nbTokens == other.nbTokens &&
               jewelryCards == other.jewelryCards &&
               reserve == other.reserve &&
               royalCards == other.royalCards &&
               tokenSummary == other.tokenSummary &&
               tokens == other.tokens;
    }
    Strategy* getStrategy(){ return strategy;}

};
std::ostream& operator<<(std::ostream& f, Player& j);

#endif //TEST_JOUEUR_H
