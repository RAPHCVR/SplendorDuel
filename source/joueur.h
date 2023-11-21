#ifndef TEST_JOUEUR_H
#define TEST_JOUEUR_H

#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>
//#include les fichiers qui appelent les autres classes

using namespace std;

enum class Type {IA, Humain};



class Joueur {
private:
    string name;
    int privilege;
    Type type;
    int nbCrown;
    int prestigePoints;
    int nbTokens;
    vector<JewelryCard> cardsJewelry; //Pas sur de l utilisation de vector
    vector<JewelryCard> reserve;
    vector<RoyalCard> cardsRoyal;
    unordered_map<string, int> tokenSummary;
    vector<vector<Token *>> tokens; // tokens[0] : liste des redTokens; 1 : goldTokens; 2 : blueTokens; 3 : pearlTokens; 4 : greenTokens; 5 : blackTokens; 6 : whiteTokens


    SummaryCard blueSummary;
    SummaryCard greenSummary;
    SummaryCard redSummary;
    SummaryCard whiteSummary;
    SummaryCard blackSummary;

public:
    string getName() const {return name;};
    int getPrivilege() const {return privilege;};
    int getPrestige() const {return prestigePoints;};
    int getCrowns() const {return nbCrown;};
    int getTokens() const {return nbTokens;};
    Type getType() const {return type;};

    // ostream


    void removeToken(Token token); // appelé quand on achete une carte ou se fait voler un jeton ou au bout de 10 jetons
    //int prestigePerColor(); // retourne le total de prestige pour une couleur du joueur
    void addCrowns(); // compter mes couronnes + prendre une carte couronne si crown = 3 ou 6 (--> appeler )
    void addPrestige(int points, TokenColor color); // compteur de tous mes prestiges (pour condition de victoire sur 20)
    void addPrivilege(); // appelee en debut de partie si l'autre commence, si l'autre rempli le plateau, si j'achete une carte avec cette capacité
    void removePrivilege(); // decrementer le nb de priviliege --> en cas de vol
    void addJewelryCard(JewelryCard card); //Pour simplifier buyCard  ajout de ma carte achetér au tas de mes cartes
    void addRoyalCard(RoyalCard card);// ajout d'une carte royale a mon inventaire
    void addToken(Token token); // ajout d'un jeton a mon inventaire

    // action obligatoires (acheter une carte et/ou prendre des jetons et/ou reserver une carte)
    void actionAddToken(); // prendre les jetons sur le plateau
    void actionReserveCard(); // retirer du deck; prendre un or (avec addToken);

    int actionBuyCard(JewelryCard &card); //Peut-etre besoin d'une carte ? prix, utilisation de la capacité... + retirer la carte du jeu (voir si on la fait nous ou dans la classe carte)
    bool canBuyCard(JewelryCard &card); 
    void spendResources(unordered_map<TokenColor, int> tokensToSpend);

    // actions optionnelles (remplir plateau, utiliser un priviliege pour acheter un jeton)
    void usePrivilege(); // appelé au moment d'acheter un jeton

    ~Player(); // regarder si besoin
    Player(const Player& j)=delete; //interdire constructeur de recopie
    Player&& operator=(const Player& j)=delete; //interdire opérateur d'affectation
    Player(string name, Type type) : name(name), type(type), nbCrown(0), prestigePoints(0),tokenSummary({{"Rouge", 0},{"Bleu", 0},
                                                                                                                      {"Vert", 0},{"Noir", 0},{"Blanc", 0},
                                                                                                                      {"Or",0},{"Perle",0}}){}; // vector?

    // voler jeton
    //
};

#endif //TEST_JOUEUR_H
