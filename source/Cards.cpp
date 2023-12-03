#include <iostream>				 
#include <cstring>						
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <random>
#include <chrono>
#include "sqlite/sqlite3.h"
#include "Cards.h"
#include "Jeton.h"

namespace Utility{
    Abilities stringToAbility(const char *str) {
        if (strcmp(str, "repeat_turn") == 0) {
            return Abilities::repeat_turn;
        } else if (strcmp(str, "cameleon") == 0) {
            return Abilities::cameleon;
        } else if (strcmp(str, "take_bonus_token") == 0) {
            return Abilities::take_bonus_token;
        } else if (strcmp(str, "take_privilege") == 0) {
            return Abilities::take_privilege;
        } else if (strcmp(str, "steal_token") == 0) {
            return Abilities::steal_token;
        } 
        else if (strcmp(str, "None") == 0) {
            return Abilities::None;
        }else {
            // Gérer le cas où la chaîne ne correspond à aucune capacité connue
            return Abilities::repeat_turn;
        }
    } 
   
    TokenColor stringToTokenColor(const char *str) {
        if (std::strcmp(str, "BLEU") == 0) {
            return TokenColor::BLEU;
        } else if (std::strcmp(str, "BLANC") == 0) {
            return TokenColor::BLANC;
        } else if (std::strcmp(str, "VERT") == 0) {
            return TokenColor::VERT;
        } else if (std::strcmp(str, "NOIR") == 0) {
            return TokenColor::NOIR;
        } else if (std::strcmp(str, "ROUGE") == 0) {
            return TokenColor::ROUGE;
        } else if (std::strcmp(str, "PERLE") == 0) {
            return TokenColor::PERLE;
        } else if (std::strcmp(str, "OR") == 0) {
            return TokenColor::OR;
        } else if (std::strcmp(str, "RIEN") == 0){
            return TokenColor::None;
        }
        else {
            // Gérer le cas où la chaîne ne correspond à aucune couleur connue
            return TokenColor::BLEU;
        }
    }

    std::string tokenColorToString(TokenColor color) {
        switch (color) {
            case TokenColor::BLEU:
                return "BLEU";
            case TokenColor::BLANC:
                return "BLANC";
            case TokenColor::VERT:
                return "VERT";
            case TokenColor::NOIR:
                return "NOIR";
            case TokenColor::ROUGE:
                return "ROUGE";
            case TokenColor::PERLE:
                return "PERLE";
            // Ajoutez d'autres cas au besoin
            default:
                return "UNKNOWN";
        }
    }
}

std::string toString(Abilities a){
    switch(a){
        case Abilities::repeat_turn: return "Rejouer";
        case Abilities::cameleon: return "Caméleon";
        case Abilities::take_bonus_token: return "Prendre un jeton";
        case Abilities::take_privilege: return "Prendre un privilège";
        case Abilities::steal_token: return "Voler un jeton";
        case Abilities::None: return "Rien";
        default: throw std::invalid_argument("Capacité inconnue");
    }
}
    
std::ostream& operator<<(std::ostream& f, Abilities a){
    return f << toString(a);
}
	
