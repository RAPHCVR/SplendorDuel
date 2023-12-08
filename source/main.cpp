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
    Deck_level_one* a = Deck_level_one::getInstance();
    std::string s = "Joueur 1";
    Player player1(s, Type::Humain);
    player1.addJewelryCard(*a->getPioche()[0]);
    return 0;
}
*/