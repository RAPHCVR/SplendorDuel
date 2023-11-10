#include <iostream>
#include <string.h>

#include "Cards.h"


//PENSER AU CAS OU SI MANQUE DE CARTE DANS LA PIOCHE (SEMBLE IMPOSSIBLE)
JewelryCard JewelryCard::takeCard(unsigned int level, unsigned int position){
    switch(level){
        case 1:
            if(position>5){
                throw std::runtime_error("Position trop élevée !");
            }
            else{
                position_level_one = position;
                return row_level_one[position];
            }
        case 2:
            if(position>4){
                throw std::runtime_error("Position trop élevée !");
            }
            else{
                position_level_two = position;
                return row_level_two[position];
            }
        case 3:
            if(position>3){
                throw std::runtime_error("Position trop élevée !");
            }
            else{
                position_level_three = position;
                return row_level_three[position];
            }
            
 
    }
 
    
}


void drawCard(){
    
}
    
