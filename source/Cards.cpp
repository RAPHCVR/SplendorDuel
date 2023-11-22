#include <iostream>
#include <string.h>
#include "sqlite/sqlite3.h"
#include "Cards.h"
#include "Jeton.h"


void Deck_level_one::createDeckFromDataBase(){
    
    sqlite3 *db; //On créer une variable sqlite du nom de db
    int rc = sqlite3_open("cards.db", &db); //rc = return code, on ouvre la database 
    
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
        unsigned int level = sqlite3_column_int(stmt, 9); //Level dans la 9ème colonne
        
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
        const char *abi = sqlite3_column_int(stmt, 10);
        Abilities ability = Abilities::*abi ;
        
        //Bonus
        const char *color = sqlite3_column_int(stmt, 8);
        Bonus bonus;
        bonus.bonus_color = TokenColor::*color;
        bonus.bonus_number = sqlite3_column_int(stmt, 11);

        //Création et ajout de l'instance au deck
        JewelryCard *newCard = new JewelryCard(level, cost, prestige_points, crowns, ability, bonus); 
        addCardToDeck(newCard);
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
void Pyramid_Cards::drawCard(unsigned int level){
    JewelryCard *Card_Temp;
    switch(level){
            case 1 :
                Card_Temp = Deck_level_one::pioche.begin();
                Deck_level_one::pioche.erase(Deck_level_one::pioche.begin());
                row_level_one.push_back(Card_Temp);
                //row_level_one[position_level_one] = Card_Temp;
            case 2 :
                Card_Temp = Deck_level_two::pioche.begin();
                Deck_level_two::pioche.erase(Deck_level_two::pioche.begin());
                row_level_two.push_back(Card_Temp);
                //row_level_two[position_level_two] = Card_Temp;
            case 3 :
                Card_Temp = Deck_level_three::pioche.begin();
                Deck_level_three::pioche.erase(Deck_level_three::pioche.begin());
                row_level_three.push_back(Card_Temp);
                //row_level_three[position_level_three] = Card_Temp;
            default :
                throw std::runtime_error("Niveau non existant");
            
    }
}
    

void SummaryCard::addBonusNumber(unsigned int b){
   this->bonusNumber += b;
}

void SummaryCard::addprestigePoints(unsigned int p){
    this->prestigePoints += p;
}
