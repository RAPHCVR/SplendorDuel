#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <string.h>
#include <vector>
#include "Jeton.h"

using namespace std;


//Enum Abilities pour les capacités
enum Abilities {repeat_turn, cameleon, take_bonus_token, take_privilege, steal_token};

class SummaryCard{
    private :
        int prestigePoints;
        int crownNumber;
        int bonusNumber;

    public : 
        int getPrestigePoints(){ return prestigePoints;}   
        int getCrownNumber(){ return crownNumber;}
        int getBonusNumber(){ return bonusNumber;}
}


//Classe exception
class JewelryCardError {
public:
    int reason;
    error(int r) : reason(r) {}
    error(const JewelryCardError &source) : reason(source.reason) {}
};

class JewelryCard {
public:
    JewelryCard(unsigned int l, std::vector<int> c, unsigned int pp, unsigned int cr, Abilities a, TokenColor b) :
    level(l), cost(c), prestige_points(pp), crowns(cr), ability(a), bonus(b) {}
    std::string getLevel() {return level;}
    std::vector<int> getCost() {return cost;}
    int getPrestige() {return prestige_points;}
    int getCrowns() {return crowns;}
    Abilities getAbility() {return ability;}
    TokenColor getBonus() {return bonus;}
private:
    unsigned int level;
    std::vector<int> cost; //dans l'ordre BLEU, BLANC, VERT, NOIR, ROUGE, PERLE, OR (modifiable)
    unsigned int prestige_points;
    unsigned int crowns;
    Abilities ability;
    TokenColor bonus;
};

//Classe exception
class RoyalCardError {
public:
    int reason;
    error(int r) : reason(r) {}
    error(const RoyalCardError &source) : reason(source.reason) {}
};

class RoyalCard {
public:
    RoyalCard(unsigned int pp, Abilities a) :
    prestige_points(pp), ability(a) {}
    int getPrestige() {return prestige_points;}
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
    JewelryCard takeCard(unsigned int level, unsigned int position);
    
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

