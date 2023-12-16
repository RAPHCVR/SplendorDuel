//
// Created by utcpret on 13/12/2023.
//

#include "QTGame.h"

#include <string>

QTGame::QTGame(QWidget* parent) : QWidget(parent) {
    controller = new Controller();
    screen = QGuiApplication::primaryScreen();
    size = new QSize(screen->size()/2);
    width = size->width();
    height = size->height();
    mainlayout = new QVBoxLayout(this);
    QHBoxLayout* PyramidPioche = new QHBoxLayout();
    QVBoxLayout* total = new QVBoxLayout();
    QHBoxLayout* CartesRoyalesPrivilegesPlateau= new QHBoxLayout();
    plateView = new PlateView(nullptr, height-100,width/2);
    pyramid = new QTPyramid();
    pioches = new QTRangeePioches(nullptr);
    pioches->setFixedWidth(100);
    boardRoyal = new QTBoardRoyal(nullptr);
    boardRoyal->setFixedWidth(300);
    privilegeCounter = new PrivilegeCounter(nullptr);
    privilegeCounter->setFixedWidth(35);
    PyramidPioche->addWidget(pioches);
    PyramidPioche->addWidget(pyramid);
    CartesRoyalesPrivilegesPlateau->addWidget(boardRoyal);
    CartesRoyalesPrivilegesPlateau->addWidget(privilegeCounter);
    CartesRoyalesPrivilegesPlateau->addWidget(plateView);
    total->addLayout(PyramidPioche);
    total->addLayout(CartesRoyalesPrivilegesPlateau);
    mainlayout->addLayout(total);
    //mainlayout->addLayout(first);
    //mainlayout->addLayout(second);
    int h = plateView->size().height() + pyramid->size().height()/2 + boardRoyal->size().height()/2;
    setFixedSize(size->width(),size->width());
    setLayout(mainlayout);
    connect(plateView, &PlateView::tokensValidated, this, &QTGame::handleTokenSelection);
    connect(plateView, &PlateView::privilegeUsed, this, &QTGame::placePrivilege);
    connect(plateView, &PlateView::endOfTurn, this, &QTGame::handleGameStatus);
    status = "start";
    handleGameStatus();
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
        privilegeCounter->updateValue();
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
        privilegeCounter->updateValue();
    }
    catch (TokenException& err) {
        std::cout << err.getMessage() << "\n";
    }
}
void QTGame::usePriviledge() {
    unsigned int nbp = controller->getcurrentPlayer().getNbPrivilege();
    unsigned int nbt = Board::getInstance().getNbTokens();
    unsigned int nb = std::min(nbp,nbt);
    std::cout << "Vous pouvez en utiliser " << nb << " privileges" << std::endl;
    plateView->updateMaxNbSelectedTokens(nb);
    plateView->updateStatus("privileges");
}

