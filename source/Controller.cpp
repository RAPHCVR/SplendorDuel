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
        currentPlayer= game->getPlayer(0);
    }
}

void Controller::setCurrentPlayer(unsigned int n) {
    currentPlayer = game->getPlayer(n);
}

void Controller::changeCurrentPlayer() {
    if (currentPlayer == game->getPlayer(0)) {
        currentPlayer = game->getPlayer(1);
    } else {
        currentPlayer = game->getPlayer(0);
    }
}

std::vector<OptionalActions> Controller::getOptionalActions(const Game &game, Player &player) const {
    std::vector<OptionalActions> result;
    result.push_back(OptionalActions::Empty);
    if (player.getPrivilege() > 0){
        result.push_back(OptionalActions::UsePrivileges);
    }
    if (not (game.getGameTable().getBag().isEmpty())){
        result.push_back(OptionalActions::FillBoard);
    }
    return result;
}

std::vector<CompulsoryActions> Controller::getCompulsoryActions(const Game &game, Player &player) const{
    std::vector<CompulsoryActions> result;
    // On verifie si y a des jetons et que ce n'est pas que des jetons Gold
    if ((not (game.getGameTable().getBoard().isEmpty())) and (not game.getGameTable().getBoard().containsOnly(TokenColor::OR))){
        result.push_back(CompulsoryActions::TakeCoins);
    }

    // On verifie si on peut reserver : on verifie si on a au moins un jeton or et que les paquets ne sont pas vides
    if (game.getGameTable().getBoard().hasTokenOfColor(TokenColor::OR)){
        if ((not (game.getGameTable().getPyramid().isEmpty(1))) || (not (game.getGameTable().getPyramid().isEmpty(2))) || (not (game.getGameTable().getPyramid().isEmpty(3)))){
            result.push_back(CompulsoryActions::ReserveCard);
        }
    }

    // On verifie si le joueur peut acheter au moins une carte
    bool hasBeenAdded = false;
    for (auto card : game.getGameTable().getPyramid().getLevelCards(1)){
        if (player.canBuyCard(*card)){
            result.push_back(CompulsoryActions::BuyCard);
            hasBeenAdded = true;
            break;
        }
    }
    if (not hasBeenAdded){
        for (auto card : game.getGameTable().getPyramid().getLevelCards(2)){
            if (player.canBuyCard(*card)){
                result.push_back(CompulsoryActions::BuyCard);
                hasBeenAdded = true;
                break;
            }
        }
    }

    if (not hasBeenAdded) {
        for (auto card: game.getGameTable().getPyramid().getLevelCards(3)) {
            if (player.canBuyCard(*card)) {
                result.push_back(CompulsoryActions::BuyCard);

                break;
            }
        }
    }
    return result;
}

