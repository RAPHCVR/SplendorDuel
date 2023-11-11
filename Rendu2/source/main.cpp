#include <iostream>
#include "Jeton.h"

int main() {

        try
        {
            //Instanciation des Jetons
            TotalTokens totalTokens;
            //Instanciation du sac
            Bag bag(totalTokens);
            //Creation du plateau
            TotalPrivileges totalPrivileges;
            Board board(bag, totalPrivileges);

            //Test takeToken
            const Token& token1 = board.takeToken(0,1);
            std::cout << token1 << "\n";
            bag.addToken(token1);

            const Token& token2 = board.takeToken(4,4);
            std::cout << token2 << "\n";
            bag.addToken(token2);
            board.showBoard();
            //Test algo placement Jetons
            board.placeToken(bag.drawToken());
            board.showBoard();
        }
        catch (TokenException& err)
        {
            std::cout << err.getMessage() << "\n";
        }
    return 0;
}
