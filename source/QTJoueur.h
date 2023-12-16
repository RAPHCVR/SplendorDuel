#ifndef QTJOUEUR_H
#define QTJOUEUR_H

#include <QtWidgets>
#include <QColor>
#include <QPalette>
#include "Controller.h"
#include <QLCDNumber>

//widget pour les tokens résumés
class TokenWidget : public QWidget {
public:
    TokenWidget(QWidget* parent, TokenColor color);

    void paintEvent(QPaintEvent* event) override;

    void updateNumToken(int newNumToken);

private:
    const TokenColor color;
    QLCDNumber* numToken;
};

//widget pour les cartes résumés
class CardWidget : public QWidget {
    Q_OBJECT

public:
    explicit CardWidget(QWidget *parent = nullptr, TokenColor color=TokenColor::None);

    void updateBonus(int newBonus);

    void updatePrestige(int newPrestige);

    TokenColor getColor(){return color;};


private:
    TokenColor color;
    QLCDNumber *bonus;
    QLCDNumber *prestige;
};

//Widget regroupant toutes les infos de joueur
class PlayerQT : public QWidget {
    Q_OBJECT

public:
    PlayerQT(Player &player, QWidget *parent = nullptr);

    void updatePrivilege(Player &player);

    void updateCrown(Player &player);

    void updateTotalPrestige(Player &player);

    void updateTokens(Player &player);

    void updateCards(Player &player);

private:
    QLabel* typeJoueur; // texte "Humain ou IA"
    QLabel* nameJoueur; //nom du joueur
    QLabel* cartes; // texte "Cartes : "
    QLabel* jetons; // texte "Jetons : "
    QLabel* Privilege; // texte "Privileges : "
    QLCDNumber* numPrivilege; // 0<=x<=3
    QLabel* Prestige; // texte "Prestige : "
    QLCDNumber* numPrestige; // prestige total du joueur
    QLabel* couronne; // texte "Couronnes : "
    QLCDNumber* numCrowns;
    QHBoxLayout* layoutjoueur; // Joueur 1 ou 2
    QHBoxLayout* layoutprivilege; // Privilege : 1
    QHBoxLayout* layoutprestige; // Prestige total : 1
    QHBoxLayout* layoutcouronne; // Couronnes : 1
    QGridLayout* layoutCards; // grille des cartes
    QGridLayout* layoutTokens; // grille des jetons
    QVBoxLayout* layoutMainJoueur;
    QPushButton* accesReserve{}; //boutton donnant acces a la reserve
    std::vector<CardWidget*> SummaryCards; //tableau des cartes resumes
    std::vector<TokenWidget*> SummaryTokens; //tableau des jetons resumes

    TokenWidget* blueToken{};
    TokenWidget* redToken{};
    TokenWidget* blackToken{};
    TokenWidget* whiteToken{};
    TokenWidget* pearlToken{};
    TokenWidget* greenToken{};
    TokenWidget* goldToken{};

    CardWidget* blueCard{};
    CardWidget* redCard{};
    CardWidget* greenCard{};
    CardWidget* blackCard{};
    CardWidget* whiteCard{};
};

#endif // QTJOUEUR_H