#ifndef TEST_JOUEUR_H
#define TEST_JOUEUR_H

#include <iostream>
#include <map>
#include <vector>

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
    vector<JewelryCard> cardsJ; //Pas sur de l utilisation de vector
    vector<CrownCard> cardsC;
    vector<Token> tokens;

public:
    string getName() const {return name;};
    int getPrivilege() const {return privilege;};
    int getPrestige() const {return prestigePoints;};
    int getCrowns() const {return nbCrown;};
    int getTokens() const {return nbTokens;};
    Type getType() const {return type;};

    void reserveCard(JewelryCard card);
    void buyCard(); //Peut-etre besoin d'une carte ?
    void removeToken(Token token);
    void usePrivilege(int privilege);
    int prestigePerColor();
    void addCrowns();
    void addPrestige();
    void addPrivilege();
    void addToken(Token token);
    void addJCard(JewelryCard card); //Pour simplifier buyCard
    void addCCard(CrownCard card);
};

#endif //TEST_JOUEUR_H
