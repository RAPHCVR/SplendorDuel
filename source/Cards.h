#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <string.h>
#include "Jeton.h"



//Enum Abilities pour les capacités
enum Abilities {repeat_turn, cameleon, take_bonus_token, take_privilege, steal_token};

//Classe exception
class JewelryCardError {
public:
    int reason;
    error(int r) : reason(r) {}
    error(const error &source) : reason(source.reason) {}
};

class JewelryCard {
public:
    JewelryCard(l, c, pp, c, a, b) :
    level(l), cost(c), prestige_points(pp), crowns(c), ability(a), bonus(b) {}
    std::string getLevel() {return level;}
    int getCost() {return cost;}
    int getPrestige() {return prestige_points;}
    int getCrowns() {return crowns;}
    Abilities getAbility() {return ability;}
    TokenColor getBonus() {return bonus;}
private:
    unsigned int level;
    unsigned int cost;
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
    error(const error &source) : reason(source.reason) {}
};

class RoyalCard {
public:
    RoyalCard(pp, a) :
    prestige_points(pp), ability(a);
    int getPrestige() {return prestige_points;}
    Abilities getAbility() {return ability;}
private:
    unsigned int prestige_points;
    Abilities ability;    
};

#endif /* CARDS_H */

