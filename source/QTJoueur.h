#ifndef QTJOUEUR_H
#define QTJOUEUR_H

#include <QtWidgets>
#include "Controller.h"
#include <QLCDNumber>
#include "QTCards.h"

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

//Widget pour la reserve
class reserveQT : public QWidget {
Q_OBJECT

public:
    enum ReserveStatus {
        buyable,
        notClickable
    };

    ReserveStatus getStatus() const { return status; }
    void setStatus(ReserveStatus newStatus) { status = newStatus; std::cout << "test12" << std::endl;}
    bool isReserveCardBuyable() const;

    reserveQT(JewelryCard* jewelryCard, QWidget* parent = nullptr);

public slots:
    void onSelectionButtonClicked();

private:
    JewelryCard* jewelryCard;
    Carte* carteWidget;  // Instance of Carte to display the JewelryCard
    QPushButton* selectionButton;
    QVBoxLayout* mainLayout;  // Main layout to control the size of Carte
    ReserveStatus status;

signals:
    void acheterReserveClicked(JewelryCard* jewelryCard);
    void closePopup();
};


//Widget regroupant toutes les infos de joueur
class PlayerQT : public QWidget {
Q_OBJECT

public:

    PlayerQT(Player &p, QWidget *parent = nullptr);

    void updatePrivilege();

    void updateCrown();

    void updateTotalPrestige();

    void updateTokens();

    void updateCards();

    void updateAllPlayer();

    Player& getPlayer() {return player;}

    void updateAllReserveStatus(reserveQT::ReserveStatus newStatus);

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
    QVBoxLayout* layoutFullCards;
    QVBoxLayout* layoutFullTokens;
    QPushButton* popupButtonReserve; //boutton donnant acces a la reserve
    QDialog* popupDialog;
    QHBoxLayout* popupLayout;
    QLabel* test;
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

    Player &player;

    std::vector<reserveQT*> reserveWidgets;
public:
    void toggleTextBoldJoueur(bool isBold);
    void showPopup(bool update=false); // Function to show the popup when the button is clicked
signals:
    void reserveCardSelected(JewelryCard* jewelryCard);
public slots:
    void onReserveCardSelected(JewelryCard* selectedCard);
};




#endif // QTJOUEUR_H
