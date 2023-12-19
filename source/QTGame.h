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
#include "Controller.h"
#include "QTJeton.h"
#include "QTCards.h"
#include "QTJoueur.h"

class QTGame : public QWidget {
    Q_OBJECT
private:
    Controller* controller;
    PlateView* plateView;
    QTPyramid* pyramid;
    QTRangeePioches* pioches;
    QTBoardRoyal* boardRoyal;
    PrivilegeCounter* privilegeCounter;
    QVBoxLayout* mainlayout;
    QScreen* screen;
    QSize* size;
    PlayerQT* player1;
    PlayerQT* player2;
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
QString MBox(const std::vector<QString>& buttonLabels = {"OK"}, const QString& title = "Message", const QString& text = "Message");
int MBox(const std::vector<OptionalActions>& buttonLabels , const QString& title = "Message", const QString& text = "Message");
int MBox(const std::vector<CompulsoryActions>& buttonLabels , const QString& title = "Message", const QString& text = "Message");
void showWarningMessage(const QString &title, const QString &content);
void showVictoryDialog(const QString &playerName);
#endif //QTGAME_H
