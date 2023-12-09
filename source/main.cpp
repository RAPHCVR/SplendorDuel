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
    Controller* controller = new Controller();
    JewelryCard *card;
    for (int i = 0; i<5 ; i++) {
        //take a card from pioche lv 1 and add it to the player's jewelryCards
        card = controller->getGame().getGameTable().getPyramid().getLevelCards(1)[i];
        controller->getcurrentPlayer().addJewelryCard(*card);
    }
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