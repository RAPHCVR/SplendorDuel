#ifndef QTCARDS_H
#define QTCARDS_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QColor>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include "sqlite/sqlite3.h"
#include <QString>
#include <QPixmap>
#include <QGridLayout>
#include <QLabel>
// Inclure cstddef pour utiliser std::byte de manière explicite
#include <cstddef>
#include "Jeton.h"
#include "Cards.h"

namespace Utility {

    QString toHexaDecimal(TokenColor c);

}

    class Carte : public QLabel {
        Q_OBJECT

    public:

        enum CardStatus {
            buyable,
            reservable,
            notClickable

        };

        CardStatus getStatus() const {return status;}
        void setStatus(CardStatus newStatus) {status = newStatus;}

        Carte(JewelryCard* jewelrycard, QWidget *parent = nullptr);
        ~Carte();

        void afficher();
        JewelryCard* getJewelryCard() {return jewelryCard;}

        bool isCardBuyable(Carte* card) const;
        bool isCardReservable(Carte* card) const;
        void mousePressEvent(QMouseEvent* event);



    private:
        QString getImagePath() const;
        JewelryCard* jewelryCard;
        CardStatus status;

    signals:
        void clicked(Carte* carte);
    };




    class QTPyramid : public QWidget {
        Q_OBJECT

    public:
        QTPyramid(QWidget *parent = nullptr);

        void afficher();

        Pyramid_Cards* getPyramidCard() {return pyramidcard;}
        QGridLayout* getgrid() {return grille;}

        void ajouterCarte(int rowlevel);

        int retirerCarte(Carte* carte);



        void carteClicked(Carte* carte);

        void updateAllCardStatus(Carte::CardStatus newStatus);

    private:
        void ajouterCarte(int row, int col);

        Pyramid_Cards* pyramidcard;
        QGridLayout *grille;

    signals:
        void acheterCarteClicked(Carte* carte);
        void reserverCarteClicked(Carte* carte);
    };


    class QTPioche : public QLabel {
        Q_OBJECT

    public:

        enum PiocheStatus {
            reservable,
            notClickable

        };

        PiocheStatus getStatus() const {return status;}
        void setStatus(PiocheStatus newStatus) {status = newStatus;}


        QTPioche(Deck_level_one* Deck1 = nullptr, Deck_level_two* Deck2 = nullptr, Deck_level_three* Deck3 = nullptr, QWidget *parent = nullptr);
        ~QTPioche();

        void afficher();
        Deck_level_one* getDeck1() {return Deck1;}
        Deck_level_two* getDeck2() {return Deck2;}
        Deck_level_three* getDeck3() {return Deck3;}


        bool isCardReservable(QTPioche* pioche) const;
        void mousePressEvent(QMouseEvent* event);



    private:
        QString getImagePath() const;
        Deck_level_one* Deck1;
        Deck_level_two* Deck2;
        Deck_level_three* Deck3;
        PiocheStatus status;

    signals:
        void clicked(QTPioche *pioche);


    };


    class QTRangeePioches : public QWidget {

        Q_OBJECT

    public:
        QTRangeePioches(QWidget *parent = nullptr);

        void afficher();

        //void ajouterPioche(int rowlevel); Pas besoin d'ajouter de pioche à priori

        //int retirerPioche(Carte* carte); Pas besoin pour le moment

        QGridLayout* getGrid() const {return grille;}

        void piocheClicked(QTPioche* pioche);
        void updateAllPiocheStatus(QTPioche::PiocheStatus newStatus);

    private:
        void ajouterPioche(int row, int col);

        QGridLayout *grille;

    signals:
        void reserverCarteClicked(QTPioche* pioche);

    };

    class QTCardRoyal : public QLabel {
    Q_OBJECT

    public:

        enum CardRoyalStatus {
            buyable,
            notClickable
        };

        CardRoyalStatus getStatus() const {return status;}
        void setStatus(CardRoyalStatus newStatus) {status = newStatus;}

        QTCardRoyal(RoyalCard* rc, QLabel* parent = nullptr);
        ~QTCardRoyal();
        void afficher();
        RoyalCard* getRoyalCard() {return royalcard;}

        bool isCardBuyable(QTCardRoyal* card) const;
        void mousePressEvent(QMouseEvent* event);

    private:
        QString getImagePath() const;
        RoyalCard* royalcard;
        CardRoyalStatus status;

    signals:
        void clicked(QTCardRoyal* carte);

    };

    class QTBoardRoyal : public QWidget {
        Q_OBJECT

    public:
        QTBoardRoyal(QWidget *parent = nullptr);

        //void ajouterRangee(const QStringList& valeurs, const QStringList& couleurs);

        //void ajouterPioche();
        void afficher();

        Deck_Royal* getDeckRoyal() {return deckroyal;}

        //void ajouterCarte(int rowlevel);

        void retirerCarte(QTCardRoyal* carte);

        QGridLayout* getGrid() {return grille;}

        void carteClicked(QTCardRoyal* carte);

        void updateAllCardStatus(QTCardRoyal::CardRoyalStatus newStatus);

    private:
        void ajouterCarte(int row, int col);

        Deck_Royal* deckroyal;
        QGridLayout *grille;

    signals:
        void acheterCarteClicked(QTCardRoyal* carte);



    };



#endif // QTCARDS_H