void QTGame::applyOptionalAction(OptionalActions action) {
    switch (action) {
        case OptionalActions::UsePrivileges:
            usePriviledge();
            status = "optionalActions";
        break;
        case OptionalActions::FillBoard:
            fillBoard();
            if (controller->getGame().getGameTable().getBoard().getNbPrivileges()>0) {
                takePrivilege(controller->getopposingPlayer());
            }
            else if (controller->getcurrentPlayer().getNbPrivilege()>0) {
                controller->getopposingPlayer().addPrivilege(controller->getcurrentPlayer().removePrivilege());
            }
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
    if (controller->checkIfPlayerWins(controller->getGame(),controller->getcurrentPlayer())) {
        status = "win";
        std::cout << "Le joueur " << controller->getcurrentPlayer().getName() << " a gagné" << std::endl;
    }
    else {
        if (status == "start") {
            play();
        }
        else if (status == "optionalActions"){
            playOptionalActions();
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
}

void QTGame::play() {
    std::cout << "C'est au tour de " << controller->getcurrentPlayer().getName() << std::endl;
    std::cout << controller->getcurrentPlayer() << std::endl;
    std::vector<CompulsoryActions> compulsoryActions = controller->getCompulsoryActions(controller->getGame(), controller->getcurrentPlayer());
    std::cout << "Cartes de la pyramide : " << std::endl;
    std::cout << controller->getGame().getGameTable().getPyramid() << std::endl;
    if (compulsoryActions.empty()) {
        std::cout << "Pas d'action obligatoires possibles, remplissage du plateau" << std::endl;
        applyOptionalAction(OptionalActions::FillBoard);
    }
    else {
        status = "optionalActions";
        handleGameStatus();
    }
}

void QTGame::playOptionalActions(){
    std::vector<OptionalActions> optionalActions = controller->getOptionalActions(controller->getGame(), controller->getcurrentPlayer());
    if (optionalActions.size()>1) {
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

        QString text = QInputDialog::getText(nullptr, "Choix des actions optionnelles",
                            "Entrez une valeur entre 1 et " + QString::number(optionalActions.size()), QLineEdit::Normal,
                            QDir::home().dirName());
        applyOptionalAction(optionalActions[text.toInt()-1]);
    }
    else {
        status = "compulsoryActions";
        handleGameStatus();
    }
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
    QString text = QInputDialog::getText(nullptr, "Choix des actions obligaroires",
                            "Entrez une valeur entre 1 et " + QString::number(compulsoryActions.size()), QLineEdit::Normal,
                            QDir::home().dirName());
    applyCompulsoryAction(compulsoryActions[text.toInt()-1]);
}

void QTGame::applyCompulsoryAction(CompulsoryActions action) {
    switch (action) {
        case CompulsoryActions::TakeCoins:
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
            buyJewelryCard(controller->getGame().getGameTable());
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
    if(controller->getcurrentPlayer().getCrowns() >=3 && controller->getcurrentPlayer().getRoyalCards().empty() || controller->getcurrentPlayer().getCrowns() >= 6 && controller->getcurrentPlayer().getRoyalCards().size()==1){
        buyNobleCard();
    }
    else {
        status = "end";
        handleGameStatus();
    }
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


void QTGame::applyCardSkills(Game&game, Player&cardOwner, Player&opponent, JewelryCard&card) {
    if (card.getAbility2()==Abilities::None) {
        if (card.getAbility1() == Abilities::take_privilege) {
            status = "check";
            if (controller->getGame().getGameTable().getBoard().getNbPrivileges()!=0) {
                takePrivilege(cardOwner);
            }
            else if (controller->getopposingPlayer().getNbPrivilege()!=0) {
                cardOwner.addPrivilege(controller->getopposingPlayer().removePrivilege());
            }
            handleGameStatus();
        }
        else if (card.getAbility1() == Abilities::take_bonus_token) {
            status = "check";
            plateView->updateStatus("take3tokens");
            plateView->updateMaxNbSelectedTokens(1);
            std::cout << "Veuillez choisir un jeton bonus sur le plateau" << std::endl;
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
            status = "check";
            handleGameStatus();
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
            status = "check";
            handleGameStatus();
        }
        else if (card.getAbility1() == Abilities::repeat_turn) {
            status = "start";
            std::cout << "Vous rejouez" << std::endl;
            handleGameStatus();
        }
    }
    else {
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
        status = "start";
        std::cout << "Vous rejouez" << std::endl;
        handleGameStatus();
    }
}

void QTGame::buyJewelryCard(GameTable& gametable) {
    bool bought = false;
    JewelryCard* card;
    std::cout << "Jetons disponibles : " << std::endl;
    std::cout << controller->getcurrentPlayer() << std::endl;
    if (controller->getcurrentPlayer().getReserve().size() > 0) {
        std::cout << "Voulez vous acheter une carte reservee ?" << std::endl;
        std::cout << "1. Oui" << std::endl;
        std::cout << "2. Non" << std::endl;
        unsigned int choice = choiceMaker(1, 2);
        if (choice == 1) {
            bought = true;
            std::cout << "Veuillez choisir une carte" << std::endl;
            for (auto card : controller->getcurrentPlayer().getReserve()) {
                std::cout << *card << std::endl;
            }
            unsigned int nbCard = choiceMaker(1, controller->getcurrentPlayer().getReserve().size());
            card = controller->getcurrentPlayer().getReserve()[nbCard - 1];
            if (controller->getcurrentPlayer().canBuyCard(*card)) {
                controller->getcurrentPlayer().actionBuyReservedCard(*card);
            }
        }
    }
    if (not(bought)) {
        unsigned int level = choiceMaker(1, 3);
        unsigned int nb = gametable.getPyramid().getLevelCards(level).size();
        std::cout << "Cartes de niveau " << level << " : \n";
        for (auto card: gametable.getPyramid().getLevelCards(level)) {
            std::cout << *card << std::endl;
        }
        unsigned int nbCard = choiceMaker(1, nb);
        card = &gametable.getPyramid().takeCard(level, nbCard - 1);
        gametable.getPyramid().drawCard(level);
        if (controller->getcurrentPlayer().canBuyCard(*card)) {
            controller->getcurrentPlayer().actionBuyCard(*card);
        }
        else {
            throw JewelryCardError("Vous ne pouvez pas acheter cette carte");
        }
    }
    if (card->getAbility1()!= Abilities::None || card->getAbility2()!= Abilities::None) {
        applyCardSkills(controller->getGame(),controller->getcurrentPlayer(), controller->getopposingPlayer(),*card);
    }
    else {
        status = "check";
        handleGameStatus();
    }
}

void QTGame::buyNobleCard() {
    std::cout << "Veuillez choisir une carte" << std::endl;
    for (auto card : Deck_Royal::getInstance()->getCards()) {
        std::cout << *card << std::endl;
    }
    unsigned int nbCard = choiceMaker(1, Deck_Royal::getInstance()->getCards().size());
    RoyalCard& card = *Deck_Royal::getInstance()->getCards()[nbCard - 1];
    controller->getcurrentPlayer().addRoyalCard(card, nbCard - 1);
    if (card.getAbility()!= Abilities::None) {
        applyRoyalCardSkills(controller->getGame(),controller->getcurrentPlayer(),controller->getopposingPlayer(),card);
    }
    else {
        status = "check";
        handleGameStatus();
    }
}

void QTGame::applyRoyalCardSkills(Game&game, Player&cardOwner, Player&opponent, RoyalCard&card) {
    if (card.getAbility() == Abilities::take_privilege) {
        status = "check";
        if (controller->getGame().getGameTable().getBoard().getNbPrivileges()!=0) {
            takePrivilege(cardOwner);
        }
        else if (controller->getopposingPlayer().getNbPrivilege()!=0) {
            cardOwner.addPrivilege(controller->getopposingPlayer().removePrivilege());
        }
        handleGameStatus();
    }
    else if (card.getAbility() == Abilities::steal_token) {
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
        status = "check";
        handleGameStatus();
    }
    else if (card.getAbility() == Abilities::repeat_turn) {
        status = "start";
        std::cout << "Vous rejouez" << std::endl;
        handleGameStatus();
    }
}

