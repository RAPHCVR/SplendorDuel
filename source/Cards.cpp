#include <iostream>				 
#include <cstring>						
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <random>
#include <chrono>
#include <filesystem>					 
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
}

std::string toString(Abilities a){
    switch(a){
        case Abilities::repeat_turn: return "repeat_turn";
        case Abilities::cameleon: return "cameleon";
        case Abilities::take_bonus_token: return "take_bonus_token";
        case Abilities::take_privilege: return "take_privilege";
        case Abilities::steal_token: return "steal_token";
        case Abilities::None: return "None";
        default: throw std::invalid_argument("Capacité inconnue");
    }
}
    
std::ostream& operator<<(std::ostream& f, Abilities a){
    return f << toString(a);
}
	
Deck_level_one* Deck_level_one::instance = nullptr;
Deck_level_one::Deck_level_one() : pioche() {

    // Obtient le chemin du fichier source actuel (__FILE__)
    std::filesystem::path sourceFilePath = std::filesystem::path(__FILE__);
    // Obtient le répertoire du fichier source
    std::filesystem::path sourceDirectory = sourceFilePath.parent_path();
    std::string pathtodatabase = sourceDirectory.string() + "/Data/cards.db";
    
    //cout << pathtodatabase << endl;
    sqlite3 *db; //On créer une variable sqlite du nom de db
										 
    int rc = sqlite3_open(pathtodatabase.c_str(), &db); //rc = return code, on ouvre la database

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
        bonus.bonus_color = toTokenColor(color);
        bonus.bonus_number = bonus_nb;

        //Id
        int id = sqlite3_column_int(stmt, 13);

        //Création et ajout de l'instance au deck
        JewelryCard *newCard = new JewelryCard(level, cost, prestige_points, crowns, ability1, ability2, bonus, id); 
        Deck_level_one::addCardToDeck(newCard);
						
    }    
    

    //Mélange des cartes de la pioche.
    //Génération d'un nombre aléatoire
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(static_cast<unsigned>(seed));
			  
    std::shuffle(pioche.begin(), pioche.end(), rng);
    
}

Deck_level_two* Deck_level_two::instance = nullptr;												   
Deck_level_two::Deck_level_two() : pioche() {
    
    // Obtient le chemin du fichier source actuel (__FILE__)
    std::filesystem::path sourceFilePath = std::filesystem::path(__FILE__);
    // Obtient le répertoire du fichier source
    std::filesystem::path sourceDirectory = sourceFilePath.parent_path();
    std::string pathtodatabase = sourceDirectory.string() + "/Data/cards.db";

    std::cout << pathtodatabase << std::endl;
    sqlite3 *db; //On créer une variable sqlite du nom de db
										 
    int rc = sqlite3_open(pathtodatabase.c_str(), &db); //rc = return code, on ouvre la database
   
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
        bonus.bonus_color = toTokenColor(color);
        bonus.bonus_number = bonus_nb;

        //Id
        int id = sqlite3_column_int(stmt, 13);

        //Création et ajout de l'instance au deck
        JewelryCard *newCard = new JewelryCard(level, cost, prestige_points, crowns, ability1, ability2, bonus, id); 
        Deck_level_two::addCardToDeck(newCard);
					  
    }    
    

    //Mélange des cartes de la pioche.
    //Génération d'un nombre aléatoire
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(static_cast<unsigned>(seed));
			  
    std::shuffle(pioche.begin(), pioche.end(), rng);
    
}
Deck_level_three *Deck_level_three::instance = nullptr;													   
Deck_level_three::Deck_level_three() : pioche() {
    

    // Obtient le chemin du fichier source actuel (__FILE__)
    std::filesystem::path sourceFilePath = std::filesystem::path(__FILE__);
    // Obtient le répertoire du fichier source
    std::filesystem::path sourceDirectory = sourceFilePath.parent_path();
    std::string pathtodatabase = sourceDirectory.string() + "/Data/cards.db";

    std::cout << pathtodatabase << std::endl;
    sqlite3 *db; //On créer une variable sqlite du nom de db
										 
    int rc = sqlite3_open(pathtodatabase.c_str(), &db); //rc = return code, on ouvre la database
     
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
        bonus.bonus_color = toTokenColor(color);
        bonus.bonus_number = bonus_nb;

        //Id
        int id = sqlite3_column_int(stmt, 13);

        //Création et ajout de l'instance au deck
        JewelryCard *newCard = new JewelryCard(level, cost, prestige_points, crowns, ability1, ability2, bonus, id); 
        Deck_level_three::addCardToDeck(newCard);
					  
    }    
    

    //Mélange des cartes de la pioche.
    //Génération d'un nombre aléatoire
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(static_cast<unsigned>(seed));
			  
    std::shuffle(pioche.begin(), pioche.end(), rng);
    
}
Deck_Royal *Deck_Royal::instance = nullptr;										   
Deck_Royal::Deck_Royal() : cards() {

    // Obtient le chemin du fichier source actuel (__FILE__)
    std::filesystem::path sourceFilePath = std::filesystem::path(__FILE__);
    // Obtient le répertoire du fichier source
    std::filesystem::path sourceDirectory = sourceFilePath.parent_path();
    std::string pathtodatabase = sourceDirectory.string() + "/Data/cards.db";

    std::cout << pathtodatabase << std::endl;
    sqlite3 *db; //On créer une variable sqlite du nom de db
										 
    int rc = sqlite3_open(pathtodatabase.c_str(), &db); //rc = return code, on ouvre la database
    
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
                JewelryCard *card = row_level_one[position];
                row_level_one.erase(row_level_one.begin()+position);
                return *card;
            }
        case 2:
            if(position>4){
                throw std::runtime_error("Position trop élevée !");
            }
            else{
                JewelryCard *card = row_level_two[position];
                row_level_two.erase(row_level_two.begin()+position);
                return *card;
            }
        case 3:
            if(position>3){
                throw std::runtime_error("Position trop élevée !");
            }
            else{
                JewelryCard *card = row_level_three[position];
                row_level_three.erase(row_level_three.begin()+position);
                return *card;
            }
        default:
            throw std::runtime_error("Niveau de carte non existant");
            

    }
}

