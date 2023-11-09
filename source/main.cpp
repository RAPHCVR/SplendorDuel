#include <iostream>
#include "Jeton.h"

int main() {

        //Jetons

        try
        {
            //Création du lot
            TotalTokens lotJetons;
            //Création du sac
            Bag sac(lotJetons);
            //Creation du plateau
            TotalPrivileges lotPrivileges;
            Board board(sac, lotPrivileges);

            //Recuperation de deux jetons sur le plateau et remise dans le sac
            const Token& jeton1 = board.takeToken(0,0);
            std::cout << jeton1 << "\n";
            sac.addToken(jeton1);

            const Token& jeton2 = board.takeToken(2,2);
            std::cout << jeton2 << "\n";
            sac.addToken(jeton2);

            //Remise d'un jeton du sac sur le plateau
            board.placeToken(sac.drawToken());
            board.showBoard();
        }
        catch (TokenException& e)
        {
            std::cout << e.getMessage() << "\n";
        }
    return 0;
}
