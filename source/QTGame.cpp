//
// Created by utcpret on 13/12/2023.
//

#include "QTGame.h"

QTGame::QTGame(QWidget* parent) : QWidget(parent) {
    controller = new Controller();
    screen = QGuiApplication::primaryScreen();
    size = new QSize(screen->size()/2);
    width = size->width();
    height = size->height();
    setFixedSize(width, height);
    mainlayout = new QVBoxLayout(this);
    first = new QHBoxLayout();
    second = new QHBoxLayout();
    plateView = new PlateView(nullptr, height-100,width/2);
    first->addWidget(plateView);
    mainlayout->addLayout(first);
    mainlayout->addLayout(second);
    setLayout(mainlayout);
    connect(plateView, &PlateView::tokensValidated, this, &QTGame::handleTokenSelection);
    connect(plateView, &PlateView::privilegeUsed, this, &QTGame::placePrivilege);
    connect(plateView, &PlateView::endOfTurn, this, &QTGame::handleGameStatus);
    status = "start";
    play();
}

void QTGame::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 5));
    painter.drawRect(rect());
}

void QTGame::handleTokenSelection(std::vector<const Token*> tokens) {
    for (auto token : tokens) {
        controller->getcurrentPlayer().addToken(*token);
    }
    if (plateView->getStatus() == "take3tokens") {
        bool check = true;
        unsigned int counter = 0;
        TokenColor color = TokenColor::None;
        if (tokens.size()!=3) {
            check = false;
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
            if (controller->getGame().getGameTable().getBoard().getNbPrivileges()>0) {
                takePrivilege(controller->getopposingPlayer());
            }
            else if (controller->getcurrentPlayer().getNbPrivilege()>0) {
                controller->getopposingPlayer().addPrivilege(controller->getcurrentPlayer().removePrivilege());
            }
        }
    }
}

void QTGame::fillBoard() {
    try {
        controller->getGame().getGameTable().getBoard().fillBoard(controller->getGame().getGameTable().getBag());
        plateView->updateWidgetsFromBoard();

    }
    catch (TokenException& err) {
        std::cout << err.getMessage() << "\n";
    }
}

void QTGame::takePrivilege(Player& player) {
    try {
        player.addPrivilege(controller->getGame().getGameTable().getBoard().takePrivilege());
        plateView->updatePrivilegeCounter();
    }
    catch (TokenException& err) {
        std::cout << err.getMessage() << "\n";
    }
}

void QTGame::placePrivilege(unsigned int nb) {
    try {
        for (unsigned int i = 0; i < nb; i++) {
            controller->getGame().getGameTable().getBoard().placePrivilege(controller->getcurrentPlayer().removePrivilege());
        }
        plateView->updatePrivilegeCounter();
    }
    catch (TokenException& err) {
        std::cout << err.getMessage() << "\n";
    }
}
void QTGame::usePriviledge() {
    unsigned int nbp = controller->getcurrentPlayer().getNbPrivilege();
    unsigned int nbt = Board::getInstance().getNbTokens();
    unsigned int nb = std::min(nbp,nbt);
    std::cout << "Combien de privileges voulez vous utiliser ? Vous pouvez en utiliser " << nb << std::endl;
    plateView->updateMaxNbSelectedTokens(nb);
    plateView->updateStatus("privileges");
}

void QTGame::applyOptionalAction(OptionalActions action) {
    switch (action) {
        case OptionalActions::UsePrivileges:
            usePriviledge();
        break;
        case OptionalActions::FillBoard:
            fillBoard();
            status = "compulsoryActions";
            handleGameStatus();
        break;
        case OptionalActions::Empty:
            status = "compulsoryActions";
            handleGameStatus();
            break;
    }
}

void QTGame::handleGameStatus(){
    if (status == "start") {
        play();
    }
    else if (status == "optionalActions"){
        if (!(controller->getOptionalActions(controller->getGame(), controller->getcurrentPlayer())==std::vector<OptionalActions>{OptionalActions::Empty})){
            playOptionalActions();
        }
        else {
            status = "compulsoryActions";
            handleGameStatus();
        }
    }
    else if (status == "compulsoryActions") {
        playCompulsoryActions();
    }
    else if (status == "check") {
        checkEndTurn();
    }
    else if (status == "end") {
        controller->changeCurrentPlayer();
        status = "start";
        play();
    }
}

void QTGame::play() {
    std::cout << "C'est au tour de " << controller->getcurrentPlayer().getName() << std::endl;
    std::cout << controller->getcurrentPlayer() << std::endl;
    std::vector<CompulsoryActions> compulsoryActions = controller->getCompulsoryActions(controller->getGame(), controller->getcurrentPlayer());
    if (compulsoryActions.empty()) {
        applyOptionalAction(OptionalActions::FillBoard);
        std::cout << "Pas d'action obligatoires possibles, remplissage du plateau" << std::endl;
    }
    else {
        status = "optionalActions";
        handleGameStatus();
    }
}

