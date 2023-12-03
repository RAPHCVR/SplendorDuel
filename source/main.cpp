#include <iostream>
#include "Jeton.h"
#include "Partie.h"
//#include "QTJeton.h"
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

int main() {
    auto* director = new Director();
    string statut_partie="New";

    if (statut_partie == "New") {
        GameBuilder* builder = new GameBuilder();
        director->set_builder(builder);
        cout<<"HUMAIN vs HUMAIN"<<std::endl;
        cout<<"Veuillez saisir le pseudo du joueur 1"<<std::endl;
        string pseudo1;
        cin>>pseudo1;
        std::cout<<"Veuillez saisir le pseudo du joueur 2"<<std::endl;
        string pseudo2;
        cin>>pseudo2;
        director->BuildGame(pseudo1, Type::Humain, pseudo2, Type::Humain);
        Game* p = builder->GetProduct();
        delete director;
        p -> gametable -> getBag().addToken( p -> gametable -> getBoard().takeToken(0,0));
        p -> gametable -> getBoard().showBoard();
    }
}