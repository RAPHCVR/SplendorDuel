#include <QApplication>
#include "Controller.h"
#include "Jeton.h"
#include "QTJeton.h"
#include <iostream>

#include "QTGame.h"

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


int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QTGame game;
    game.show();
    return QApplication::exec();
}

/*
int main() {
    Controller* controller = new Controller();
    JewelryCard *card;
    controller->getopposingPlayer().addToken(controller->getGame().getGameTable().getBoard().takeToken(0,0));
    while (card->getAbility1()!=Abilities::steal_token) {
        card = &controller->getGame().getGameTable().getPyramid().takeCard(2,1);
        controller->getGame().getGameTable().getPyramid().drawCard(2);
        controller->getcurrentPlayer().addJewelryCard(*card);
    }
    controller->applyCardSkills(controller->getGame(),controller->getcurrentPlayer(),controller->getopposingPlayer(),*card);
    controller->play();
    return 0;
}
*/

/*
int main() {
    std::vector<std::pair<int, int>> grid = {{3, 3}, {4, 4}, {5, 4}};
    std::vector<std::pair<int, int>>* gridPtr = &grid;

    if (areCoordinatesAlignedAndConsecutive(gridPtr)) {
        std::cout << "The coordinates are aligned and consecutive." << std::endl;
    } else {
        std::cout << "The coordinates are not aligned or not consecutive." << std::endl;
    }

    return 0;
}
*/
