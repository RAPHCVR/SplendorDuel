#ifndef STRATEGY_H
#define STRATEGY_H

#include "Jeton.h"
#include "Cards.h"
#include "joueur.h"
#include "Controller.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>

/*
rappels
actions obligatoire : 1 seule --> 3 jetons / resa + or / acheter carte
actions optionnelles : 0, 1 ou 2 --> utiliser privilege = prendre jeton (pas or) / remplir plateau + prendre un privilege
*/

/*
CHOIX :
- nb action optionnels + laquelle
- quelle action obligatoire
*/


class Strategy{
public:
    virtual ~Strategy() = default;
    virtual int random(int min, int max)=0 ;
    virtual vector<OptionalActions> choseOptionalActions()=0;

};


class HumanStrategy : public Strategy{
public:
    virtual vector<OptionalActions> choseOptionalActions() override;

};


class AiStrategy : public Strategy{
public:
    virtual int random(int min, int max) override;
    virtual vector<OptionalActions> choseOptionalActions() override;
    virtual vector<CompulsoryActions> choseCompulsoryActions() override;


};

#endif //STRATEGY_H
