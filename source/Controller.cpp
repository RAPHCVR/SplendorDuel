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
        srand(time(0));
        unsigned int i = rand() % 2;
        currentPlayer= game->getPlayer(i);
        getopposingPlayer().addPrivilege(game->getGameTable().getBoard().takePrivilege());
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
    if (player.getNbPrivilege() > 0 && not (game.getGameTable().getBoard().isEmpty())){
        result.push_back(OptionalActions::UsePrivileges);
    }
    if (not (game.getGameTable().getBag().isEmpty())){
        result.push_back(OptionalActions::FillBoard);
    }
    result.push_back(OptionalActions::Empty);
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
        if (((not (game.getGameTable().getPyramid().isEmpty(1))) || (not (game.getGameTable().getPyramid().isEmpty(2))) || (not (game.getGameTable().getPyramid().isEmpty(3)))) && (player.canReserveCard())){
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

void Controller::applyOptionalAction(Game &game, Player &player, OptionalActions action) {
    switch (action) {
        case OptionalActions::UsePrivileges:
            usePriviledge(game.getGameTable().getBoard());
            break;
        case OptionalActions::FillBoard:
            fillBoard(game.getGameTable().getBoard(), game.getGameTable().getBag());
            break;
        case OptionalActions::Empty:
            break;
    }
}

void Controller::applyCompulsoryAction(Game &game, Player &player, CompulsoryActions action) {
    std::vector<const Token*> tokens;
    bool check = true;
    unsigned int counter = 0;
    TokenColor color = TokenColor::None;
    switch (action) {
        case CompulsoryActions::TakeCoins:
            std::cout << game.getGameTable().getPyramid() << std::endl;
            for (unsigned int i = 0; i < 3; i++) {
                    tokens.push_back(&chooseToken(game.getGameTable().getBoard(), player));
            }
            for (auto token : tokens) {
                if (token!=nullptr) {
                    if (color==TokenColor::None) {
                        color = token->getColor();
                    }
                    else if (color!=token->getColor()) {
                        check = false;
                    }
                }
            }
            if (not(check)) {
                for (auto token : tokens) {
                    if (token!=nullptr) {
                        if (token->getColor()==TokenColor::PERLE) {
                            counter++;
                        }
                    }
                }
                if (counter == 2) {
                    check = true;
                }
            }
            if (check) {
                if (game.gametable->getBoard().getNbPrivileges()>0) {
                    getopposingPlayer().addPrivilege(game.gametable->getBoard().takePrivilege());
                }
                else if (currentPlayer->getNbPrivilege()>0) {
                    getopposingPlayer().addPrivilege(currentPlayer->removePrivilege());
                }
            }
            break;
        case CompulsoryActions::ReserveCard:
            bookCard(game.getGameTable().getPyramid(), game.getGameTable());
            break;
        case CompulsoryActions::BuyCard:
            buyJewelryCard(game.getGameTable());
            break;
    }
}

void Controller::usePriviledge(Board& board) {
    unsigned int nbp = currentPlayer->getNbPrivilege();
    unsigned int nbt = board.getNbTokens();
    unsigned int nb = std::min(nbp,nbt);
    std::cout << "Combien de privileges voulez vous utiliser ? Vous pouvez en utiliser " << nb << std::endl;
    unsigned int nbPrivilege = choiceMaker(0, nb);
    for (unsigned int i = 0; i < nbPrivilege; i++) {
        board.placePrivilege(currentPlayer->removePrivilege());
        chooseToken(board, *currentPlayer);
    }
}

const Token& Controller::chooseToken(Board&board, Player&player) {
    std::cout << "Veuillez choisir un jeton" << std::endl;
    board.showBoard();
    unsigned int x = choiceMaker(1, 5);
    unsigned int y = choiceMaker(1, 5);
    if(game->gametable->getBoard().isCellEmpty(x-1,y-1))
        throw TokenException("L'emplacement ne contient pas de jeton\n");
    if(game->gametable->getBoard().CellColor(x-1, y-1,TokenColor::OR))
        throw TokenException("Impossible de prendre un jeton or\n");
    const Token& token = board.takeToken(x-1, y-1);
    player.addToken(token);
    return token;
}

void Controller::chooseGoldenToken(Board&board, Player&player) {
    std::cout << "Veuillez choisir un jeton" << std::endl;
    board.showBoard();
    unsigned int x = choiceMaker(1, 5);
    unsigned int y = choiceMaker(1, 5);
    if(game->gametable->getBoard().isCellEmpty(x-1,y-1))
        throw TokenException("L'emplacement ne contient pas de jeton\n");
    if(not(game->gametable->getBoard().CellColor(x-1, y-1,TokenColor::OR)))
        throw TokenException("Vous devez choisir un jeton or\n");
    const Token& token = board.takeToken(x-1, y-1);
    player.addToken(token);
}


void Controller::fillBoard(Board&board, Bag&bag) {
    if (board.getNbPrivileges()!=0) {
        getopposingPlayer().addPrivilege(board.takePrivilege());
    }
    else if (currentPlayer->getNbPrivilege()!=0) {
        getopposingPlayer().addPrivilege(currentPlayer->removePrivilege());
    }
    board.fillBoard(bag);
}

void Controller::bookCard(Pyramid_Cards& pyramid, GameTable& gametable) {
    std::cout << "Voulez vous reserver une carte d'une des pioches (1) ou une carte de la pyramide (2) ?" << std::endl;
    unsigned int choice = choiceMaker(1, 2);
    if (choice == 1) {
        std::cout << "Veuillez choisir une pioche" << std::endl;
        std::cout << "1. Pioche niveau 1" << std::endl;
        std::cout << "2. Pioche niveau 2" << std::endl;
        std::cout << "3. Pioche niveau 3" << std::endl;
        unsigned int choice2 = choiceMaker(1, 3);
        JewelryCard& card = takeCard(choice2);
        chooseGoldenToken(gametable.getBoard(), *currentPlayer);
        currentPlayer->reserveOneCard(card);
    }
    else {
        std::cout << gametable.getPyramid() << std::endl;
        unsigned int level = choiceMaker(1, 3);
        unsigned int nb = pyramid.getLevelCards(level).size();
        unsigned int nbCard = choiceMaker(1, nb);
        chooseGoldenToken(gametable.getBoard(), *currentPlayer);
        currentPlayer->reserveOneCard(pyramid.takeCard(level,nbCard-1));
    }
}

void Controller::buyJewelryCard(GameTable& gametable) {
    bool bought = false;
    if (currentPlayer->getReserve().size() > 0) {
        std::cout << "Voulez vous acheter une carte reservee ?" << std::endl;
        std::cout << "1. Oui" << std::endl;
        std::cout << "2. Non" << std::endl;
        unsigned int choice = choiceMaker(1, 2);
        if (choice == 1) {
            bought = true;
            std::cout << "Veuillez choisir une carte" << std::endl;
            for (auto card : currentPlayer->getReserve()) {
                std::cout << card << std::endl;
            }
            unsigned int nbCard = choiceMaker(1, currentPlayer->getReserve().size());
            JewelryCard& card = *currentPlayer->getReserve()[nbCard - 1];
            if (currentPlayer->canBuyCard(card)) {
                currentPlayer->actionBuyReservedCard(card);
                return;
            }
        }
    }
    if (not(bought)) {
        std::cout << gametable.getPyramid() << std::endl;
        unsigned int level = choiceMaker(1, 3);
        unsigned int nb = gametable.getPyramid().getLevelCards(level).size();
        unsigned int nbCard = choiceMaker(1, nb);
        JewelryCard& card = gametable.getPyramid().takeCard(level, nbCard - 1);
        if (currentPlayer->canBuyCard(card)) {
            currentPlayer->actionBuyCard(card);
        }
    }
}

void Controller::buyNobleCard() {
    std::cout << "Veuillez choisir une carte" << std::endl;
    for (auto card : Deck_Royal::getInstance()->getCards()) {
        std::cout << card << std::endl;
    }
    unsigned int nbCard = choiceMaker(1, Deck_Royal::getInstance()->getCards().size());
    RoyalCard& card = *Deck_Royal::getInstance()->getCards()[nbCard - 1];
    currentPlayer->addRoyalCard(card);
}

void Controller::applyCardSkills(Game&game, Player&cardOwner, Player&opponent, JewelryCard&card) {
    if (card.getAbility1() == Abilities::take_privilege) {
        if (getGame().getGameTable().getBoard().getNbPrivileges()!=0) {
            getopposingPlayer().addPrivilege(getGame().getGameTable().getBoard().takePrivilege());
        }
        else if (getopposingPlayer().getNbPrivilege()!=0) {
            currentPlayer->addPrivilege(currentPlayer->removePrivilege());
        }
    }
    else if (card.getAbility1() == Abilities::take_bonus_token) {
        chooseToken(game.getGameTable().getBoard(), cardOwner);
    }
    else if (card.getAbility1() == Abilities::steal_token) {
        std::cout << "Le joueur adverse a pour jetons : " << std::endl;
        for (auto [color,nb] : getopposingPlayer().getTokenSummary()){
            std::cout << nb << " jetons " << color << std::endl;
        }
        std::cout << "Veuillez choisir une couleur de jeton  (NOIR, ROUGE, BLEU, VERT, BLANC, PERLE)" << std::endl;
        std::string rep;
        std::cin >> rep;
        TokenColor color = toTokenColor(rep);
        if (getopposingPlayer().getTokenSummary().find(color) == getopposingPlayer().getTokenSummary().end()){
            throw TokenException("Le joueur adverse n'a pas de jeton de cette couleur");
        }
        else {
            const Token& token = getopposingPlayer().removeToken(color);
            cardOwner.addToken(token);
        }
    }
    else if (card.getAbility1() == Abilities::cameleon) {
        std::cout << "Veuillez choisir une couleur de jeton  (NOIR, ROUGE, BLEU, VERT, BLANC, PERLE)" << std::endl;
        std::string rep;
        std::cin >> rep;
        TokenColor color = toTokenColor(rep);
        if (cardOwner.getTokenSummary().find(color) == cardOwner.getTokenSummary().end()){
            throw TokenException("Vous n'avez pas de jeton de cette couleur");
        }
        else {
            cardOwner.getColorSummary(color).addBonusNumber(1);
        }
    }
    else if (card.getAbility1() == Abilities::repeat_turn) {
        playTurn();
    }
    if (card.getAbility2() == Abilities::take_privilege) {
        if (getGame().getGameTable().getBoard().getNbPrivileges()!=0) {
            getopposingPlayer().addPrivilege(getGame().getGameTable().getBoard().takePrivilege());
        }
        else if (getopposingPlayer().getNbPrivilege()!=0) {
            currentPlayer->addPrivilege(currentPlayer->removePrivilege());
        }
    }
    else if (card.getAbility2() == Abilities::take_bonus_token) {
        chooseToken(game.getGameTable().getBoard(), cardOwner);
    }
    else if (card.getAbility2() == Abilities::steal_token) {
        std::cout << "Le joueur adverse a pour jetons : " << std::endl;
        for (auto [color,nb] : getopposingPlayer().getTokenSummary()){
            std::cout << nb << " jetons " << color << std::endl;
        }
        std::cout << "Veuillez choisir une couleur de jeton  (NOIR, ROUGE, BLEU, VERT, BLANC, PERLE)" << std::endl;
        std::string rep;
        std::cin >> rep;
        TokenColor color = toTokenColor(rep);
        if (getopposingPlayer().getTokenSummary().find(color) == getopposingPlayer().getTokenSummary().end()){
            throw TokenException("Le joueur adverse n'a pas de jeton de cette couleur");
        }
        else {
            const Token& token = getopposingPlayer().removeToken(color);
            cardOwner.addToken(token);
        }
    }
    else if (card.getAbility2() == Abilities::cameleon) {
        std::cout << "Veuillez choisir une couleur de jeton  (NOIR, ROUGE, BLEU, VERT, BLANC, PERLE)" << std::endl;
        std::string rep;
        std::cin >> rep;
        TokenColor color = toTokenColor(rep);
        if (cardOwner.getTokenSummary().find(color) == cardOwner.getTokenSummary().end()){
            throw TokenException("Vous n'avez pas de jeton de cette couleur");
        }
        else {
            cardOwner.getColorSummary(color).addBonusNumber(1);
        }
    }
    else if (card.getAbility2() == Abilities::repeat_turn) {
        playTurn();
    }
}


unsigned choiceMaker(unsigned a, unsigned b) {
    unsigned int choice;
    std::cout << "Veuillez choisir entre " << a << " et " << b << std::endl;
    std::cin >> choice;
    while (choice < a || choice > b){
        std::cout << "Veuillez choisir entre " << a << " et " << b << std::endl;
        std::cin >> choice;
    }
    return choice;
}

void Controller::play() {
    while (not checkIfPlayerWins(*game, getopposingPlayer())) {
        playTurn();
        if(currentPlayer->getCrowns() == 3 || currentPlayer->getCrowns() == 6){
            buyNobleCard();
        }
        changeCurrentPlayer();
    }
    std::cout << "Le joueur " << currentPlayer->getName() << " a gagné" << std::endl;
}

void Controller::playTurn() {
    std::cout << "C'est au tour de " << currentPlayer->getName() << std::endl;
    std::vector<CompulsoryActions> compulsoryActions = getCompulsoryActions(*game, *currentPlayer);
    unsigned int choice = 0;
    if (compulsoryActions.empty()) {
        applyOptionalAction(*game, *currentPlayer, OptionalActions::FillBoard);
        std::cout << "Pas d'action obligatoires possibles, remplisage du plateau" << std::endl;
    }
    else {
        std::vector<OptionalActions> optionalActions = getOptionalActions(*game, *currentPlayer);
        while (choice != optionalActions.size() && optionalActions.size() !=1) {
            std::cout << "Veuillez choisir une action optionnelle" << std::endl;
            for (auto action : optionalActions) {
                switch (action) {
                    case OptionalActions::Empty:
                        std::cout << "Ne rien faire" << std::endl;
                    break;
                    case OptionalActions::UsePrivileges:
                        std::cout << "Utiliser des privileges" << std::endl;
                    break;
                    case OptionalActions::FillBoard:
                        std::cout << "Remplir le plateau" << std::endl;
                    break;
                }
            }
            choice = choiceMaker(1, optionalActions.size());
            applyOptionalAction(*game, *currentPlayer, optionalActions[choice-1]);
            if (optionalActions[choice-1] == OptionalActions::FillBoard || currentPlayer->getNbPrivilege() == 0){
                choice = optionalActions.size();
            }
        }
    }
    compulsoryActions = getCompulsoryActions(*game, *currentPlayer);
    std::cout << "Veuillez choisir une action obligatoire" << std::endl;
    for (auto action : compulsoryActions) {
        switch (action) {
            case CompulsoryActions::TakeCoins:
                std::cout << "Prendre des jetons" << std::endl;
            break;
            case CompulsoryActions::ReserveCard:
                std::cout << "Reserver une carte" << std::endl;
            break;
            case CompulsoryActions::BuyCard:
                std::cout << "Acheter une carte" << std::endl;
            break;
        }
    }
    choice = choiceMaker(1, compulsoryActions.size());
    applyCompulsoryAction(*game, *currentPlayer, compulsoryActions[choice-1]);
}

bool Controller::checkIfPlayerWins(Game& game, Player& player) {
    if (player.getPrestige() >= 20) {
        return true;
    }
    else if (player.getCrowns()>=10) {
        return true;
    }
    else if(player.getMaxPrestigeColor()>=10) {
        return true;
    }

    return false;
}
