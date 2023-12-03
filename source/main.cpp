#include <iostream>
#include "Jeton.h"
#include "Controller.h"
#include "QTJeton.h"
#include <QApplication>

/*
int main(int argc, char *argv[]) {
        QApplication app(argc, argv);
        try
        {
            //Creation du plateau
            Board& board = Board::getInstance();
            //Test prendre un jeton
            const Token t = board.takeToken(0,0);
            board.showBoard();
            Board& board2 = Board::getInstance();
            std::cout << t << std::endl;
            MainWindow mainWindow1(board2);
            mainWindow1.show();
            return QApplication::exec();
        }
        catch (TokenException& err)
        {
            std::cout << err.getMessage() << "\n";
        }
}
*/


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Controller* controller = new Controller();
    MainWindow mainWindow1(controller->getGame().getGameTable().getBoard());
    mainWindow1.show();
    std::cin.get();
    controller->getGame().getGameTable().getBoard().takePrivilege();
    unsigned int a =controller->getGame().getGameTable().getBoard().getNbPrivileges();
    return QApplication::exec();
}
