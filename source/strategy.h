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
    virtual std::vector<OptionalActions> choseOptionalActions()=0;
    virtual std::vector<CompulsoryActions> choseCompulsoryActions()=0;
    virtual std::vector<const Token*> choseTokensToTake()=0;
    virtual TokenColor choseTokenColor(std::vector<TokenColor>& chosableColors)=0;
};


class HumanStrategy : public Strategy{
public:
    virtual std::vector<OptionalActions> choseOptionalActions() override;
    virtual std::vector<CompulsoryActions> choseCompulsoryActions() override;
    virtual std::vector<const Token*> choseTokensToTake() override;
    virtual TokenColor choseTokenColor(std::vector<TokenColor>& chosableColors) override;
};

class AiStrategy : public Strategy{
public:
    virtual int random(int min, int max) override;
    virtual std::vector<OptionalActions> choseOptionalActions() override;
    virtual std::vector<CompulsoryActions> choseCompulsoryActions() override;
    virtual std::vector<const Token*> AiStrategy::choseTokensToTake() override;
    virtual TokenColor choseTokenColor(std::vector<TokenColor>& chosableColors) override;
};

#endif //STRATEGY_H
