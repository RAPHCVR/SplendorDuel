//
// Created by world on 03/12/2023.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Partie.h"
#include "QTJeton.h"

enum class OptionalActions{
    UsePrivileges, FillBoard, Empty
};

enum class CompulsoryActions{
    TakeCoins, ReserveCard, BuyCard
};

class Controller {
private:
    Game* game;
    Player* currentPlayer = nullptr;
public:
    Controller();
    ~Controller() { delete game; }
    // getters
    Game& getGame() { return *game; };
    Player& getcurrentPlayer() { return *currentPlayer; }
    Player& getopposingPlayer() {for (int i = 0; i < 2; i++) { if (game->getPlayer(i) != currentPlayer) { return *game->getPlayer(i); } } }
    // setters
    void setCurrentPlayer(unsigned int n);
    // actions partie
    void play();
    void playTurn();
    void changeCurrentPlayer();

    std::vector<OptionalActions> getOptionalActions(const Game& game, Player& player) const;
    std::vector<CompulsoryActions> getCompulsoryActions(const Game& game, Player& player) const;
    void applyOptionalAction(Game& game, Player& player,  OptionalActions action);
    void applyCompulsoryAction(Game& game, Player& player,  CompulsoryActions action);
    void applyCardSkills(Game& game, Player& cardOwner, Player& opponent, JewelryCard& card);
    void applyRoyalCardSkills(Game&game, Player&cardOwner, Player&opponent, RoyalCard&card);

    bool checkCardPurchase(const JewelryCard& card, GameTable& gametable);
    // Actions optionnelles
    void usePriviledge(Board& board);
    void fillBoard(Board& board, Bag& bag);

    // Actions obligatoires
    const Token& chooseToken(Board& board, Player& player, std::vector<std::pair<int, int>>* tokenIndexes = nullptr);
    void chooseGoldenToken(Board& board, Player& player);
    void buyNobleCard ();
    void buyJewelryCard(GameTable& gametable);
    void bookCard(Pyramid_Cards& pyramid, GameTable& gametable);
    bool checkIfPlayerWins(Game& game, Player& player);

    //gestion données
    void saveGame();
    void saveScore();

};

unsigned int choiceMaker(unsigned int a, unsigned int b);
bool areCoordinatesAlignedAndConsecutive(const std::vector<std::pair<int, int>>* coordinates);

#endif //CONTROLLER_H
