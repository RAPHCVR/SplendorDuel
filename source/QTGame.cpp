//
// Created by utcpret on 13/12/2023.
//

#include "QTGame.h"
#include "QTQuestion.h"
#include <string>

QTGame::QTGame(QWidget* parent) : QWidget(parent) {
    QTStartingMenu* startingmenu = new QTStartingMenu(nullptr);
    startingmenu->exec();
    controller = new Controller("New", startingmenu->getPlayerName1().toStdString(), startingmenu->getPlayerName2().toStdString(), startingmenu->getPlayerType1(), startingmenu->getPlayerType2());
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
    boardRoyal->setMaximumWidth(400);
    pioches->setMaximumWidth(200);
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
    setFixedSize(size->width()*2,size->height()*1.75);
    setLayout(mainlayout);
    connect(plateView, &PlateView::tokensValidated, this, &QTGame::handleTokenSelection);
    connect(plateView, &PlateView::privilegeUsed, this, &QTGame::placePrivilege);
    connect(plateView, &PlateView::endOfTurn, this, &QTGame::handleGameStatus);

    connect(pyramid, &QTPyramid::acheterCarteClicked, this, &QTGame::handleBuyingJewelryCard);
    connect(pyramid, &QTPyramid::reserverCarteClicked, this, &QTGame::handleBookingJewelryCardFromPyramid);
    connect(pioches, &QTRangeePioches::reserverCarteClicked, this, &QTGame::handleBookingJewelryCardFromPioche);
    connect(boardRoyal, &QTBoardRoyal::acheterCarteClicked, this, &QTGame::handleBuyingRoyalCard);
    connect(player1, &PlayerQT::reserveCardSelected, this, &QTGame::handleBuyingReserveCard);
    connect(player2, &PlayerQT::reserveCardSelected, this, &QTGame::handleBuyingReserveCard);
    status = "start";
    handleGameStatus();
}
/*
QTStartingMenu::QTStartingMenu(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    //Nouvelle partie
    QPushButton *newGameButton = new QPushButton("Nouvelle partie", this);
    connect(newGameButton, &QPushButton::clicked, this, &QTStartingMenu::startNewGame);
    layout->addWidget(newGameButton);

    //Charger une partie sauvegardée
    QPushButton *loadGameButton = new QPushButton("Charger une partie sauvegardée", this);
        connect(loadGameButton, &QPushButton::clicked, this, &QTStartingMenu::loadGame);
    layout->addWidget(loadGameButton);

    //Quitter le jeu
    QPushButton *quitButton = new QPushButton("Quitter le jeu", this);
    connect(quitButton, &QPushButton::clicked, this, &QTStartingMenu::quitGame);

    setFixedSize(400, 200);
}
*/
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
    unsigned int nbt = Board::getInstance()->getNbTokens();
    unsigned int nb = std::min(nbp,nbt);
    std::string s ="Vous pouvez utiliser " + std::to_string(nb) + " privileges";
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
    player1->updateAllPlayer();
    player2->updateAllPlayer();
    if (status == "win"){
        //create an MBox asking if the players want to replay ir to quit
        std::vector<QString> buttonLabels = {"Rejouer","Quitter"};
        QString choice = MBox(buttonLabels, "Choix", "Voulez vous rejouer ou quitter ?");
        if (choice == "Rejouer") {
            std::string pseudo1 = controller->getcurrentPlayer().getName();
            std::string pseudo2 = controller->getopposingPlayer().getName();
            controller -> reinit();
            controller = new Controller("New", pseudo1, pseudo2, Type::Humain, Type::Humain);
            generateNewGame();
        }
        else {
            std::exit(0);
        }
    }
    else if (controller->checkIfPlayerWins(controller->getGame(),controller->getcurrentPlayer())) {
        status = "win";
        showVictoryDialog(QString::fromStdString(controller->getcurrentPlayer().getName()), this);
    }
    else {
        if (status == "start") {
            setBoldCurrentPlayer();
            play();
        }
        else if (status == "optionalActions"){
            setBoldCurrentPlayer();
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
    std::string s = "C'est au tour de " + controller->getcurrentPlayer().getName();
    MBox({"OK"},"Message", QString::fromStdString(s));
    std::vector<CompulsoryActions> compulsoryActions = controller->getCompulsoryActions(controller->getGame(), controller->getcurrentPlayer());
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
    applyCompulsoryAction(compulsoryActions[MBox(compulsoryActions, "Choix des actions obligatoires", QString::fromStdString(s))]);
}

void QTGame::applyCompulsoryAction(CompulsoryActions action) {
    std::string s;
    switch (action) {
        case CompulsoryActions::TakeCoins:
            plateView->updateStatus("take3tokens");
            plateView->updateMaxNbSelectedTokens(3);
            status = "check";
            break;
        case CompulsoryActions::ReserveCard:
            bookCard(controller->getGame().getGameTable());
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
        int nb = - (10 - controller->getcurrentPlayer().getNbTokens());
        QString s=  "Vous devez retirer " + QString::number(nb) + " jetons pour n'en conserver que 10";
        MBox({"OK"},"Message", s);
        while (nb != 0) {
            std::vector<QString> colors;
            //add all the clor that are not null in the player
            for (auto color : controller->getcurrentPlayer().getTokenSummary()) {
                if (color.second != 0) {
                    colors.push_back(QString::fromStdString(toString(color.first)));
                }
            }
            std::string rep = "Veuillez choisir une couleur de jeton";
            QString choice = MBox(colors,"Choix", QString::fromStdString(rep));
            TokenColor color = toTokenColor(choice.toStdString());
            s ="Choisissez combien vous voulez en retirer ";
            unsigned int tot = getNumberBetween(1,std::min(controller->getcurrentPlayer().getTokenSummary().find(color)->second,nb),s);
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

void QTGame::bookCard(GameTable& gametable) {
    pyramid->updateAllCardStatus(Carte::reservable);
    pioches->updateAllPiocheStatus(QTPioche::reservable);

    QMessageBox msgBox;
    msgBox.setText("Sélectionnez la carte que vous voulez réserver");
    msgBox.addButton("Ok", QMessageBox::RejectRole);
    msgBox.exec();
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
            QString s;
            if (opponent.getNbTokens()!=0) {
                s = "Veuillez choisir une couleur de jeton à retirer du joueur adverse :";
                std::vector<QString> colors = {"NOIR", "ROUGE", "BLEU", "VERT", "BLANC", "PERLE"};
                QString rep = MBox(colors,"Choix", s);
                TokenColor color = toTokenColor(rep.toStdString());
                if (controller->getopposingPlayer().getTokenSummary().find(color)->second==0){
                    std::cout << "Le joueur adverse n'a pas de jeton de cette couleur" << std::endl;
                }
                else {
                    const Token& token = controller->getopposingPlayer().removeToken(color);
                    cardOwner.addToken(token);
                }
            }
            else {
                s = "Le joueur adverse n'a pas de jetons";
                MBox({"OK"},"Message", s);
            }
            status = "check";
            handleGameStatus();
        }
        else if (card.getAbility1() == Abilities::cameleon) {
            QString s = "Veuillez choisir une couleur de bonus";
            std::vector<QString> colors = {"NOIR", "ROUGE", "BLEU", "VERT", "BLANC"};
            std::string rep = MBox(colors,"Choix", s).toStdString();
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
            QString s ="Vous rejouez";
            MBox({"OK"},"Message", s);
            handleGameStatus();
        }
    }
    else {
        QString s = "Veuillez choisir une couleur de bonus";
        std::vector<QString> colors = {"NOIR", "ROUGE", "BLEU", "VERT", "BLANC"};
        std::string rep = MBox(colors,"Choix", s).toStdString();
        TokenColor color = toTokenColor(rep);

        if (cardOwner.getColorSummary(color).getBonusNumber() == 0 ){
            throw TokenException("Vous n'avez pas de cartes de cette couleur");
        }
        else {
            cardOwner.getColorSummary(color).addBonusNumber(1);
            cardOwner.addPrestige(1,color);
        }
        status = "start";
        s = "Vous rejouez";
        MBox({"OK"},"Message", s);
        handleGameStatus();
    }
}

void QTGame::buyJewelryCard(GameTable& gametable) {
    bool bought = false;
    std::cout << "Jetons disponibles : " << std::endl;
    std::cout << controller->getcurrentPlayer() << std::endl;
    if (controller->getcurrentPlayer().canbuyreservedcard()) {
        QString s = "Voulez vous acheter une carte reservee ?";
        std::vector<QString> buttonLabels = {"Oui","Non"};
        QString choice = MBox(buttonLabels, "Choix", s);
        if (choice == "Oui") {

            PlayerQT* currentPlayerQT;
            if (player1->getPlayer() == controller->getcurrentPlayer()){
                currentPlayerQT=player1;
            }
            else{
                currentPlayerQT=player2;
            }

            QMessageBox msgBox;
            msgBox.setText("Choisissez la carte que vous voulez acheter");

            msgBox.addButton("Ok", QMessageBox::RejectRole);
            msgBox.exec();
            std::cout << "test8" << std::endl;
            currentPlayerQT->showPopup(true);
            bought = true;
        }
    }

    if (not(bought)) {
        pyramid->updateAllCardStatus(Carte::buyable);

        QMessageBox msgBox;
        msgBox.setText("Sélectionnez la carte que vous voulez acheter");
        msgBox.addButton("Ok", QMessageBox::RejectRole);
        msgBox.exec();
    }
}



void QTGame::buyNobleCard() {
    boardRoyal->updateAllCardStatus(QTCardRoyal::buyable);
    std::cout << "Veuillez choisir une carte" << std::endl;
    for (auto card : Deck_Royal::getInstance()->getCards()) {
        std::cout << *card << std::endl;
    }
    QMessageBox msgBox;
    msgBox.setText("Sélectionnez la carte Royale que vous voulez acheter");
    msgBox.addButton("Ok", QMessageBox::RejectRole);
    msgBox.exec();

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
        QString s;
        if (opponent.getNbTokens()!=0) {
            s = "Veuillez choisir une couleur de jeton à retirer du joueur adverse :";
            std::vector<QString> colors = {"NOIR", "ROUGE", "BLEU", "VERT", "BLANC", "PERLE"};
            QString rep = MBox(colors,"Choix", s);
            TokenColor color = toTokenColor(rep.toStdString());
            if (controller->getopposingPlayer().getTokenSummary().find(color)->second==0){
                std::cout << "Le joueur adverse n'a pas de jeton de cette couleur" << std::endl;
            }
            else {
                const Token& token = controller->getopposingPlayer().removeToken(color);
                cardOwner.addToken(token);
            }
        }
        else {
            s = "Le joueur adverse n'a pas de jetons";
            MBox({"OK"},"Message", s);
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

void QTGame::handleBuyingJewelryCard(Carte* cardclicked) {
    int level = cardclicked->getJewelryCard()->getLevel();
    int row, col, rowspan, columnspan;
    pyramid->getgrid()->getItemPosition(pyramid->getgrid()->indexOf(cardclicked), &row, &col, &rowspan, &columnspan);
    JewelryCard &card = controller->getGame().getGameTable().getPyramid().takeCard(level, col);
    controller->getGame().getGameTable().getPyramid().drawCard(level);
    if (controller->getcurrentPlayer().canBuyCard(card)) {
        controller->getcurrentPlayer().actionBuyCard(card);
        if (card.getAbility1()!= Abilities::None || card.getAbility2()!= Abilities::None) {
            pyramid->updateAllCardStatus(Carte::notClickable);
            applyCardSkills(controller->getGame(),controller->getcurrentPlayer(), controller->getopposingPlayer(),card);
        }
        else {
            pyramid->updateAllCardStatus(Carte::notClickable);
            status = "check";
            handleGameStatus();
        }
    }
    else {
        showWarningMessage("Achat impossible", "Vous n'avez pas assez de jetons pour acheter cette carte");
    }
}

void QTGame::handleBookingJewelryCardFromPyramid(Carte* clickedCard) {

    //On connecte le back-end
    if (clickedCard) {
        int row, col, rowspan, columnspan;
        pyramid->getgrid()->getItemPosition(pyramid->getgrid()->indexOf(clickedCard), &row, &col, &rowspan, &columnspan);
        unsigned int level = abs(row - 3);
        controller->getcurrentPlayer().reserveOneCard(pyramid->getPyramidCard()->takeCard(level , col));
        pyramid->getPyramidCard()->drawCard(level);
    }

    //Partie qui gère l'update graphique se trouve dans QTPyramid::carteClicked
    pyramid->updateAllCardStatus(Carte::notClickable);
    pioches->updateAllPiocheStatus(QTPioche::notClickable);
    plateView->updateStatus("gold");
    plateView->updateMaxNbSelectedTokens(1);

    MBox({"OK"},"Message", "Veuillez choisir un jeton or");
    player1->updateAllPlayer();
    player2->updateAllPlayer();

}

void QTGame::handleBookingJewelryCardFromPioche(QTPioche* piocheclicked) {


    if (piocheclicked) {
        int row, col, rowspan, columnspan;
        pioches->getGrid()->getItemPosition(pioches->getGrid()->indexOf(piocheclicked), &row, &col, &rowspan, &columnspan);
        int level = abs(row - 3); // Convertir la ligne en level
        JewelryCard& card = takeCard(level);
        controller->getcurrentPlayer().reserveOneCard(card);
    }
    pioches->updateAllPiocheStatus(QTPioche::notClickable);
    pyramid->updateAllCardStatus(Carte::notClickable);
    plateView->updateStatus("gold");
    plateView->updateMaxNbSelectedTokens(1);

    MBox({"OK"},"Message", "Veuillez choisir un jeton or");
    player1->updateAllPlayer();
    player2->updateAllPlayer();
}

void QTGame::handleBuyingRoyalCard(QTCardRoyal* cardclicked){

    int row, col, rowspan, columnspan;
    boardRoyal->getGrid()->getItemPosition(boardRoyal->getGrid()->indexOf(cardclicked), &row, &col, &rowspan, &columnspan);
    int position = row + col;
    controller->getcurrentPlayer().addRoyalCard(*(cardclicked->getRoyalCard()), position);
    if (cardclicked->getRoyalCard()->getAbility()!= Abilities::None) {
        boardRoyal->updateAllCardStatus(QTCardRoyal::notClickable);
        applyRoyalCardSkills(controller->getGame(),controller->getcurrentPlayer(),controller->getopposingPlayer(),*(cardclicked->getRoyalCard()));
    }
    else {
        boardRoyal->updateAllCardStatus(QTCardRoyal::notClickable);
        status = "check";
        handleGameStatus();
    }
}

void showWarningMessage(const QString &title, const QString &content) {
    QMessageBox::warning(nullptr, title, content);
}

QString MBox(const std::vector<QString>& buttonLabels, const QString& title, const QString& content) {
    CustomDialog dialog(nullptr, CustomDialog::EmitMode::Label); // Set to Label emit mode
    dialog.setTitle(title);
    dialog.setContent(content);

    for (const auto& label : buttonLabels) {
        dialog.addButton(label, 0); // Assume role is 0, modify if needed
    }

    QString clickedButtonText;
    QObject::connect(&dialog, &CustomDialog::buttonClickedLabel, [&clickedButtonText](const QString &buttonText) {
        clickedButtonText = buttonText;
    });

    dialog.setWindowModality(Qt::NonModal);
    dialog.show();
    dialog.exec();

    return clickedButtonText;
}

int MBox(const std::vector<OptionalActions>& buttonLabels, const QString& title, const QString& content) {
    CustomDialog dialog(nullptr, CustomDialog::EmitMode::Index); // Set to Index emit mode
    dialog.setTitle(title);
    dialog.setContent(content);

    for (const auto& label : buttonLabels) {
        switch (label) {
            case OptionalActions::Empty:
                dialog.addButton("Ne rien faire",0);
            break;
            case OptionalActions::UsePrivileges:
                dialog.addButton("Utiliser des privilèges",1);
            break;
            case OptionalActions::FillBoard:
                dialog.addButton("Remplir le plateau",2);
            break;
        }
    }

    int result = -1;
    QObject::connect(&dialog, &CustomDialog::buttonClicked, [&result](int index) {
        result = index;
    });

    dialog.setWindowModality(Qt::NonModal);
    dialog.show();
    dialog.exec();

    return result;
}

int MBox(const std::vector<CompulsoryActions>& buttonLabels, const QString& title, const QString& content) {
    CustomDialog dialog(nullptr, CustomDialog::EmitMode::Index); // Set to Index emit mode
    dialog.setTitle(title);
    dialog.setContent(content);

    for (const auto& label : buttonLabels) {
        switch (label) {
            case CompulsoryActions::TakeCoins:
                dialog.addButton("Prendre des jetons",0);
            break;
            case CompulsoryActions::ReserveCard:
                dialog.addButton("Reserver une carte",1);
            break;
            case CompulsoryActions::BuyCard:
                dialog.addButton("Acheter une carte",2);
            break;
        }
    }

    int result = -1;
    QObject::connect(&dialog, &CustomDialog::buttonClicked, [&result](int index) {
        result = index;
    });

    dialog.setWindowModality(Qt::NonModal);
    dialog.show();
    dialog.exec();

    return result;
}

void showVictoryDialog(const QString &playerName, QTGame *gameInstance) {
    QDialog victoryDialog;
    victoryDialog.setWindowTitle("Félicitations !");

    // Create the congratulatory message
    QLabel *messageLabel = new QLabel(&victoryDialog);
    messageLabel->setText(QString("Bravo %1, vous avez gagné !").arg(playerName));
    QFont font = messageLabel->font();
    font.setPointSize(16);
    font.setBold(true);
    messageLabel->setFont(font);

    // Add a close button
    QPushButton *closeButton = new QPushButton("Fermer", &victoryDialog);
    QObject::connect(closeButton, &QPushButton::clicked, &victoryDialog, &QDialog::accept);

    // Connect the finished signal to the handleGameStatus method of QTGame instance
    if (gameInstance) {
        QObject::connect(&victoryDialog, &QDialog::finished, gameInstance, [gameInstance]() { gameInstance->handleGameStatus(); });
    }

    // Layout the elements in the dialog
    QVBoxLayout *layout = new QVBoxLayout(&victoryDialog);
    layout->addWidget(messageLabel, 0, Qt::AlignCenter);
    layout->addWidget(closeButton, 0, Qt::AlignCenter);

    victoryDialog.exec();
}

int getNumberBetween(int x, int y, const QString &message, QWidget *parent) {
    QInputDialog dialog(parent);
    dialog.setInputMode(QInputDialog::IntInput);
    dialog.setIntRange(x, y);
    dialog.setWindowTitle("Enter Number");

    // Use the custom message
    dialog.setLabelText(message);

    int result = x; // Default value
    if (dialog.exec() == QDialog::Accepted) {
        result = dialog.intValue();
    }

    return result;
}

void QTGame::setBoldCurrentPlayer(){
    if (player1->getPlayer() == controller->getcurrentPlayer()){
        player1->toggleTextBoldJoueur(true);
        player2->toggleTextBoldJoueur(false);
    }
    else{
        player2->toggleTextBoldJoueur(true);
        player1->toggleTextBoldJoueur(false);
    }
}

void QTGame::handleBuyingReserveCard(JewelryCard* cardClicked){
    std::cout << "ADAAAA" << std::endl;
    player1->updateAllReserveStatus(reserveQT::ReserveStatus::notClickable);
    player2->updateAllReserveStatus(reserveQT::ReserveStatus::notClickable);
    if (controller->getcurrentPlayer().canBuyCard(*cardClicked)) {
        controller->getcurrentPlayer().actionBuyReservedCard(*cardClicked);
    }

    status = "check";
    handleGameStatus();
}

void clearLayout(QLayout* layout) {
    if (!layout) return;

    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        // If the item is a widget, delete it
        if (QWidget* widget = item->widget()) {
            delete widget;
        }
        // If the item is another layout, recursively clear and delete it
        else if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout);
            delete childLayout;
        }
        // For other items (like spacers), just delete the item
        else {
            delete item;
        }
    }
}


void clearWidgetAndSetNewLayout(QWidget* parentWidget, QLayout* newLayout) {
    if (!parentWidget) return;

    // Clear the existing layout
    if (QLayout* oldLayout = parentWidget->layout()) {
        clearLayout(oldLayout);
        delete oldLayout;
    }

    // Set the new layout
    parentWidget->setLayout(newLayout);
}

void QTGame::generateNewGame() {
    QVBoxLayout* mainlayout2 = new QVBoxLayout(this);
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
    boardRoyal->setMaximumWidth(400);
    pioches->setMaximumWidth(200);
    CartesRoyalesPrivilegesPlateau->addWidget(privilegeCounter);
    CartesRoyalesPrivilegesPlateau->addWidget(plateView);
    centre->addLayout(PyramidPioche);
    centre->addLayout(CartesRoyalesPrivilegesPlateau);
    total -> addWidget(player1);
    total -> addLayout(centre);
    total -> addWidget(player2);
    mainlayout2->addLayout(total);
    clearWidgetAndSetNewLayout(this,mainlayout2);
    int h = plateView->size().height() + pyramid->size().height()/2 + boardRoyal->size().height()/2;
    setFixedSize(size->width()*2,size->height()*1.75);
    connect(plateView, &PlateView::tokensValidated, this, &QTGame::handleTokenSelection);
    connect(plateView, &PlateView::privilegeUsed, this, &QTGame::placePrivilege);
    connect(plateView, &PlateView::endOfTurn, this, &QTGame::handleGameStatus);

    connect(pyramid, &QTPyramid::acheterCarteClicked, this, &QTGame::handleBuyingJewelryCard);
    connect(pyramid, &QTPyramid::reserverCarteClicked, this, &QTGame::handleBookingJewelryCardFromPyramid);
    connect(pioches, &QTRangeePioches::reserverCarteClicked, this, &QTGame::handleBookingJewelryCardFromPioche);
    connect(boardRoyal, &QTBoardRoyal::acheterCarteClicked, this, &QTGame::handleBuyingRoyalCard);
    status = "start";
    handleGameStatus();
}