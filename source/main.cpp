#include <QApplication>
#include "Controller.h"
#include "Jeton.h"
#include "QTJeton.h"
#include <iostream>

#include "QTGame.h"
#include "Save.h"

/*
int main() {
    for (int i = 0; i < 100; i++) {
        std::cout << "i = " << i << std::endl;
        Controller *c = new Controller("New", "player1", "player2", Type::IA, Type::IA);
        c->play();
        c->reinit();
    }
}

*/

/* main de base
int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QTGame game;
    game.show();
    return QApplication::exec();
}
*/

int main() {
    //Pyramid_Cards* pyra = retrevePyramid();
    //Deck_level_one* dec1 = retrivedeck1();
    //Deck_level_two* dec2 = retrivedeck2();
    //Deck_level_three* dec3 = retrivedeck3();
    //Deck_Royal* deckR = retrivedeckroyal();
    //Board* boardjp = retriveboard();
    Player* player0 = retriveplayer(0); // NON FONCTIONNEL
    //Bag* sac = retrivebag();
    //void retrivereserve(); // ??
    return 0;
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
