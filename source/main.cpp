#include <iostream>
#include "Jeton.h"

#include "QTJeton.h"
#include <QApplication>


int main(int argc, char *argv[]) {
        QApplication app(argc, argv);
        try
        {
            //Instanciation des Jetons
            TotalTokens totalTokens;
            //Instanciation du sac
            Bag bag(totalTokens);
            //Creation du plateau
            TotalPrivileges totalPrivileges;
            Board board(bag, totalPrivileges);
            //Test affichage du plateau
            board.showBoard();
            //Test prendre un jeton
            Token t = board.takeToken(0,0);
            board.showBoard();
            std::cout << t << std::endl;
            MainWindow mainWindow1(board);
            mainWindow1.show();
            return app.exec();
        }
        catch (TokenException& err)
        {
            std::cout << err.getMessage() << "\n";
        }
}
