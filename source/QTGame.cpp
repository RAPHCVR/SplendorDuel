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
    QPushButton* privileges = new QPushButton("prendre un privilege");
    connect(privileges, &QPushButton::clicked, this, [this]() {takePrivilege(); applyOptionalAction(OptionalActions::UsePrivileges);});
    first->addWidget(plateView);
    second->addWidget(privileges);
    mainlayout->addLayout(first);
    mainlayout->addLayout(second);
    setLayout(mainlayout);
    connect(plateView, &PlateView::tokensValidated, this, &QTGame::handleTokenSelection);
    connect(plateView, &PlateView::privilegeUsed, this, &QTGame::placePrivilege);
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

void QTGame::takePrivilege() {
    try {
        controller->getcurrentPlayer().addPrivilege(controller->getGame().getGameTable().getBoard().takePrivilege());
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
}

void QTGame::applyOptionalAction(OptionalActions action) {
    switch (action) {
        case OptionalActions::UsePrivileges:
            usePriviledge();
        break;
        case OptionalActions::FillBoard:
            fillBoard();
        break;
        case OptionalActions::Empty:
            break;
    }
}

/*
void QTGame::play() {
    std::cout << "C'est au tour de " << controller->getcurrentPlayer().getName() << std::endl;
    std::cout << controller->getcurrentPlayer() << std::endl;
    std::vector<CompulsoryActions> compulsoryActions = controller->getCompulsoryActions(controller->getGame(), controller->getcurrentPlayer());
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
}
*/