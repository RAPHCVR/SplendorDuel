#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "sqlite/sqlite3.h"
#include "Jeton.h"

//Enum Abilities pour les capacités
enum class Abilities {repeat_turn, cameleon, take_bonus_token, take_privilege, steal_token, None};
std::string toString(Abilities a);
std::ostream& operator<<(std::ostream& f, Abilities a);

namespace Utility {
    Abilities stringToAbility(const char *str);
}

struct Bonus {
    int bonus_number;
    TokenColor bonus_color;
};
std::ostream& operator<<(std::ostream&f, Bonus &b);


bool operator==(const Bonus& b1, const Bonus& b2);

class SummaryCard{
    
private :
    unsigned int prestigePoints;
    unsigned int bonusNumber;
    //int crownNumber;

public :
    explicit SummaryCard(unsigned int p = 0, unsigned int b = 0) : prestigePoints(p), bonusNumber(b) {};
    unsigned int getPrestigePoints(){ return prestigePoints;}
    unsigned int getBonusNumber(){ return bonusNumber;}
    void addBonusNumber(unsigned int b);
    void addprestigePoints(unsigned int p);
    //int getCrownNumber(){ return crownNumber;}
};


//Classe exception
class JewelryCardError {
    //Classe de gestion des exception avec les carte de gemmes
private:
    std::string message; //Message d'exception
public:
    JewelryCardError(const std::string &msg) : message(msg) {} //Constructeur d'exception
    ~JewelryCardError() = default;

    std::string getMessage() const { return message; } //Récupération du message d'exception
};



class JewelryCard {
public:
    JewelryCard(unsigned int l, std::unordered_map<TokenColor, int> c, unsigned int pp, unsigned int cr, Abilities a1, Abilities a2, Bonus b, int i) :
    level(l), cost(c), prestige_points(pp), crowns(cr), ability1(a1), ability2(a2), bonus(b), id(i) {}
    int getLevel() {return level;}
    std::unordered_map<TokenColor, int> getCost() {return cost;}
    int getPrestige() {return prestige_points;}
    int getCrowns() {return crowns;}
    Abilities getAbility1() {return ability1;}
    Abilities getAbility2() {return ability2;}
    Bonus getBonus() {return bonus;}
	int getId() {return id;}
    bool operator==(const JewelryCard& other) const {
        return level == other.level && cost == other.cost && prestige_points == other.prestige_points && crowns == other.crowns && ability1 == other.ability1 && ability2 == other.ability2 && bonus == other.bonus;
    }
private:
    unsigned int level;
    std::unordered_map<TokenColor, int> cost; //dans l'ordre BLEU, BLANC, VERT, NOIR, ROUGE, PERLE (modifiable)
    unsigned int prestige_points;
    unsigned int crowns;
    Abilities ability1;
    Abilities ability2;
    Bonus bonus;
	int id;
};

std::ostream& operator<<(std::ostream& f, JewelryCard& c);


class RoyalCardError {
    //Classe de gestion des exception avec les cartes royales
private:
    std::string message; //Message d'exception
public:
    RoyalCardError(const std::string &msg) : message(msg) {} //Constructeur d'exception
    ~RoyalCardError() = default;

    std::string getMessage() const { return message; } //Récupération du message d'exception
};

class RoyalCard {
public:
    RoyalCard(unsigned int pp, Abilities a, unsigned int i) :
    prestige_points(pp), ability(a), id(i) {}
    unsigned int getPrestige() {return prestige_points;}
    Abilities getAbility() {return ability;}
    unsigned int getId() {return id;}									 
private:
    unsigned int prestige_points;
    Abilities ability;
    unsigned int id;					
};

std::ostream& operator<<(std::ostream& f, RoyalCard& c);

class Deck_Royal {
public:

    Deck_Royal(const std::string& databaseSavePath); // Rémy save

    // Transformation en singleton
    static Deck_Royal* getInstance() {
        if (instance == nullptr) {
            instance = new Deck_Royal();  // Crée l'instance si elle n'existe pas encore
        }
        return instance;
    }

    static void resetInstance() {
        delete instance;
        instance = nullptr;
    }
    //Deck_Royal();

    std::vector<RoyalCard*> getCards() { return cards; }

    void addCardToDeck(RoyalCard* card) {
        cards.push_back(card);
    }

    void deleteCard(int pos) {
        cards.erase(cards.begin() + pos);
    }


private:

    Deck_Royal();
    
    static Deck_Royal* instance;