void QTGame::playOptionalActions(){
    std::vector<OptionalActions> optionalActions = controller->getOptionalActions(controller->getGame(), controller->getcurrentPlayer());
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
    unsigned int choice = choiceMaker(1, optionalActions.size());
    applyOptionalAction(optionalActions[choice-1]);
}

void QTGame::playCompulsoryActions(){
    std::vector<CompulsoryActions> compulsoryActions = controller->getCompulsoryActions(controller->getGame(), controller->getcurrentPlayer());
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
    unsigned int choice = choiceMaker(1, compulsoryActions.size());
    applyCompulsoryAction(compulsoryActions[choice-1]);
}

void QTGame::applyCompulsoryAction(CompulsoryActions action) {
    switch (action) {
        case CompulsoryActions::TakeCoins:
            std::cout << controller->getGame().getGameTable().getPyramid() << std::endl;
            std::cout << "Veuillez choisir des jetons" << std::endl;
            plateView->updateStatus("take3tokens");
            plateView->updateMaxNbSelectedTokens(3);
            status = "check";
            break;
        case CompulsoryActions::ReserveCard:
            bookCard(controller->getGame().getGameTable().getPyramid(), controller->getGame().getGameTable());
            status = "check";
            break;
        case CompulsoryActions::BuyCard:
            controller->buyJewelryCard(controller->getGame().getGameTable());
            status = "check";
            handleGameStatus();
            break;
    }
}

void QTGame::checkEndTurn() {
    if (controller->getcurrentPlayer().getNbTokens()>10) {
        unsigned int nb = - (10 - controller->getcurrentPlayer().getNbTokens());
        std::cout << "Voici vos Jetons, vous devez en retirer " << nb << " pour n'en conserver que 10 : " << std::endl;
        std::cout << controller->getcurrentPlayer() << std::endl;
        while (nb != 0) {
            std::cout << "Veuillez choisir une couleur de jeton  (NOIR, ROUGE, BLEU, VERT, BLANC, PERLE, OR)" << std::endl;
            std::string rep;
            std::cin >> rep;
            if (rep != "NOIR" && rep != "ROUGE" && rep != "BLEU" && rep != "VERT" && rep != "BLANC" && rep != "PERLE" && rep != "OR") {
                throw TokenException("Couleur invalide");
            }
            TokenColor color = toTokenColor(rep);
            std::cout << "Choisissez combien vous voulez en retirer " << std::endl;
            unsigned int tot = choiceMaker(1, controller->getcurrentPlayer().getTokenSummary().find(color)->second);
            for (unsigned int i = 0; i < tot; i++) {
                controller->getGame().getGameTable().getBag().addToken(controller->getcurrentPlayer().removeToken(color));
                nb--;
            }
        }
    }
    status = "end";
    handleGameStatus();
}

void QTGame::bookCard(Pyramid_Cards& pyramid, GameTable& gametable) {
    std::cout << "Voulez vous reserver une carte d'une des pioches (1) ou une carte de la pyramide (2) ?" << std::endl;
    unsigned int choice = choiceMaker(1, 2);
    if (choice == 1) {
        std::cout << "Veuillez choisir une pioche" << std::endl;
        std::cout << "1. Pioche niveau 1" << std::endl;
        std::cout << "2. Pioche niveau 2" << std::endl;
        std::cout << "3. Pioche niveau 3" << std::endl;
        unsigned int choice2 = choiceMaker(1, 3);
        JewelryCard& card = takeCard(choice2);
        controller->getcurrentPlayer().reserveOneCard(card);
    }
    else {
        std::cout << gametable.getPyramid() << std::endl;
        unsigned int level = choiceMaker(1, 3);
        unsigned int nb = pyramid.getLevelCards(level).size();
        unsigned int nbCard = choiceMaker(1, nb);
        controller->getcurrentPlayer().reserveOneCard(pyramid.takeCard(level,nbCard-1));
        pyramid.drawCard(level);
    }
    plateView->updateStatus("gold");
    plateView->updateMaxNbSelectedTokens(1);
}

