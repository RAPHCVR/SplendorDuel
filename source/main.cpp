#include <QApplication>
#include "Controller.h"
#include "Jeton.h"
#include "QTJeton.h"
#include <iostream>

#include "QTGame.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QTGame game;
    game.show();
    return QApplication::exec();
}


/*
int main() {
    Controller* cont = new Controller("Save", "NULL", "NULL", Type::IA, Type::Humain);
    cont->play();
    return 0;
}
*/