    std::vector<RoyalCard*> cards;
};

class Deck_level_one {
public:

    Deck_level_one(const std::string& databaseSavePath); // Rémy Save

    // Transformation en singleton
    static Deck_level_one* getInstance() {
    if (instance == nullptr) {
        instance = new Deck_level_one();  // Crée l'instance si elle n'existe pas encore
        }
        return instance;
    }

    static void resetInstance() {
        delete instance;
        instance = nullptr;
    }

    //Deck_level_one();
    std::vector<JewelryCard*> getPioche() { return pioche; }

    void addCardToDeck(JewelryCard* card) { //ajouter une carte au deck
        pioche.push_back(card);
    }
    void deleteFirstItem() { pioche.erase(pioche.begin()); }

private:

    Deck_level_one();
    
    static Deck_level_one* instance;

    std::vector<JewelryCard*> pioche;
};

class Deck_level_two {
public:

    Deck_level_two(const std::string& databaseSavePath); // Rémy save

    // Transformation en singleton
    static Deck_level_two* getInstance() {
        if (instance == nullptr) {
            instance = new Deck_level_two();  // Crée l'instance si elle n'existe pas encore
        }
        return instance;
    }

    static void resetInstance() {
        delete instance;
        instance = nullptr;
    }

    //Deck_level_two();
    std::vector<JewelryCard*> getPioche() { return pioche; }

    void addCardToDeck(JewelryCard* card) { //ajouter une carte au deck
        pioche.push_back(card);
    }

    void deleteFirstItem() { pioche.erase(pioche.begin()); }
private:

    Deck_level_two();

    static Deck_level_two* instance;
    
    std::vector<JewelryCard*> pioche;
};

class Deck_level_three {
public:

    Deck_level_three(const std::string& databaseSavePath); // Rémy save

    // Transformation en singleton
    static Deck_level_three* getInstance() {
    if (instance == nullptr) {
        instance = new Deck_level_three();  // Crée l'instance si elle n'existe pas encore
        }
        return instance;
    }

    static void resetInstance() {
        delete instance;
        instance = nullptr;
    }

    //Deck_level_three();
    std::vector<JewelryCard*> getPioche() { return pioche; }

    void addCardToDeck(JewelryCard* card) { //ajouter une carte au deck
        pioche.push_back(card);
    }

    void deleteFirstItem() { pioche.erase(pioche.begin()); }
private:

    Deck_level_three();

    static Deck_level_three* instance;

    std::vector<JewelryCard*> pioche;
};
JewelryCard& takeCard(unsigned int level);

class Pyramid_Cards {
public:

    Pyramid_Cards(const std::string & databaseSavePath); //Rémy Save

    // Transformation en singleton
    static Pyramid_Cards* getInstance() {
        if (instance == nullptr) {
            instance = new Pyramid_Cards();  // Crée l'instance si elle n'existe pas encore
        }
        return instance;
    }

    static void resetInstance() {
        delete instance;
        instance = nullptr;
    }

    //Pyramid_Cards(Deck_level_one Deck_one, Deck_level_two Deck_two, Deck_level_three Deck_three);
    void drawCard(unsigned int level);
    JewelryCard& takeCard(unsigned int level, unsigned int position);
    std::vector<JewelryCard*> getRow1() {return row_level_one;}
    std::vector<JewelryCard*> getRow2() {return row_level_two;}
    std::vector<JewelryCard*> getRow3() {return row_level_three;}


    bool isEmpty(unsigned int level) { return (level == 1) ? row_level_one.empty() : (level == 2) ? row_level_two.empty() : row_level_three.empty(); };
    std::vector<JewelryCard*> getLevelCards(unsigned int i) { return (i == 1) ? row_level_one : (i == 2) ? row_level_two : row_level_three; }

private:

    Pyramid_Cards();

    static Pyramid_Cards *instance;

    std::vector<JewelryCard*> row_level_one;
    std::vector<JewelryCard*> row_level_two;
    std::vector<JewelryCard*> row_level_three;
    static const unsigned int max_level_one = 5;
    static const unsigned int max_level_two = 4;
    static const unsigned int max_level_three = 3;
    /* Pas besoin car vecteur se stack automatiquement
    unsigned int position_level_one = 0;
    unsigned int position_level_two = 0;
    unsigned int position_level_three = 0;
     */


};
std::ostream& operator << (std::ostream & f,Pyramid_Cards& p);
#endif /* CARDS_H */

