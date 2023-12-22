#ifndef LO21PROJECT_PARTIE_H
#define LO21PROJECT_PARTIE_H

#include <iostream>
#include <string>
#include "joueur.h"
#include "sqlite/sqlite3.h"

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
    GameTable() : bag(Bag::getInstance()), board(Board::getInstance()),  deckroyal(Deck_Royal::getInstance()), decklv1(Deck_level_one::getInstance()), decklv2(Deck_level_two::getInstance()), decklv3(Deck_level_three::getInstance()), pyramid(Pyramid_Cards::getInstance()) {}

    ~GameTable() { delete pyramid; delete decklv1; delete decklv2; delete decklv3; delete bag; delete board; };

    Bag& getBag() const { return *bag; }
    Board& getBoard() const { return *board; }
    Pyramid_Cards& getPyramid() const {return *pyramid; }
    Deck_Royal& getDeckRoyal() const {return *deckroyal; }
    Deck_level_one& getDeckLevelOne() const {return *decklv1; }
    Deck_level_two& getDeckLevelTwo() const {return *decklv2; }
    Deck_level_three& getDeckLevelThree() const {return *decklv3; }

    GameTable(const GameTable&) = delete;
    GameTable& operator=(const GameTable&) = delete;
    friend class Game;

};

//On implémente la classe partie, qui va contenir les éléments essentiels au déroulement du jeu: GameTable, les Joueurs, et le nombre de tour en cours
class Game {
private:
    unsigned int round;
    Player *players[2];
    GameTable *gametable;
public:
    Game() : gametable(new GameTable()) {
        players[0] = nullptr;
        players[1] = nullptr;
    }
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


    //Setter
    void setPlayer1(Player *j);
    void setPlayer2(Player *j);
    void setRound(unsigned int n) {
        round = n;
    }

    void roundIncrement() {
        round++;
    }
};


class Builder {
    //Design pattern builder
public:
    virtual ~Builder(){}
    virtual void setPlayers(std::string& player1Name,Type typep1, std::string& player2Name, Type typep2) const =0;
    virtual void setPlayersCards() const =0;
    virtual void setPlayersTokens() const =0;
    virtual void roundInit() const =0;
    //ajouter des méthodes ici pour les cartes et les jetons, etc
};

class GameBuilder : public Builder{
    private: Game* game;

    /**
     * A fresh builder instance should contain a blank product object, which is
     * used in further assembly.
     */
    public:

    GameBuilder(){
        this->Reset();
    }

    ~GameBuilder(){
        delete game;
    }

    void Reset(){
        this->game= new Game();
    }
    /**
     * All production steps work with the same product instance.
     */

    void setPlayers(std::string& player1Name,const Type typep1, std::string& player2Name, const Type typep2)const override{
        this->game->setPlayer1(new Player(player1Name,typep1));
        this->game->setPlayer2(new Player(player2Name,typep2));
    }

    void setPlayersCards()const override{};

    void setPlayersTokens()const override{}

    void roundInit() const override {
        game -> setRound(0);
    };

    /**
     * Concrete Builders are supposed to provide their own methods for
     * retrieving results. That's because various types of builders may create
     * entirely different products that don't follow the same interface.
     * Therefore, such methods cannot be declared in the base Builder interface
     * (at least in a statically typed programming language). Note that PHP is a
     * dynamically typed language and this method CAN be in the base interface.
     * However, we won't declare it there for the sake of clarity.
     *
     * Usually, after returning the end result to the client, a builder instance
     * is expected to be ready to start producing another product. That's why
     * it's a usual practice to call the reset method at the end of the
     * `getProduct` method body. However, this behavior is not mandatory, and
     * you can make your builders wait for an explicit reset call from the
     * client code before disposing of the previous result.
     */

    /**
     * Please be careful here with the memory ownership. Once you call
     * GetProduct the user of this function is responsable to release this
     * memory. Here could be a better option to use smart pointers to avoid
     * memory leaks
     */

    Game* GetProduct() {
        Game* result= this->game;
        this->Reset();
        return result;
    }
};

class GameSaveBuilder: public Builder {
private:
    Game * game;
public:
    GameSaveBuilder() {
        this -> Reset();
    }
    ~GameSaveBuilder() {
        delete game;
    }
    void Reset() {
        this -> game = new Game();
    }

    //ajouter des méthodes ici pour les cartes et les jetons, etc


    Game * GetProduct() {
        Game * result = this -> game;
        return result;
    }

};

class Director{
    /**
     * @var Builder
     */
private:
    Builder* builder;
    /**
     * The Director works with any builder instance that the client code passes
     * to it. This way, the client code may alter the final type of the newly
     * assembled product.
     */

public:

    void set_builder(Builder* builder){
        this->builder=builder;
    }

    /**
     * The Director can construct several product variations using the same
     * building steps.
     */

    void BuildGame(std::string& player1Name,const Type typep1, std::string& player2Name, const Type typep2){
        this->builder->setPlayers(player1Name, typep1, player2Name, typep2);
        this->builder->roundInit();
    }
};



#endif //LO21PROJECT_JETON_H

