#ifndef TEST_JOUEUR_H
#define TEST_JOUEUR_H

#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>
#include "joueur.h"
#include "Jeton.cpp"
#include "Jeton.h"
#include "Cards.h"
#include "Cards.cpp"


using namespace std;

enum class Type {IA, Humain};



class Player {
private:
    string name;
    int privilege;
    Type type;
    int nbCrown;
    int prestigePoints;
    //int nbTokens;
    vector<JewelryCard> cardsJewelry; //Pas sur de l utilisation de vector
    vector<JewelryCard> reserve;
    vector<RoyalCard> cardsRoyal;
    unordered_map<TokenColor, int> tokenSummary;
    vector<vector<Token *>> tokens; // ordre (bleu,blanc,vert,noir,rouge, perle, or)


    SummaryCard blueSummary;
    SummaryCard whiteSummary;
    SummaryCard greenSummary;
    SummaryCard blackSummary;
    SummaryCard redSummary;

public:
    string getName() const {return name;}
    int getPrivilege() const {return privilege;}
    int getPrestige() const {return prestigePoints;}
    int getCrowns() const {return nbCrown;}
    //int getTokens() const {return nbTokens;};
    Type getType() const {return type;}
    SummaryCard getBlueSummary(){return blueSummary;}
    SummaryCard getWhiteSummary(){return whiteSummary;}
    SummaryCard getGreenSummary(){return greenSummary;}
    SummaryCard getBlackSummary(){return whiteSummary;}
    SummaryCard getRedSummary(){return redSummary;}

    vector<int> getBonusSummary();
 
    unordered_map<TokenColor, int> getTokenSummary(){ return tokenSummary;}


    // ostream


    void removeToken(Token &token); // appelé quand on achete une carte ou se fait voler un jeton ou au bout de 10 jetons
    //int prestigePerColor(); // retourne le total de prestige pour une couleur du joueur
    void addCrowns(); // compter mes couronnes + prendre une carte couronne si crown = 3 ou 6 (--> appeler )
    void addPrestige(int points, TokenColor color); // compteur de tous mes prestiges (pour condition de victoire sur 20)
    void addPrivilege(); // appelee en debut de partie si l'autre commence, si l'autre rempli le plateau, si j'achete une carte avec cette capacité
    void removePrivilege(); // decrementer le nb de priviliege --> en cas de vol
    void addJewelryCard(JewelryCard &card); //Pour simplifier buyCard  ajout de ma carte achetér au tas de mes cartes
    void addRoyalCard(RoyalCard &card);// ajout d'une carte royale a mon inventaire
    void addToken(Token &token); // ajout d'un jeton a mon inventaire

    // action obligatoires (acheter une carte et/ou prendre des jetons et/ou reserver une carte)
    void actionAddToken(); // prendre les jetons sur le plateau
    void actionReserveCard(); // retirer du deck; prendre un or (avec addToken);

    void actionBuyCard(JewelryCard &card); //Peut-etre besoin d'une carte ? prix, utilisation de la capacité... + retirer la carte du jeu (voir si on la fait nous ou dans la classe carte)
    bool canBuyCard(JewelryCard &card); 
    void spendResources(int red, int green, int blue, int black, int pearl, int white);

    // actions optionnelles (remplir plateau, utiliser un priviliege pour acheter un jeton)
    void usePrivilege(); // appelé au moment d'acheter un jeton

    ~Player(); // regarder si besoin
    Player(const Player& j)=delete; //interdire constructeur de recopie
    Player&& operator=(const Player& j)=delete; //interdire opérateur d'affectation
    Player(string name, Type type) : name(name), type(type), nbCrown(0), prestigePoints(0), nbTokens(0),tokenSummary({{"Rouge", 0},{"Bleu", 0},
                                                                                                                      {"Vert", 0},{"Noir", 0},{"Blanc", 0},
                                                                                                                      {"Or",0},{"Perle",0}}){}; // vector?

};

#endif //TEST_JOUEUR_H