/*
void QTGame::applyCardSkills(Game&game, Player&cardOwner, Player&opponent, JewelryCard&card) {
    if (card.getAbility1() == Abilities::take_privilege) {
        if (controller->getGame().getGameTable().getBoard().getNbPrivileges()!=0) {
            cardOwner.addPrivilege(controller->getGame().getGameTable().getBoard().takePrivilege());
        }
        else if (controller->getopposingPlayer().getNbPrivilege()!=0) {
            cardOwner.addPrivilege(controller->getopposingPlayer().removePrivilege());
        }
    }
    else if (card.getAbility1() == Abilities::take_bonus_token) {
        chooseToken(game.getGameTable().getBoard(), cardOwner);
    }
    else if (card.getAbility1() == Abilities::steal_token) {
        if (opponent.getNbTokens()!=0) {
            std::cout << "Voici les jetons du joueur advrese : \n";
            std::cout << opponent << std::endl;
            std::cout << "Veuillez choisir une couleur de jeton  (NOIR, ROUGE, BLEU, VERT, BLANC, PERLE)" << std::endl;
            std::string rep;
            std::cin >> rep;
            if (rep != "NOIR" && rep != "ROUGE" && rep != "BLEU" && rep != "VERT" && rep != "BLANC" && rep != "PERLE") {
                throw TokenException("Couleur invalide");
            }
            TokenColor color = toTokenColor(rep);
            if (controller->getopposingPlayer().getTokenSummary().find(color)->second==0){
                std::cout << "Le joueur adverse n'a pas de jeton de cette couleur" << std::endl;
            }
            else {
                const Token& token = controller->getopposingPlayer().removeToken(color);
                cardOwner.addToken(token);
            }
        }
        else {
            std::cout << "Le joueur adverse n'a pas de jetons" << std::endl;
        }
    }
    else if (card.getAbility1() == Abilities::cameleon) {
        std::cout << "Veuillez choisir une couleur de jeton  (NOIR, ROUGE, BLEU, VERT, BLANC)" << std::endl;
        std::string rep;
        std::cin >> rep;
        if (rep != "NOIR" && rep != "ROUGE" && rep != "BLEU" && rep != "VERT" && rep != "BLANC") {
            throw TokenException("Couleur invalide");
        }
        TokenColor color = toTokenColor(rep);

        if (cardOwner.getColorSummary(color).getBonusNumber() == 0 ){
            throw TokenException("Vous n'avez pas de cartes de cette couleur");
        }
        else {
            cardOwner.getColorSummary(color).addBonusNumber(1);
            cardOwner.addPrestige(1,color);
        }
    }
    else if (card.getAbility1() == Abilities::repeat_turn) {
        status = "start";
        play();
    }
    if (card.getAbility2() == Abilities::take_privilege) {
        if (controller->getGame().getGameTable().getBoard().getNbPrivileges()!=0) {
            cardOwner.addPrivilege(controller->getGame().getGameTable().getBoard().takePrivilege());
        }
        else if (controller->getopposingPlayer().getNbPrivilege()!=0) {
            cardOwner.addPrivilege(controller->getopposingPlayer().removePrivilege());
        }
    }
    else if (card.getAbility2() == Abilities::take_bonus_token) {
        chooseToken(game.getGameTable().getBoard(), cardOwner);
    }
    else if (card.getAbility2() == Abilities::steal_token) {
        if (opponent.getNbTokens()!=0) {
            std::cout << "Voici les jetons du joueur advrese : \n";
            std::cout << opponent << std::endl;
            std::cout << "Veuillez choisir une couleur de jeton  (NOIR, ROUGE, BLEU, VERT, BLANC, PERLE)" << std::endl;
            std::string rep;
            std::cin >> rep;
            if (rep != "NOIR" && rep != "ROUGE" && rep != "BLEU" && rep != "VERT" && rep != "BLANC" && rep != "PERLE") {
                throw TokenException("Couleur invalide");
            }
            TokenColor color = toTokenColor(rep);
            if (controller->getopposingPlayer().getTokenSummary().find(color)->second==0){
                std::cout << "Le joueur adverse n'a pas de jeton de cette couleur" << std::endl;
            }
            else {
                const Token& token = controller->getopposingPlayer().removeToken(color);
                cardOwner.addToken(token);
            }
        }
        else {
            std::cout << "Le joueur adverse n'a pas de jetons" << std::endl;
        }
    }
    else if (card.getAbility2() == Abilities::cameleon) {
        std::cout << "Veuillez choisir une couleur de jeton  (NOIR, ROUGE, BLEU, VERT, BLANC)" << std::endl;
        std::string rep;
        std::cin >> rep;
        if (rep != "NOIR" && rep != "ROUGE" && rep != "BLEU" && rep != "VERT" && rep != "BLANC") {
            throw TokenException("Couleur invalide");
        }
        TokenColor color = toTokenColor(rep);

        if (cardOwner.getColorSummary(color).getBonusNumber() == 0 ){
            throw TokenException("Vous n'avez pas de cartes de cette couleur");
        }
        else {
            cardOwner.getColorSummary(color).addBonusNumber(1);
            cardOwner.addPrestige(1,color);
        }
    }
    else if (card.getAbility2() == Abilities::repeat_turn) {
        status = "start";
        play();
    }
}
*/