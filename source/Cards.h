#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <string.h>
#include <vector>
#include "Jeton.h"

using namespace std;

//Enum Abilities pour les capacités
enum Abilities {repeat_turn, cameleon, take_bonus_token, take_privilege, steal_token};

struct Bonus {
    int bonus_number;
    TokenColor bonus_color;
};

class SummaryCard{
    
private :
    unsigned int prestigePoints;
    unsigned int bonusNumber;
    //int crownNumber;

public :
    int getPrestigePoints(){ return prestigePoints;}
    int getBonusNumber(){ return bonusNumber;}
    void addBonusNumber(unsigned int b);
    void addprestigePoints(unsigned int p);
    //int getCrownNumber(){ return crownNumber;}
};


//Classe exception
class JewelryCardError {
    //Classe de gestion des exception avec les carte de gemmes
private:
    std::string message; //Message d'exception
public:
    JewelryCardError(const std::string &msg) : message(msg) {} //Constructeur d'exception
    ~JewelryCardError() = default;

    std::string getMessage() const { return message; } //Récupération du message d'exception
};



class JewelryCard {
public:
    JewelryCard(unsigned int l, std::vector<int> c, unsigned int pp, unsigned int cr, Abilities a, Bonus b) :
    level(l), cost(c), prestige_points(pp), crowns(cr), ability(a), bonus(b) {}
    unsigned int getLevel() {return level;}
    std::vector<int> getCost() {return cost;}
    int getPrestige() {return prestige_points;}
    int getCrowns() {return crowns;}
    Abilities getAbility() {return ability;}
    Bonus getBonus() {return bonus;}
private:
    unsigned int level;
    std::unordered_map<TokenColor, int> cost; //dans l'ordre BLEU, BLANC, VERT, NOIR, ROUGE, PERLE (modifiable)
    unsigned int prestige_points;
    unsigned int crowns;
    Abilities ability;
    Bonus bonus;
};


class RoyalCardError {
    //Classe de gestion des exception avec les cartes royales
private:
    std::string message; //Message d'exception
public:
    RoyalCardError(const std::string &msg) : message(msg) {} //Constructeur d'exception
    ~RoyalCardError() = default;

    std::string getMessage() const { return message; } //Récupération du message d'exception
};

class RoyalCard {
public:
    RoyalCard(unsigned int pp, Abilities a) :
    prestige_points(pp), ability(a) {}
    unsigned int getPrestige() {return prestige_points;}
    Abilities getAbility() {return ability;}
private:
    unsigned int prestige_points;
    Abilities ability;
};


class Deck_level_one{ 
    public:
        friend class Pyramid_Cards;
    private:
        std::vector<JewelryCard *> pioche;
};

class Deck_level_two{
public:
    friend class Pyramid_Cards;
private:
    std::vector<JewelryCard *> pioche;
};

class Deck_level_three{
public:
    friend class Pyramid_Cards;
private:
    std::vector<JewelryCard *> pioche;
};

class Pyramid_Cards{
public:
    
    void drawCard(unsigned int level);
    JewelryCard& takeCard(unsigned int level, unsigned int position);
    
private:
    std::vector<JewelryCard *> row_level_one;
    std::vector<JewelryCard *> row_level_two;
    std::vector<JewelryCard *> row_level_three;
    static const unsigned int max_level_one = 5;
    static const unsigned int max_level_two = 4;
    static const unsigned int max_level_three = 3;
    /* Pas besoin car vecteur se stack automatiquement
    unsigned int position_level_one = 0;
    unsigned int position_level_two = 0;
    unsigned int position_level_three = 0;
     */ 


};

#endif /* CARDS_H */