Deck_level_one::Deck_level_one() : pioche() {
    
            
    sqlite3 *db; //On créer une variable sqlite du nom de db
    int rc = sqlite3_open("C:/Users/world/CLionProjects/projet-lo21-splendor-duel/source/Data/cards.db", &db); //rc = return code, on ouvre la database

    if (rc) {
        std::cerr << "Erreur lors de l'ouverture de la base de données: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }
    
    const char *query = "SELECT * FROM JewelryCards WHERE level = 1"; //requete pour chercher carte lvl 1
    // Préparer la requête
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Erreur lors de la préparation de la requête: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        
        //Niveau
        int level = sqlite3_column_int(stmt, 9); //Level dans la 9ème colonne
							  
  
        //Coûts
        unsigned int cost_w = sqlite3_column_int(stmt, 2);
        unsigned int cost_v = sqlite3_column_int(stmt, 3);
        unsigned int cost_n = sqlite3_column_int(stmt, 4);
        unsigned int cost_p = sqlite3_column_int(stmt, 5);
        unsigned int cost_r = sqlite3_column_int(stmt, 6);
        unsigned int cost_b = sqlite3_column_int(stmt, 7);
        std::unordered_map<TokenColor, int> cost = { //dans l'ordre BLEU, BLANC, VERT, NOIR, ROUGE, PERLE (modifiable)
            {TokenColor::BLEU, cost_b},
            {TokenColor::BLANC, cost_w},
            {TokenColor::VERT, cost_v},
            {TokenColor::NOIR, cost_n},
            {TokenColor::ROUGE, cost_r},
            {TokenColor::PERLE, cost_p}        
        };

        //Prestige
        unsigned int prestige_points = sqlite3_column_int(stmt, 0);
      
        //Couronnes
        unsigned int crowns = sqlite3_column_int(stmt, 1);
      
        //Capacité
        const char *abi1 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
        Abilities ability1 = Utility::stringToAbility(abi1);
        const char *abi2 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
        Abilities ability2 = Utility::stringToAbility(abi2);
     
        //Bonus
        int bonus_nb = sqlite3_column_int(stmt, 12);
        const char *color = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));  
        Bonus bonus;      
        bonus.bonus_color = Utility::stringToTokenColor(color);   
        bonus.bonus_number = bonus_nb;
    

        //Création et ajout de l'instance au deck
        JewelryCard *newCard = new JewelryCard(level, cost, prestige_points, crowns, ability1, ability2, bonus); 
        Deck_level_one::addCardToDeck(newCard);
						
    }    
    

    //Mélange des cartes de la pioche.
    //Génération d'un nombre aléatoire
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(static_cast<unsigned>(seed));
			  
    std::shuffle(pioche.begin(), pioche.end(), rng);
    
}

