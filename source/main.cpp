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
    MainWindow *mainWindow1 = new MainWindow();
    mainWindow1->show();
    return QApplication::exec();
}

/*
int main() {
    Board &board = Board::getInstance();
    unsigned int a = board.getNbPrivileges();
    std::string s = "Joueur 1";
    Player player1(s, Type::Humain);
    player1.addPrivilege(board.takePrivilege());
    a = board.getNbPrivileges();
    unsigned int b = player1.getPrivilege();
    board.placePrivilege(player1.removePrivilege());
    a = board.getNbPrivileges();
    b = player1.getPrivilege();
    return 0;
}
*/