JewelryCard& takeCard(unsigned level) {
    //think of deleting the card in the pioche
    switch (level) {
        case 1:
            if (Deck_level_one::getInstance()->getPioche().empty()) {
                throw std::runtime_error("Le deck niveau 1 est vide");
            }
            else {
                JewelryCard *card = Deck_level_one::getInstance()->getPioche()[0];
                Deck_level_one::getInstance()->deleteFirstItem();
                return *card;
            }
        case 2:
            if (Deck_level_two::getInstance()->getPioche().empty()) {
                throw std::runtime_error("Le deck niveau 2 est vide");
            }
            else {
                JewelryCard *card = Deck_level_two::getInstance()->getPioche()[0];
                Deck_level_two::getInstance()->deleteFirstItem();
                return *card;
            }
        case 3:
            if (Deck_level_three::getInstance()->getPioche().empty()) {
                throw std::runtime_error("Le deck niveau 3 est vide");
            }
            else {
                JewelryCard *card = Deck_level_three::getInstance()->getPioche()[0];
                Deck_level_three::getInstance()->deleteFirstItem();
                return *card;
            }
        default:
            throw std::runtime_error("Niveau de carte non existant");
    }
}



//Idee de code, à modif impérativement
//VOIR SI CELA FONCTIONNE AVEC LES POINTEURS
//PENSER A TOUJOURS UTILISER DRAW UNE FOIS UTILISE TAKE
Pyramid_Cards *Pyramid_Cards::instance = nullptr;
Pyramid_Cards::Pyramid_Cards()  : row_level_one(), row_level_two(), row_level_three() {

    //Ligne niveau 1
    for(int i=0; i<5; i++)
    {
        row_level_one.push_back(std::move(Deck_level_one::getInstance()->getPioche()[0]));
        Deck_level_one::getInstance()->deleteFirstItem();
    }

    //Ligne cartes niveau 2
    for(int i=0; i<4; i++)
    {
        row_level_two.push_back(std::move(Deck_level_two::getInstance()->getPioche()[0]));
        Deck_level_two::getInstance()->deleteFirstItem();
    }

    // Ligne cartes niveau 3
    for (int i = 0; i<3; i++)
    {
        row_level_three.push_back(std::move(Deck_level_three::getInstance()->getPioche()[0]));
        Deck_level_three::getInstance()->deleteFirstItem();
    }
    
}