Deck_level_two::Deck_level_two() : pioche() {
    
            
    sqlite3 *db; //On créer une variable sqlite du nom de db
    int rc = sqlite3_open("C:/Users/world/CLionProjects/projet-lo21-splendor-duel/source/Data/cards.db", &db); //rc = return code, on ouvre la database
    
    if (rc) {
        std::cerr << "Erreur lors de l'ouverture de la base de données: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }
    
    const char *query = "SELECT * FROM JewelryCards WHERE level = 2"; //requete pour chercher carte lvl 2
    // Préparer la requête
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Erreur lors de la préparation de la requête: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        
        //Niveau
        int level = sqlite3_column_int(stmt, 9); //Level dans la 9ème colonne
  
        //Coûts
        unsigned int cost_w = sqlite3_column_int(stmt, 2);
        unsigned int cost_v = sqlite3_column_int(stmt, 3);
        unsigned int cost_n = sqlite3_column_int(stmt, 4);
        unsigned int cost_p = sqlite3_column_int(stmt, 5);
        unsigned int cost_r = sqlite3_column_int(stmt, 6);
        unsigned int cost_b = sqlite3_column_int(stmt, 7);
        std::unordered_map<TokenColor, int> cost = { //dans l'ordre BLEU, BLANC, VERT, NOIR, ROUGE, PERLE (modifiable)
            {TokenColor::BLEU, cost_b},
            {TokenColor::BLANC, cost_w},
            {TokenColor::VERT, cost_v},
            {TokenColor::NOIR, cost_n},
            {TokenColor::ROUGE, cost_r},
            {TokenColor::PERLE, cost_p}        
        };

        //Prestige
        unsigned int prestige_points = sqlite3_column_int(stmt, 0);
      
        //Couronnes
        unsigned int crowns = sqlite3_column_int(stmt, 1);
      
        //Capacité
        const char *abi1 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
        Abilities ability1 = Utility::stringToAbility(abi1);
        const char *abi2 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
        Abilities ability2 = Utility::stringToAbility(abi2);
     
        //Bonus
        int bonus_nb = sqlite3_column_int(stmt, 12);
        const char *color = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));  
        Bonus bonus;      
        bonus.bonus_color = Utility::stringToTokenColor(color);   
        bonus.bonus_number = bonus_nb;
    

        //Création et ajout de l'instance au deck
        JewelryCard *newCard = new JewelryCard(level, cost, prestige_points, crowns, ability1, ability2, bonus); 
        Deck_level_two::addCardToDeck(newCard);
					  
    }    
    

    //Mélange des cartes de la pioche.
    //Génération d'un nombre aléatoire
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(static_cast<unsigned>(seed));
			  
    std::shuffle(pioche.begin(), pioche.end(), rng);
    
}
Deck_level_three::Deck_level_three() : pioche() {
    
            
    sqlite3 *db; //On créer une variable sqlite du nom de db
    int rc = sqlite3_open("C:/Users/world/CLionProjects/projet-lo21-splendor-duel/source/Data/cards.db", &db); //rc = return code, on ouvre la database
    
    if (rc) {
        std::cerr << "Erreur lors de l'ouverture de la base de données: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }
    
    const char *query = "SELECT * FROM JewelryCards WHERE level = 3"; //requete pour chercher carte lvl 3
    // Préparer la requête
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Erreur lors de la préparation de la requête: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        
        //Niveau
        int level = sqlite3_column_int(stmt, 9); //Level dans la 9ème colonne
  
        //Coûts
        unsigned int cost_w = sqlite3_column_int(stmt, 2);
        unsigned int cost_v = sqlite3_column_int(stmt, 3);
        unsigned int cost_n = sqlite3_column_int(stmt, 4);
        unsigned int cost_p = sqlite3_column_int(stmt, 5);
        unsigned int cost_r = sqlite3_column_int(stmt, 6);
        unsigned int cost_b = sqlite3_column_int(stmt, 7);
        std::unordered_map<TokenColor, int> cost = { //dans l'ordre BLEU, BLANC, VERT, NOIR, ROUGE, PERLE (modifiable)
            {TokenColor::BLEU, cost_b},
            {TokenColor::BLANC, cost_w},
            {TokenColor::VERT, cost_v},
            {TokenColor::NOIR, cost_n},
            {TokenColor::ROUGE, cost_r},
            {TokenColor::PERLE, cost_p}        
        };

        //Prestige
        unsigned int prestige_points = sqlite3_column_int(stmt, 0);
      
        //Couronnes
        unsigned int crowns = sqlite3_column_int(stmt, 1);
      
        //Capacité
        const char *abi1 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
        Abilities ability1 = Utility::stringToAbility(abi1);
		
					
        const char *abi2 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
        Abilities ability2 = Utility::stringToAbility(abi2);
     
        //Bonus
        int bonus_nb = sqlite3_column_int(stmt, 12);
        const char *color = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));  
        Bonus bonus;      
        bonus.bonus_color = Utility::stringToTokenColor(color);   
        bonus.bonus_number = bonus_nb;
    

        //Création et ajout de l'instance au deck
        JewelryCard *newCard = new JewelryCard(level, cost, prestige_points, crowns, ability1, ability2, bonus); 
        Deck_level_three::addCardToDeck(newCard);
					  
    }    
    

    //Mélange des cartes de la pioche.
    //Génération d'un nombre aléatoire
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(static_cast<unsigned>(seed));
			  
    std::shuffle(pioche.begin(), pioche.end(), rng);
    
}
Deck_Royal::Deck_Royal() : cards() {

    sqlite3 *db; //On créer une variable sqlite du nom de db
    int rc = sqlite3_open("C:/Users/world/CLionProjects/projet-lo21-splendor-duel/source/Data/cards.db", &db); //rc = return code, on ouvre la database
    
    if (rc) {
        std::cerr << "Erreur lors de l'ouverture de la base de données: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }
    
    const char *query = "SELECT * FROM RoyalCards"; //requete pour chercher cartes royales
    // Préparer la requête
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Erreur lors de la préparation de la requête: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        
        //Identifiant
        unsigned int id = sqlite3_column_int(stmt, 0);

        // Capacité
        const char *abi = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        Abilities ability = Utility::stringToAbility(abi);

        //Prestige
        unsigned int prestige_points = sqlite3_column_int(stmt, 2);

        //Création et ajout de l'instance au deck
        RoyalCard *newCard = new RoyalCard(prestige_points, ability, id); 
        Deck_Royal::addCardToDeck(newCard);
    }

}
//PENSER AU CAS OU SI MANQUE DE CARTE DANS LA PIOCHE (SEMBLE IMPOSSIBLE)
JewelryCard& Pyramid_Cards::takeCard(unsigned int level, unsigned int position){
    switch(level){
        case 1:
            if(position>5){
                throw std::runtime_error("Position trop élevée !");
            }
            else{
                unsigned int position_level_one = position;
                return *row_level_one[position];
            }
        case 2:
            if(position>4){
                throw std::runtime_error("Position trop élevée !");
            }
            else{
                unsigned int position_level_two = position;
                return *row_level_two[position];
            }
        case 3:
            if(position>3){
                throw std::runtime_error("Position trop élevée !");
            }
            else{
                unsigned int position_level_three = position;
                return *row_level_three[position];
            }
        default:
            throw std::runtime_error("Niveau de carte non existant");
            

    }
}



