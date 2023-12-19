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
    void bookCard(GameTable& gametable);
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

public slots:
    void handleBuyingJewelryCard(Carte* cardclicked);
    void handleBookingJewelryCardFromPyramid(Carte* clickedCard);
    void handleBookingJewelryCardFromPioche(QTPioche* piocheclicked);
    void handleBuyingRoyalCard(QTCardRoyal* cardclicked);
};


class QTStartingMenu : public QDialog {
    Q_OBJECT

public:
    QTStartingMenu(QWidget *parent = nullptr);

    QString getPlayerName1() {return playerName1;}
    QString getPlayerName2() {return playerName2;}

private:
    QString playerName1;
    QString playerName2;

public slots:
    void startNewGame() {
        // Obtenir les noms des joueurs
        QString playerName1 = QInputDialog::getText(this, "Nouvelle partie", "Nom du joueur 1 :");
        QString playerName2 = QInputDialog::getText(this, "Nouvelle partie", "Nom du joueur 2 :");

        accept();
        qDebug() << "Nouvelle partie avec les joueurs : " << playerName1 << " et " << playerName2;
    }

    void loadGame() {
        // Logique pour charger une partie sauvegardée
        qDebug() << "Charger une partie sauvegardée";
    }

    void quitGame() {
        std::exit(0);
    }
};

QString MBox(const std::vector<QString>& buttonLabels = {"OK"}, const QString& title = "Message", const QString& text = "Message");
int MBox(const std::vector<OptionalActions>& buttonLabels , const QString& title = "Message", const QString& text = "Message");
int MBox(const std::vector<CompulsoryActions>& buttonLabels , const QString& title = "Message", const QString& text = "Message");
void showWarningMessage(const QString &title, const QString &content);
void showVictoryDialog(const QString &playerName);
int getNumberBetween(int x, int y, const QString &message, QWidget *parent = nullptr);
#endif //QTGAME_H

