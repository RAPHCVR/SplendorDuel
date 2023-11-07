#ifndef JEWELRYCARD_H
#define JEWELRYCARD_H

#include <iostream>
#include <string.h>

//PENSER A INCLUDE LES ENUMS POUR ABILITIES AINSI QUE BONUS

class JewelryCard {
public:
    std::string getLevel() {return level;}
    int getCost() {return cost;}
    int getPrestige() {return prestige_points;}
    int getCrowns() {return crowns;}
    Abilities getAbility() {return ability;}
    Bonus getBonus() {return bonus;}
private:
    unsigned int level;
    unsigned int cost;
    unsigned int prestige_points;
    unsigned int crowns;
    Abilities ability;
    Bonus bonus;
};
#endif /* JEWELRYCARD_H */

