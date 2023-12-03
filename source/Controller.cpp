//
// Created by world on 03/12/2023.
//

#include "Controller.h"

Controller::Controller() {
    auto* director = new Director();
    //A MODIF: Sauvegarde de la partie/Nouvelle partie
    std::string statut_partie="New";

    if (statut_partie == "New") {
        GameBuilder* builder = new GameBuilder();
        director->set_builder(builder);
        std::cout<<"HUMAIN vs HUMAIN"<<std::endl;
        //std::cout<<"Veuillez saisir le pseudo du joueur 1"<<std::endl;
        std::string pseudo1 = "Joueur 1";
        //std::cin>>pseudo1;
        //std::cout<<"Veuillez saisir le pseudo du joueur 2"<<std::endl;
        std::string pseudo2 = "Joueur 2";
        //std::cin>>pseudo2;
        director->BuildGame(pseudo1, Type::Humain, pseudo2, Type::Humain);
        Game* p = builder->GetProduct();
        delete director;
        game = p;
    }
}