//Idee de code, à modif impérativement
//VOIR SI CELA FONCTIONNE AVEC LES POINTEURS
//PENSER A TOUJOURS UTILISER DRAW UNE FOIS UTILISE TAKE
Pyramid_Cards::Pyramid_Cards(Deck_level_one Deck_one, Deck_level_two Deck_two, Deck_level_three Deck_three)  : row_level_one(), row_level_two(), row_level_three() {

    //Ligne niveau 1
    for(int i=0; i<5; i++)
    {
        row_level_one.push_back(move(Deck_one.getPioche()[0]));
        Deck_one.deleteFirstItem();
    }

    //Ligne cartes niveau 2
    for(int i=0; i<4; i++)
    {
        row_level_two.push_back(move(Deck_two.getPioche()[0]));
        Deck_two.deleteFirstItem();        
    }

    // Ligne cartes niveau 3
    for (int i = 0; i<3; i++)
    {
        row_level_three.push_back(move(Deck_three.getPioche()[0]));
        Deck_three.deleteFirstItem();        
    }
    
}

void Pyramid_Cards::drawCard(unsigned int level, Deck_level_one Deck_one, Deck_level_two Deck_two, Deck_level_three Deck_three) {
						   
    switch (level) {
        case 1:
            if (!Deck_one.getPioche().empty())
            {
                row_level_one.push_back(move(Deck_one.getPioche()[0]));
                Deck_one.deleteFirstItem();
            }
            else
            {
                throw std::runtime_error("Le deck niveau 1 est vide");
            }
            break;
        case 2:
            if (!Deck_two.getPioche().empty()) {
														 
                row_level_two.push_back(move(Deck_two.getPioche()[0]));
                Deck_two.deleteFirstItem();
            } else {
                throw std::runtime_error("Le deck niveau 2 est vide");
            }
            break;
        case 3:
            if (!Deck_three.getPioche().empty()) {
														   
                row_level_three.push_back(move(Deck_three.getPioche()[0]));
                Deck_three.deleteFirstItem();
            } else {
                throw std::runtime_error("Le deck niveau 3 est vide");
            }
            break;
        default:
            throw std::runtime_error("Niveau non existant");
   
    }
}  

void SummaryCard::addBonusNumber(unsigned int b){
   this->bonusNumber += b;
}

void SummaryCard::addprestigePoints(unsigned int p){
    this->prestigePoints += p;
}
