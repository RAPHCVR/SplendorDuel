#ifndef QTCARDS_H
#define QTCARDS_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QColor>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <sqlite/sqlite3.h>
#include <QString>
#include <QPixmap>
#include <QGridLayout>
#include <QLabel>
// Inclure cstddef pour utiliser std::byte de manière explicite
#include <cstddef>
#include "Jeton.h"
#include "Cards.h"

using namespace std;

namespace Utility {

    QString toHexaDecimal(TokenColor c);

}

    class Carte : public QLabel {
        Q_OBJECT

    public:

        Carte(JewelryCard* jewelrycard, QWidget *parent = nullptr);
        ~Carte();

        void afficher();
        JewelryCard* getJewelryCard() {return jewelryCard;}

        void mousePressEvent(QMouseEvent* event);



    private:
        QString getImagePath() const;
        JewelryCard* jewelryCard;

    signals:
        void clicked(Carte* carte);
    };




    class QTPyramid : public QWidget {
        Q_OBJECT

    public:
        QTPyramid(QWidget *parent = nullptr);

        void ajouterRangee(const QStringList& valeurs, const QStringList& couleurs);

        void ajouterPioche();
        void afficher();

        Pyramid_Cards* getPyramidCard() {return pyramidcard;}

        void ajouterCarte(int rowlevel);

        int retirerCarte(Carte* carte);



        void carteClicked(Carte* carte);

    private:
        void ajouterCarte(int row, int col);

        Pyramid_Cards* pyramidcard;
        QGridLayout *grille;
    };


    class QTPioche : public QLabel {
        Q_OBJECT

    public:

        QTPioche(Deck_level_one* Deck1 = nullptr, Deck_level_two* Deck2 = nullptr, Deck_level_three* Deck3 = nullptr, QWidget *parent = nullptr);
        ~QTPioche();

        void afficher();
        Deck_level_one* getDeck1() {return Deck1;}
        Deck_level_two* getDeck2() {return Deck2;}
        Deck_level_three* getDeck3() {return Deck3;}

        void mousePressEvent(QMouseEvent* event);



    private:
        QString getImagePath() const;
        Deck_level_one* Deck1;
        Deck_level_two* Deck2;
        Deck_level_three* Deck3;

    signals:
        void clicked(QTPioche* pioche);
    };


    class QTRangeePioches : public QWidget {

        Q_OBJECT

    public:
        QTRangeePioches(QWidget *parent = nullptr);

        void afficher();

        //void ajouterPioche(int rowlevel); Pas besoin d'ajouter de pioche à priori

        //int retirerPioche(Carte* carte); Pas besoin pour le moment

        bool piocheClicked(QTPioche* pioche);

    private:
        void ajouterPioche(int row, int col);

        QGridLayout *grille;

    };

    class QTCardRoyal : public QLabel {
    Q_OBJECT

    public:
        QTCardRoyal(RoyalCard* rc, QLabel* parent = nullptr);
        ~QTCardRoyal();
        void afficher();
        RoyalCard* getRoyalCard() {return royalcard;}

        void mousePressEvent(QMouseEvent* event);

    private:
        QString getImagePath() const;
        RoyalCard* royalcard;

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

        int retirerCarte(Carte* carte);



        void carteClicked(QTCardRoyal* carte);

    private:
        void ajouterCarte(int row, int col);

        Deck_Royal* deckroyal;
        QGridLayout *grille;


    };



#endif // QTCARDS_H
