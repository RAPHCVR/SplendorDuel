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
    QVBoxLayout* centre = new QVBoxLayout();
    QHBoxLayout* total = new QHBoxLayout();
    QHBoxLayout* CartesRoyalesPrivilegesPlateau= new QHBoxLayout();
    plateView = new PlateView(nullptr, height-100,width/2);

    pyramid = new QTPyramid();
    pioches = new QTRangeePioches(nullptr);
    boardRoyal = new QTBoardRoyal(nullptr);
    privilegeCounter = new PrivilegeCounter(nullptr);
    player1 = new PlayerQT(controller->getcurrentPlayer(), nullptr);
    player2 = new PlayerQT(controller->getopposingPlayer(), nullptr);
    PyramidPioche->addWidget(pioches);
    PyramidPioche->addWidget(pyramid);
    CartesRoyalesPrivilegesPlateau->addWidget(boardRoyal);
    boardRoyal->setMaximumWidth(500);
    CartesRoyalesPrivilegesPlateau->addWidget(privilegeCounter);
    CartesRoyalesPrivilegesPlateau->addWidget(plateView);
    centre->addLayout(PyramidPioche);
    centre->addLayout(CartesRoyalesPrivilegesPlateau);
    total -> addWidget(player1);
    total -> addLayout(centre);
    total -> addWidget(player2);
    mainlayout->addLayout(total);
    //mainlayout->addLayout(first);
    //mainlayout->addLayout(second);
    int h = plateView->size().height() + pyramid->size().height()/2 + boardRoyal->size().height()/2;
    setFixedSize(size->width()*2,size->height()*1.5);
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
    std::string s ="Vous pouvez en utiliser " + std::to_string(nb) + " privileges";
    MBox({"OK"},"Message", QString::fromStdString(s));
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
            player1->updateAllPlayer();
            player2->updateAllPlayer();
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
    std::string s = "C'est au tour de " + controller->getcurrentPlayer().getName();
    MBox({"OK"},"Message", QString::fromStdString(s));
    std::vector<CompulsoryActions> compulsoryActions = controller->getCompulsoryActions(controller->getGame(), controller->getcurrentPlayer());
    std::cout << "Cartes de la pyramide : " << std::endl;
    std::cout << controller->getGame().getGameTable().getPyramid() << std::endl;
    if (compulsoryActions.empty()) {
        s = "Pas d'action obligatoires possibles, remplissage du plateau";
        MBox({"OK"},"Message", QString::fromStdString(s));
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
        std::string msg = "Veuillez choisir une action optionnelle";
        /*
        std::vector<QString> buttonLabels;
        for (auto action : optionalActions) {
            switch (action) {
                case OptionalActions::Empty:
                    buttonLabels.push_back("Ne rien faire");
                break;
                case OptionalActions::UsePrivileges:
                    buttonLabels.push_back("Utiliser des privilèges");
                break;
                case OptionalActions::FillBoard:
                    buttonLabels.push_back("Remplir le plateau");
                break;
            }
        }

        QString text = QInputDialog::getText(nullptr, "Choix des actions optionnelles",
                            "Entrez une valeur entre 1 et " + QString::number(optionalActions.size()), QLineEdit::Normal,
                            QDir::home().dirName());
        */

        applyOptionalAction(optionalActions[MBox(optionalActions, "Choix des actions optionnelles", QString::fromStdString(msg))]);
    }
    else {
        status = "compulsoryActions";
        handleGameStatus();
    }
}

void QTGame::playCompulsoryActions(){
    std::vector<CompulsoryActions> compulsoryActions = controller->getCompulsoryActions(controller->getGame(), controller->getcurrentPlayer());
    std::string s = "Veuillez choisir une action obligatoire";
    /*
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
    */
    applyCompulsoryAction(compulsoryActions[MBox(compulsoryActions, "Choix des actions obligatoires", QString::fromStdString(s))]);
}

