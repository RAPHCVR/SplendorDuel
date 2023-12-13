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
    TokenWidget(QWidget* parent, const TokenColor* color);

    void paintEvent(QPaintEvent* event) override;

    void setColor(const TokenColor* color);

    void updateNumToken(const TokenColor* color);

private:
    const TokenColor* color;
    QLCDNumber* numToken;
};

//widget pour les cartes résumés
class CardWidget : public QWidget {
    Q_OBJECT

public:
    CardWidget(const TokenColor color, QWidget *parent = nullptr);

    void updateBonus(SummaryCard carte);

    void updatePrestige(SummaryCard carte);


private:
    QLCDNumber *bonus;
    QLCDNumber *prestige;
};

//Widget regroupant toutes les infos de joueur
class PlayerQT : public QWidget {
    Q_OBJECT

public:
    PlayerQT(const Player player, QWidget *parent = nullptr);

    void updatePrivilege(const Player player);

    void updateCrown(const Player player);

    void updateTotalPrestige(const Player player);


private:
    QLabel* joueur; // texte "Joueur"
    //QLCDNumber* numJoueur; // 1 ou 2
    QLabel* nameJoueur; //nom du joueur
    QLabel* cartes; // texte "Cartes : "
    QLabel* jetons; // texte "Jetons : "
    QLabel* Privilege; // texte "Privileges : "
    QLCDNumber* numPrivilege; // 0<=x<=3
    QLabel* Prestige; // texte "Privileges : "
    QLCDNumber* numPrestige; // 0<=x<=3
    QLabel* couronne; // texte "Couronnes : "
    QLCDNumber* numCrowns;
    QHBoxLayout* layoutjoueur; // Joueur 1 ou 2
    QHBoxLayout* layoutprivilege; // Privilege : 1
    QHBoxLayout* layoutcouronne; // Couronnes : 1
    QGridLayout* layoutCards; // grille des cartes
    QGridLayout* layoutTokens; // grille des jetons
    QVBoxLayout* layoutMainJoueur;
    QPushButton* accesReserve; //boutton donnant acces a la reserve
    std::vector<CardWidget*> SummaryCards; //tableau des cartes resumes
    std::vector<TokenWidget*> SummaryTokens; //tableau des jetons resumes
};

#endif // QTJOUEUR_H
