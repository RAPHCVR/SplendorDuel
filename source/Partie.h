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
#include "joueur.h"

class GameException {
    //Classe de gestion des exception avec la partie
private:
    std::string message; //Message d'exception
public:
    explicit GameException(const std::string &msg) : message(msg) {} //Constructeur d'exception

    ~GameException() = default;

    std::string getMessage() const { return message; } //Récupération du message d'exception
};


class GameTable {
    friend class Game;
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

//On implémente la classe partie, qui va contenir les éléments essentiels au déroulement du jeu: GameTable, les Joueurs, et le nombre de tour en cours
class Game {
private:
    unsigned int round;
    Player *players[2];
    GameTable *gametable;
public:
    Game();
    ~Game() {
        delete gametable;
        delete players[0];
        delete players[1];
    }

    unsigned int getRound() const {
        return round;
    }
    GameTable& getGameTable() const {
        return *gametable;
    }
    Player* getPlayer(size_t i) const {
        return players[i];
    }
    Player* getPlayer1() const {
        return players[0];
    }
    Player* getPlayer2() const {
        return players[1];
    }


    //Setter
    void setPlayer1(Player & j);
    void setPlayer2(Player & j);
    void setRound(unsigned int n) {
        round = n;
    }

    void roundIncrement() {
        round++;
    }
};

//Classe abstraite Builder pour construire nouvelle partie
class GameBuilder {
public: virtual ~GameBuilder() {};
    virtual void setPlayers(string pseudo1, type t1, string pseudo2, type t2) const = 0;
    virtual void setPlayers() const = 0;
    virtual void setPlayersCards() const = 0;
    virtual void setPlayersToken() const = 0;
    virtual void updateGameTable() const = 0;
    virtual void setGameInfos() const = 0;
    friend class Game;
};
//A faire: Builder, Memento(Sauvegarde)...
#endif //PARTIE_H