void Pyramid_Cards::drawCard(unsigned int level) {
		 
    switch (level) {
        case 1:
            if (!Deck_level_one::getInstance()->getPioche().empty())
            {
                row_level_one.push_back(std::move(Deck_level_one::getInstance()->getPioche()[0]));
                Deck_level_one::getInstance()->deleteFirstItem();
            }
            else
            {
                std::cout<<"Le deck niveau 1 est vide"<<std::endl;
            }
            break;
        case 2:
            if (!Deck_level_two::getInstance()->getPioche().empty()) {
			   
                row_level_two.push_back(std::move(Deck_level_two::getInstance()->getPioche()[0]));
                Deck_level_two::getInstance()->deleteFirstItem();
            } else {
                std::cout<<"Le deck niveau 2 est vide"<<std::endl;
            }
            break;
        case 3:
            if (!Deck_level_three::getInstance()->getPioche().empty()) {
				 
                row_level_three.push_back(std::move(Deck_level_three::getInstance()->getPioche()[0]));
                Deck_level_three::getInstance()->deleteFirstItem();
            } else {
                std::cout<<"Le deck niveau 3 est vide"<<std::endl;
            }
            break;
        default:
            throw std::runtime_error("Niveau non existant");
   
    }
}

void SummaryCard::addBonusNumber(unsigned int b){
   this->bonusNumber += b;
}

bool operator==(const Bonus& b1, const Bonus& b2){
    return b1.bonus_color == b2.bonus_color && b1.bonus_number == b2.bonus_number;
}

void SummaryCard::addprestigePoints(unsigned int p){
    this->prestigePoints += p;
}

std::ostream& operator<<(std::ostream&f,const Bonus& b) {
    if (b.bonus_color!=TokenColor::None) {
        if (b.bonus_number == 1) {
            f << b.bonus_number << " " << b.bonus_color;
        }
        else {
            f << b.bonus_number << " " << b.bonus_color<<"S";
        }
    }
    else {
        f << "Pas de bonus";
    }
    return f;
}

std::ostream & operator << (std::ostream & f, JewelryCard& c) {
    f << "------------------------------\n";
    f << "Niveau : " << c.getLevel() << "   " << "Bonus : " << c.getBonus() << "\n";
    f << "------------------------------\n";
    f << "Capacite : " << c.getAbility1() << ", " << c.getAbility2() << "\n";
    f << "------------------------------\n";
    f << "Prix :\n";
    for (auto const& [color, costValue] : c.getCost()) {
        if(costValue!=0) {
            if (costValue==1) {
                f << costValue << " " << color << "\n";
            }
            else {
                f << costValue << " " << color << "S\n";
            }
        }
    }
    f << "------------------------------\n";
    f << "Prestige : " << c.getPrestige() << "   " << "Couronnes : " << c.getCrowns() << "\n";
    f << "------------------------------\n";
    return f;
}

std::ostream & operator << (std::ostream & f,
RoyalCard& c) {
    f << "------------------------------\n";
    f << "ID : " << c.getId() << "Prestige : " << c.getPrestige() << "   " << "Capacité : " << c.getAbility() << "\n";
    f << "------------------------------\n";
    return f;
}

std::ostream& operator << (std::ostream & f,Pyramid_Cards& p) {
    for (auto card : p.getLevelCards(1)) {
        f << *card;
    }
    for (auto card : p.getLevelCards(2)) {
        f << *card;
    }
    for (auto card : p.getLevelCards(3)) {
        f << *card;
    }
    return f;
}

/* ----------------------------------------------------------------------------------------------------------------- */
// Ajout Rémy pour save

