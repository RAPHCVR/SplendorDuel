#ifndef ROYALCARD_H
#define ROYALCARD_H

class RoyalCard {
public:
    int getPrestige() {return prestige_points;}
    Abilities getAbility() {return ability;}
private:
    unsigned int prestige_points;
    Abilities ability;    
};

#endif /* ROYALCARD_H */