void QTGame::applyCompulsoryAction(CompulsoryActions action) {
    std::string s;
    switch (action) {
        case CompulsoryActions::TakeCoins:
            s = "Veuillez choisir des jetons";
            MBox({"OK"},"Message", QString::fromStdString(s));
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
        player1->updateAllPlayer();
        player2->updateAllPlayer();
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
        player1->updateAllPlayer();
        player2->updateAllPlayer();
    }
    else {
        player1->updateAllPlayer();
        player2->updateAllPlayer();
        status = "end";
        handleGameStatus();
    }
}

void QTGame::bookCard(Pyramid_Cards& pyramid, GameTable& gametable) {
    std::vector<QString> buttonLabels = {"Pioche","Pyramide"};
    QString choice = MBox(buttonLabels, "Choix", "Voulez vous reserver une carte d'une des pioches ou une carte de la pyramide ?");
    if (choice == "Pioche") {
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
    player1->updateAllPlayer();
    player2->updateAllPlayer();
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
            MBox({"OK"},"Message", "Veuillez choisir un jeton bonus sur le plateau");
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
        std::string s = "Vous rejouez";
        MBox({"OK"},"Message", QString::fromStdString(s));
        handleGameStatus();
    }
}

QString MBox(const std::vector<QString>& buttonLabels, const QString& title, const QString& content) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(content);

    // Ajout des boutons personnalisés
    std::vector<QPushButton*> buttons;
    for (const auto& label : buttonLabels) {
        buttons.push_back(msgBox.addButton(label, QMessageBox::NoRole));
    }

    // Affichage de la boîte de dialogue
    msgBox.exec();

    // Récupération du bouton cliqué
    QAbstractButton* clickedButton = msgBox.clickedButton();
    if (clickedButton) {
        // Retourne le texte du bouton cliqué
        return clickedButton->text();
    }

    // Retourne une chaîne vide si aucun bouton n'a été cliqué
    return "";
}

int MBox(const std::vector<OptionalActions>& buttonLabels , const QString& title, const QString& content) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(content);

    // Ajout des boutons personnalisés
    std::vector<QPushButton*> buttons;
    for (const auto& label : buttonLabels) {
        switch (label) {
            case OptionalActions::Empty:
                buttons.push_back(msgBox.addButton("Ne rien faire", QMessageBox::NoRole));
            break;
            case OptionalActions::UsePrivileges:
                buttons.push_back(msgBox.addButton("Utiliser des privilèges", QMessageBox::NoRole));
            break;
            case OptionalActions::FillBoard:
                buttons.push_back(msgBox.addButton("Remplir le plateau", QMessageBox::NoRole));
            break;
        }
    }

    // Affichage de la boîte de dialogue
    msgBox.exec();

    // Récupération du bouton cliqué
    QAbstractButton* clickedButton = msgBox.clickedButton();
    if (clickedButton) {
        // Retourne le texte du bouton cliqué
        return std::find(buttons.begin(), buttons.end(), clickedButton) - buttons.begin();
    }

    // Retourne une chaîne vide si aucun bouton n'a été cliqué
    return -1;
}

int MBox(const std::vector<CompulsoryActions>& buttonLabels , const QString& title, const QString& content) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(content);

    // Ajout des boutons personnalisés
    std::vector<QPushButton*> buttons;
    for (const auto& label : buttonLabels) {
        switch (label) {
            case CompulsoryActions::TakeCoins:
                buttons.push_back(msgBox.addButton("Prendre des jetons", QMessageBox::NoRole));
            break;
            case CompulsoryActions::ReserveCard:
                buttons.push_back(msgBox.addButton("Reserver une carte", QMessageBox::NoRole));
            break;
            case CompulsoryActions::BuyCard:
                buttons.push_back(msgBox.addButton("Acheter une carte", QMessageBox::NoRole));
            break;
        }
    }

    // Affichage de la boîte de dialogue
    msgBox.exec();

    // Récupération du bouton cliqué
    QAbstractButton* clickedButton = msgBox.clickedButton();
    if (clickedButton) {
        // Retourne le texte du bouton cliqué
        return std::find(buttons.begin(), buttons.end(), clickedButton) - buttons.begin();
    }

    // Retourne une chaîne vide si aucun bouton n'a été cliqué
    return -1;
}