//
// Created by utcpret on 13/12/2023.
//

#ifndef QTGAME_H
#define QTGAME_H
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScreen>
#include <QSize>
#include <QGridLayout>
#include "Controller.h"
#include "QTJeton.h"
#include "QTCards.h"
#include "Cards.h"

class QTGame : public QWidget {
    Q_OBJECT
private:
    Controller* controller;
    PlateView* plateView;
    QTPyramid* qtpyramid;
    QTRangeePioches* qtrangeepioches;
    QTBoardRoyal* qtboardroyal;

    QHBoxLayout* first;
    QHBoxLayout* second;
    QVBoxLayout* mainlayout;
    QGridLayout* maingrid;
    QScreen* screen;
    QSize* size;
    unsigned int width;
    unsigned int height;
    std::string status;
public:
    void quit() {
        this->close();
    }
    void paintEvent(QPaintEvent* event) override;
    QTGame(QWidget* parent = nullptr);
    void handleTokenSelection(std::vector<const Token*> tokens);
    void fillBoard();
    void takePrivilege(Player& player);
    void bookCard(Pyramid_Cards& pyramid, GameTable& gametable);
    void usePriviledge();
    void placePrivilege(unsigned int nb);
    void applyOptionalAction(OptionalActions action);
    void play();
    void playOptionalActions();
    void handleGameStatus();
    void playCompulsoryActions();
    void applyCompulsoryAction(CompulsoryActions action);
    void checkEndTurn();
    void applyCardSkills(Game&game, Player&cardOwner, Player&opponent, JewelryCard&card);
    void buyJewelryCard(GameTable& gametable);
    void buyNobleCard();
    void applyRoyalCardSkills(Game&game, Player&cardOwner, Player&opponent, RoyalCard&card);
};

#endif //QTGAME_H
