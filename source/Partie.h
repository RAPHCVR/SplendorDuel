//
// Created by utcpret on 11/11/2023.
//

#ifndef PARTIE_H
#define PARTIE_H
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include "Jeton.h"
#include "Cards.h"

class GameTable {
    friend class Partie;
    private:
        // Partie Cartes
        Deck_Royal *deckroyal;
        Deck_level_one *decklv1;
        Deck_level_two *decklv2 ;
        Deck_level_three *decklv3 ;
        Pyramid_Cards *pyramid ;
        // Partie Jetons
        Bag *bag;
        Board *board;

    public:
        GameTable() : bag(&Bag::getInstance()), board(&Board::getInstance()),  deckroyal(new Deck_Royal()), decklv1(new Deck_level_one()), decklv2(new Deck_level_two()), decklv3(new Deck_level_three()), pyramid(new Pyramid_Cards(*decklv1,*decklv2,*decklv3)) {}

        ~GameTable() { delete pyramid; delete decklv1; delete decklv2; delete decklv3; delete bag; delete board; };

        Bag& getBag() const { return *bag; }
        Board& getBoard() const { return *board; }
        Pyramid_Cards& getPyramid() const {return *pyramid; }

        GameTable(const GameTable&) = delete;
        GameTable& operator=(const GameTable&) = delete;


};


//A faire: Partie, PartieException, Builder, Sauvegarde...
#endif //PARTIE_H
