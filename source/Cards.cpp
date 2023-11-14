#include <iostream>
#include <string.h>

#include "Cards.h"


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
