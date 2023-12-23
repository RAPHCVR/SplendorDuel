//
// Created by utcpret on 13/12/2023.
//

#ifndef QTGAME_H
#define QTGAME_H
#include <QWidget>
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
    void setBoldCurrentPlayer();
    void generateNewGame();
    PlayerQT* getCurrentQTPlayer();
    bool isCurrentPlayerIA();
    void handleIATokenSelection();

public slots:
    void handleBuyingJewelryCard(Carte* cardclicked);
    void handleBookingJewelryCardFromPyramid(Carte* clickedCard);
    void handleBookingJewelryCardFromPioche(QTPioche* piocheclicked);
    void handleBuyingRoyalCard(QTCardRoyal* cardclicked);
    void handleBuyingReserveCard(JewelryCard* clickedCard);
};

class QTStartingMenu : public QDialog {
    Q_OBJECT

public:
    QTStartingMenu(QWidget *parent = nullptr) : QDialog(parent) {
        // Create widgets
        playerNameEdit1 = new QLineEdit(this);
        playerNameEdit2 = new QLineEdit(this);
        playerTypeCombo1 = new QComboBox(this);
        playerTypeCombo2 = new QComboBox(this);
        startButton = new QPushButton("Lancer une partie", this);
        loadButton = new QPushButton("Charger une partie sauvegardée", this);
        quitButton = new QPushButton("Quitter", this);

        // Populate combo boxes
        playerTypeCombo1->addItem("Humain", QVariant::fromValue(Type::Humain));
        playerTypeCombo1->addItem("IA", QVariant::fromValue(Type::IA));
        playerTypeCombo2->addItem("Humain", QVariant::fromValue(Type::Humain));
        playerTypeCombo2->addItem("IA", QVariant::fromValue(Type::IA));

        // Create layout and add widgets
        QFormLayout *layout = new QFormLayout(this);
        layout->addRow(new QLabel("Nom du Joueur 1:"), playerNameEdit1);
        layout->addRow(new QLabel("Type du Joueur 1:"), playerTypeCombo1);
        layout->addRow(new QLabel("Nom du Joueur 2:"), playerNameEdit2);
        layout->addRow(new QLabel("Type du Joueur 2:"), playerTypeCombo2);
        layout->addWidget(startButton);
        layout->addWidget(loadButton);
        layout->addWidget(quitButton);

        // Connect signals to slots
        connect(startButton, &QPushButton::clicked, this, &QTStartingMenu::startNewGame);
        connect(loadButton, &QPushButton::clicked, this, &QTStartingMenu::loadGame);
        connect(quitButton, &QPushButton::clicked, this, &QTStartingMenu::quitGame);
    }

    QString getPlayerName1() { return playerNameEdit1->text(); }
    QString getPlayerName2() { return playerNameEdit2->text(); }
    Type getPlayerType1() { return playerTypeCombo1->currentData().value<Type>(); }
    Type getPlayerType2() { return playerTypeCombo2->currentData().value<Type>(); }
    bool isLoad() { return load; }
private:
    QLineEdit *playerNameEdit1;
    QLineEdit *playerNameEdit2;
    QComboBox *playerTypeCombo1;
    QComboBox *playerTypeCombo2;
    QPushButton *startButton;
    QPushButton *loadButton;
    QPushButton *quitButton;

public slots:
    void startNewGame() {
        playerName1 = getPlayerName1();
        playerName2 = getPlayerName2();
        playerType1 = playerTypeCombo1->currentText();
        playerType2 = playerTypeCombo2->currentText();
        load = false;
        accept();
        qDebug() << "New game with players: " << playerName1 << "(" << playerType1 << ")"
                 << " and " << playerName2 << "(" << playerType2 << ")";
    }

    void loadGame() {
        // Logic to load a saved game
        qDebug() << "Load a saved game";
        load = true;
    }

    void quitGame() {
        std::exit(0);
    }

private:
    QString playerName1;
    QString playerName2;
    QString playerType1;
    QString playerType2;
    bool load;
};


QString MBox(const std::vector<QString>& buttonLabels = {"OK"}, const QString& title = "Message", const QString& text = "Message");
int MBox(const std::vector<OptionalActions>& buttonLabels , const QString& title = "Message", const QString& text = "Message");
int MBox(const std::vector<CompulsoryActions>& buttonLabels , const QString& title = "Message", const QString& text = "Message");
void showWarningMessage(const QString &title, const QString &content);
void showVictoryDialog(const QString &playerName, QTGame *gameInstance);
int getNumberBetween(int x, int y, const QString &message, QWidget *parent = nullptr);
void clearWidgetAndSetNewLayout(QWidget* parentWidget, QLayout* newLayout);
void clearLayout(QLayout* layout);
#endif //QTGAME_H

