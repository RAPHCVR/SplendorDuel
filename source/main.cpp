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
    MainWindow *mainWindow1 = new MainWindow();
    mainWindow1->show();
    app.processEvents();
    std::cout<<"Appuyez sur une touche et entree pour continuer"<<std::endl;
    std::cin.get();
    controller->getGame().getGameTable().getBoard().takeToken(0,0);
    std::vector<OptionalActions> v = controller->getOptionalActions(controller->getGame(),controller->getcurrentPlayer());
    controller->getGame().getGameTable().getBoard().showBoard();
    return QApplication::exec();
}
