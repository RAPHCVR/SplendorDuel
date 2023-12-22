#ifndef STRATEGY_H
#define STRATEGY_H

#include "Jeton.h"
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

enum class OptionalActions{
    UsePrivileges, FillBoard, Empty
};

enum class CompulsoryActions{
    TakeCoins, ReserveCard, BuyCard
};

class Strategy{
public:
    virtual ~Strategy() = default;
    virtual int choicemaker(int min, int max)=0;
    virtual std::vector<OptionalActions> choseOptionalActions()=0;
    virtual CompulsoryActions choseCompulsoryAction()=0;
    virtual std::vector<std::pair<int, int>> choseTokensToTake()=0;
    virtual TokenColor choseTokenColor(std::vector<TokenColor>& chosableColors)=0;
};


class HumanStrategy : public Strategy{
public:
    int choicemaker(int min, int max) override;
    std::vector<OptionalActions> choseOptionalActions() override;
    CompulsoryActions choseCompulsoryAction() override;
    std::vector<std::pair<int, int>> choseTokensToTake() override;
    TokenColor choseTokenColor(std::vector<TokenColor>& chosableColors) override;
};

class AiStrategy : public Strategy{
public:
    int choicemaker(int min, int max) override;
    std::vector<OptionalActions> choseOptionalActions() override;
    CompulsoryActions choseCompulsoryAction() override;
    std::vector<std::pair<int, int>> choseTokensToTake() override;
    TokenColor choseTokenColor(std::vector<TokenColor>& chosableColors) override;
};

#endif //STRATEGY_H