Pyramid_Cards::Pyramid_Cards(const std::string & databaseSavePath) : row_level_one(), row_level_two(), row_level_three() {

    //cout << pathtodatabase << endl;
    sqlite3* db; //On créer une variable sqlite du nom de db

    int rc = sqlite3_open(databaseSavePath.c_str(), &db); //rc = return code, on ouvre la database

    if (rc) {
        std::cerr << "Erreur lors de l'ouverture de la base de données: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    //Ligne niveau 1
        const char* querylvl1 = "SELECT * FROM pyramid WHERE position = 1"; //requete pour chercher carte lvl 3
        // Préparer la requête
        sqlite3_stmt* stmtlvl1;
        rc = sqlite3_prepare_v2(db, querylvl1, -1, &stmtlvl1, nullptr);

        if (rc != SQLITE_OK) {
            std::cerr << "Erreur lors de la préparation de la requête: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return;
        }

        while ((rc = sqlite3_step(stmtlvl1)) == SQLITE_ROW) {

            //Niveau
            int level = sqlite3_column_int(stmtlvl1, 10); //Level dans la 11ème colonne


            //Coûts
            unsigned int cost_w = sqlite3_column_int(stmtlvl1, 3);
            unsigned int cost_v = sqlite3_column_int(stmtlvl1, 4);
            unsigned int cost_n = sqlite3_column_int(stmtlvl1, 5);
            unsigned int cost_p = sqlite3_column_int(stmtlvl1, 6);
            unsigned int cost_r = sqlite3_column_int(stmtlvl1, 7);
            unsigned int cost_b = sqlite3_column_int(stmtlvl1, 8);
            std::unordered_map<TokenColor, int> cost = { //dans l'ordre BLEU, BLANC, VERT, NOIR, ROUGE, PERLE (modifiable)
                {TokenColor::BLEU, cost_b},
                {TokenColor::BLANC, cost_w},
                {TokenColor::VERT, cost_v},
                {TokenColor::NOIR, cost_n},
                {TokenColor::ROUGE, cost_r},
                {TokenColor::PERLE, cost_p}
            };

            //Prestige
            unsigned int prestige_points = sqlite3_column_int(stmtlvl1, 1);

            //Couronnes
            unsigned int crowns = sqlite3_column_int(stmtlvl1, 2);

            //Capacité
            const char* abi1 = reinterpret_cast<const char*>(sqlite3_column_text(stmtlvl1, 11));
            Abilities ability1 = Utility::stringToAbility(abi1);
            const char* abi2 = reinterpret_cast<const char*>(sqlite3_column_text(stmtlvl1, 12));
            Abilities ability2 = Utility::stringToAbility(abi2);

            //Bonus
            int bonus_nb = sqlite3_column_int(stmtlvl1, 12);
            const char* color = reinterpret_cast<const char*>(sqlite3_column_text(stmtlvl1, 9));
            Bonus bonus;
            bonus.bonus_color = toTokenColor(color);
            bonus.bonus_number = bonus_nb;

            //Id
            int id = sqlite3_column_int(stmtlvl1, 0);

            //Création et ajout de l'instance au deck
            JewelryCard* newCard = new JewelryCard(level, cost, prestige_points, crowns, ability1, ability2, bonus, id);
            row_level_one.push_back(std::move(newCard));

    }

    //Ligne cartes niveau 2
        const char* querylvl2 = "SELECT * FROM pyramid WHERE position = 2"; //requete pour chercher carte lvl 3
        // Préparer la requête
        sqlite3_stmt* stmtlvl2;
        rc = sqlite3_prepare_v2(db, querylvl2, -1, &stmtlvl2, nullptr);

        if (rc != SQLITE_OK) {
            std::cerr << "Erreur lors de la préparation de la requête: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return;
        }

        while ((rc = sqlite3_step(stmtlvl2)) == SQLITE_ROW) {

            //Niveau
            int level = sqlite3_column_int(stmtlvl2, 10); //Level dans la 11ème colonne


            //Coûts
            unsigned int cost_w = sqlite3_column_int(stmtlvl2, 3);
            unsigned int cost_v = sqlite3_column_int(stmtlvl2, 4);
            unsigned int cost_n = sqlite3_column_int(stmtlvl2, 5);
            unsigned int cost_p = sqlite3_column_int(stmtlvl2, 6);
            unsigned int cost_r = sqlite3_column_int(stmtlvl2, 7);
            unsigned int cost_b = sqlite3_column_int(stmtlvl2, 8);
            std::unordered_map<TokenColor, int> cost = { //dans l'ordre BLEU, BLANC, VERT, NOIR, ROUGE, PERLE (modifiable)
                {TokenColor::BLEU, cost_b},
                {TokenColor::BLANC, cost_w},
                {TokenColor::VERT, cost_v},
                {TokenColor::NOIR, cost_n},
                {TokenColor::ROUGE, cost_r},
                {TokenColor::PERLE, cost_p}
            };

            //Prestige
            unsigned int prestige_points = sqlite3_column_int(stmtlvl2, 1);

            //Couronnes
            unsigned int crowns = sqlite3_column_int(stmtlvl2, 2);

            //Capacité
            const char* abi1 = reinterpret_cast<const char*>(sqlite3_column_text(stmtlvl2, 11));
            Abilities ability1 = Utility::stringToAbility(abi1);
            const char* abi2 = reinterpret_cast<const char*>(sqlite3_column_text(stmtlvl2, 12));
            Abilities ability2 = Utility::stringToAbility(abi2);

            //Bonus
            int bonus_nb = sqlite3_column_int(stmtlvl2, 12);
            const char* color = reinterpret_cast<const char*>(sqlite3_column_text(stmtlvl2, 9));
            Bonus bonus;
            bonus.bonus_color = toTokenColor(color);
            bonus.bonus_number = bonus_nb;

            //Id
            int id = sqlite3_column_int(stmtlvl2, 0);

            //Création et ajout de l'instance au deck
            JewelryCard* newCard = new JewelryCard(level, cost, prestige_points, crowns, ability1, ability2, bonus, id);
            row_level_two.push_back(std::move(newCard));
        }

    // Ligne cartes niveau 3
        const char* querylvl3 = "SELECT * FROM pyramid WHERE position = 3"; //requete pour chercher carte lvl 3
        // Préparer la requête
        sqlite3_stmt* stmtlvl3;
        rc = sqlite3_prepare_v2(db, querylvl3, -1, &stmtlvl3, nullptr);

        if (rc != SQLITE_OK) {
            std::cerr << "Erreur lors de la préparation de la requête: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return;
        }

        while ((rc = sqlite3_step(stmtlvl3)) == SQLITE_ROW) {

            //Niveau
            int level = sqlite3_column_int(stmtlvl3, 10); //Level dans la 11ème colonne


            //Coûts
            unsigned int cost_w = sqlite3_column_int(stmtlvl3, 3);
            unsigned int cost_v = sqlite3_column_int(stmtlvl3, 4);
            unsigned int cost_n = sqlite3_column_int(stmtlvl3, 5);
            unsigned int cost_p = sqlite3_column_int(stmtlvl3, 6);
            unsigned int cost_r = sqlite3_column_int(stmtlvl3, 7);
            unsigned int cost_b = sqlite3_column_int(stmtlvl3, 8);
            std::unordered_map<TokenColor, int> cost = { //dans l'ordre BLEU, BLANC, VERT, NOIR, ROUGE, PERLE (modifiable)
                {TokenColor::BLEU, cost_b},
                {TokenColor::BLANC, cost_w},
                {TokenColor::VERT, cost_v},
                {TokenColor::NOIR, cost_n},
                {TokenColor::ROUGE, cost_r},
                {TokenColor::PERLE, cost_p}
            };

            //Prestige
            unsigned int prestige_points = sqlite3_column_int(stmtlvl3, 1);

            //Couronnes
            unsigned int crowns = sqlite3_column_int(stmtlvl3, 2);

            //Capacité
            const char* abi1 = reinterpret_cast<const char*>(sqlite3_column_text(stmtlvl3, 11));
            Abilities ability1 = Utility::stringToAbility(abi1);
            const char* abi2 = reinterpret_cast<const char*>(sqlite3_column_text(stmtlvl3, 12));
            Abilities ability2 = Utility::stringToAbility(abi2);

            //Bonus
            int bonus_nb = sqlite3_column_int(stmtlvl3, 12);
            const char* color = reinterpret_cast<const char*>(sqlite3_column_text(stmtlvl3, 9));
            Bonus bonus;
            bonus.bonus_color = toTokenColor(color);
            bonus.bonus_number = bonus_nb;

            //Id
            int id = sqlite3_column_int(stmtlvl3, 0);

            //Création et ajout de l'instance au deck
            JewelryCard* newCard = new JewelryCard(level, cost, prestige_points, crowns, ability1, ability2, bonus, id);
            row_level_three.push_back(std::move(newCard));
        }

}

Deck_level_one::Deck_level_one(const std::string& databaseSavePath) : pioche() {

    //cout << pathtodatabase << endl;
    sqlite3* db; //On créer une variable sqlite du nom de db

    int rc = sqlite3_open(databaseSavePath.c_str(), &db); //rc = return code, on ouvre la database

    if (rc) {
        std::cerr << "Erreur lors de l'ouverture de la base de données: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    const char* query = "SELECT * FROM deck1 WHERE level = 1"; //requete pour chercher carte lvl 1
    // Préparer la requête
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Erreur lors de la préparation de la requête: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        //Niveau
        int level = sqlite3_column_int(stmt, 10); //Level dans la 11ème colonne


        //Coûts
        unsigned int cost_w = sqlite3_column_int(stmt, 3);
        unsigned int cost_v = sqlite3_column_int(stmt, 4);
        unsigned int cost_n = sqlite3_column_int(stmt, 5);
        unsigned int cost_p = sqlite3_column_int(stmt, 6);
        unsigned int cost_r = sqlite3_column_int(stmt, 7);
        unsigned int cost_b = sqlite3_column_int(stmt, 8);
        std::unordered_map<TokenColor, int> cost = { //dans l'ordre BLEU, BLANC, VERT, NOIR, ROUGE, PERLE (modifiable)
            {TokenColor::BLEU, cost_b},
            {TokenColor::BLANC, cost_w},
            {TokenColor::VERT, cost_v},
            {TokenColor::NOIR, cost_n},
            {TokenColor::ROUGE, cost_r},
            {TokenColor::PERLE, cost_p}
        };

        //Prestige
        unsigned int prestige_points = sqlite3_column_int(stmt, 1);

        //Couronnes
        unsigned int crowns = sqlite3_column_int(stmt, 2);

        //Capacité
        const char* abi1 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
        Abilities ability1 = Utility::stringToAbility(abi1);
        const char* abi2 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
        Abilities ability2 = Utility::stringToAbility(abi2);

        //Bonus
        int bonus_nb = sqlite3_column_int(stmt, 12);
        const char* color = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
        Bonus bonus;
        bonus.bonus_color = toTokenColor(color);
        bonus.bonus_number = bonus_nb;

        //Id
        int id = sqlite3_column_int(stmt, 0);

        //Création et ajout de l'instance au deck
        JewelryCard* newCard = new JewelryCard(level, cost, prestige_points, crowns, ability1, ability2, bonus, id);
        Deck_level_one::addCardToDeck(newCard);

    }
}

Deck_level_two::Deck_level_two(const std::string& databaseSavePath) : pioche() {

    //cout << pathtodatabase << endl;
    sqlite3* db; //On créer une variable sqlite du nom de db

    int rc = sqlite3_open(databaseSavePath.c_str(), &db); //rc = return code, on ouvre la database

    if (rc) {
        std::cerr << "Erreur lors de l'ouverture de la base de données: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    const char* query = "SELECT * FROM deck2 WHERE level = 2"; //requete pour chercher carte lvl 1
    // Préparer la requête
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Erreur lors de la préparation de la requête: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        //Niveau
        int level = sqlite3_column_int(stmt, 10); //Level dans la 11ème colonne


        //Coûts
        unsigned int cost_w = sqlite3_column_int(stmt, 3);
        unsigned int cost_v = sqlite3_column_int(stmt, 4);
        unsigned int cost_n = sqlite3_column_int(stmt, 5);
        unsigned int cost_p = sqlite3_column_int(stmt, 6);
        unsigned int cost_r = sqlite3_column_int(stmt, 7);
        unsigned int cost_b = sqlite3_column_int(stmt, 8);
        std::unordered_map<TokenColor, int> cost = { //dans l'ordre BLEU, BLANC, VERT, NOIR, ROUGE, PERLE (modifiable)
            {TokenColor::BLEU, cost_b},
            {TokenColor::BLANC, cost_w},
            {TokenColor::VERT, cost_v},
            {TokenColor::NOIR, cost_n},
            {TokenColor::ROUGE, cost_r},
            {TokenColor::PERLE, cost_p}
        };

        //Prestige
        unsigned int prestige_points = sqlite3_column_int(stmt, 1);

        //Couronnes
        unsigned int crowns = sqlite3_column_int(stmt, 2);

        //Capacité
        const char* abi1 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
        Abilities ability1 = Utility::stringToAbility(abi1);
        const char* abi2 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
        Abilities ability2 = Utility::stringToAbility(abi2);

        //Bonus
        int bonus_nb = sqlite3_column_int(stmt, 12);
        const char* color = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
        Bonus bonus;
        bonus.bonus_color = toTokenColor(color);
        bonus.bonus_number = bonus_nb;

        //Id
        int id = sqlite3_column_int(stmt, 0);

        //Création et ajout de l'instance au deck
        JewelryCard* newCard = new JewelryCard(level, cost, prestige_points, crowns, ability1, ability2, bonus, id);
        Deck_level_two::addCardToDeck(newCard);

    }
}

Deck_level_three::Deck_level_three(const std::string& databaseSavePath) : pioche() {

    //cout << pathtodatabase << endl;
    sqlite3* db; //On créer une variable sqlite du nom de db

    int rc = sqlite3_open(databaseSavePath.c_str(), &db); //rc = return code, on ouvre la database

    if (rc) {
        std::cerr << "Erreur lors de l'ouverture de la base de données: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    const char* query = "SELECT * FROM deck3 WHERE level = 3"; //requete pour chercher carte lvl 3
    // Préparer la requête
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Erreur lors de la préparation de la requête: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        //Niveau
        int level = sqlite3_column_int(stmt, 10); //Level dans la 11ème colonne


        //Coûts
        unsigned int cost_w = sqlite3_column_int(stmt, 3);
        unsigned int cost_v = sqlite3_column_int(stmt, 4);
        unsigned int cost_n = sqlite3_column_int(stmt, 5);
        unsigned int cost_p = sqlite3_column_int(stmt, 6);
        unsigned int cost_r = sqlite3_column_int(stmt, 7);
        unsigned int cost_b = sqlite3_column_int(stmt, 8);
        std::unordered_map<TokenColor, int> cost = { //dans l'ordre BLEU, BLANC, VERT, NOIR, ROUGE, PERLE (modifiable)
            {TokenColor::BLEU, cost_b},
            {TokenColor::BLANC, cost_w},
            {TokenColor::VERT, cost_v},
            {TokenColor::NOIR, cost_n},
            {TokenColor::ROUGE, cost_r},
            {TokenColor::PERLE, cost_p}
        };

        //Prestige
        unsigned int prestige_points = sqlite3_column_int(stmt, 1);

        //Couronnes
        unsigned int crowns = sqlite3_column_int(stmt, 2);

        //Capacité
        const char* abi1 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
        Abilities ability1 = Utility::stringToAbility(abi1);
        const char* abi2 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
        Abilities ability2 = Utility::stringToAbility(abi2);

        //Bonus
        int bonus_nb = sqlite3_column_int(stmt, 12);
        const char* color = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
        Bonus bonus;
        bonus.bonus_color = toTokenColor(color);
        bonus.bonus_number = bonus_nb;

        //Id
        int id = sqlite3_column_int(stmt, 0);

        //Création et ajout de l'instance au deck
        JewelryCard* newCard = new JewelryCard(level, cost, prestige_points, crowns, ability1, ability2, bonus, id);
        Deck_level_three::addCardToDeck(newCard);

    }
}


Deck_Royal::Deck_Royal(const std::string& databaseSavePath) : cards() {
    //cout << pathtodatasavebase << endl;
    sqlite3* db; //On créer une variable sqlite du nom de db

    int rc = sqlite3_open(databaseSavePath.c_str(), &db); //rc = return code, on ouvre la database

    if (rc) {
        std::cerr << "Erreur lors de l'ouverture de la base de données: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    const char* query = "SELECT * FROM boardRoyal"; //requete pour chercher cartes royales
    // Préparer la requête
    sqlite3_stmt* stmt;
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
        const char* abi = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        Abilities ability = Utility::stringToAbility(abi);

        //Prestige
        unsigned int prestige_points = sqlite3_column_int(stmt, 2);

        //Création et ajout de l'instance au deck
        RoyalCard* newCard = new RoyalCard(prestige_points, ability, id);
        Deck_Royal::addCardToDeck(newCard);
    }
}