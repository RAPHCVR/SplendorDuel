#include "QTJoueur.h"

std::string typeToString(Type type) {
    switch (type) {
        case Type::IA:
            return "IA";
        case Type::Humain:
            return "Humain";
        default:
            return "Unknown";
    }
}

//Methodes widget token resume

QColor convertToQColorFromTokenColor(TokenColor tokenColor) {
    switch (tokenColor) {
        case TokenColor::BLEU:
            return {0, 0, 255};  // Blue
        case TokenColor::BLANC:
            return {255, 255, 255};  // White
        case TokenColor::VERT:
            return {0, 255, 0};  // Green
        case TokenColor::NOIR:
            return {0, 0, 0};  // Black
        case TokenColor::ROUGE:
            return {255, 0, 0};  // Red
        case TokenColor::PERLE:
            // Replace with the desired color values for PERLE
            return {200, 0, 255};  // Example gray color
        case TokenColor::OR:
            return {255, 215, 0};  // Gold
        case TokenColor::None:
        default:
            return {};  // Default to an invalid color
    }
}

TokenWidget::TokenWidget(QWidget* parent, const TokenColor c) : QWidget(parent), color(c){
    setFixedSize(50, 50);
    QPalette palette;
    QColor randomColor = convertToQColorFromTokenColor(color);
    palette.setColor(QPalette::Base, randomColor);
    setAutoFillBackground(true);
    setPalette(palette);
    numToken = new QLCDNumber(this);
    numToken->setDigitCount(2);
    numToken->display(0);
}

void TokenWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().base());
    painter.drawEllipse(rect().adjusted(2, 2, -2, -2));
}

void TokenWidget::updateNumToken(int newNumToken){
    numToken->display(newNumToken);

}

//Methodes widget carte resume

void CardWidget::updateBonus(int newBonus){
    bonus->display(newBonus);
};

void CardWidget::updatePrestige(int newPrestige){
    prestige->display(newPrestige);
};

CardWidget::CardWidget(QWidget *parent, TokenColor color)
        : QWidget(parent), color(color) {

    // Set la couleur de fond
    QColor newColor=convertToQColorFromTokenColor(color);
    setStyleSheet(QString("background-color: %1;").arg(newColor.name()));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);

    bonus = new QLCDNumber(this);
    bonus->display(0);
    layout->addWidget(bonus);

    prestige = new QLCDNumber(this);
    prestige->display(0);
    layout->addWidget(prestige);
}

//Methodes Reserve

bool reserveQT::isReserveCardBuyable() const {
    return (status == ReserveStatus::buyable);
}

reserveQT::reserveQT(JewelryCard* card, QWidget* parent)
        : QWidget(parent), jewelryCard(card), status(ReserveStatus::notClickable) {

    // Create the Carte widget to display the JewelryCard
    carteWidget = new Carte(jewelryCard, this);

    // Create a selection button
    selectionButton = new QPushButton("Acheter", this);
    connect(selectionButton, &QPushButton::clicked, this, &reserveQT::onSelectionButtonClicked);

    // Create the main layout
    mainLayout = new QVBoxLayout(this);

    // Add Carte and selection button to the main layout
    mainLayout->addWidget(carteWidget);

    // Set the size constraint for the Carte widget
    carteWidget->setMaximumSize(150, 200);  // Adjust the size as needed
    carteWidget->setMinimumSize(150, 200);

    mainLayout->addWidget(selectionButton);

    // Set the main layout for the widget
    setLayout(mainLayout);
}

void reserveQT::onSelectionButtonClicked() {
    qDebug() << "Carte cliquée : Level : " << jewelryCard->getLevel()
             << " et id : " << jewelryCard->getId();
    if (status == ReserveStatus::buyable) {
        emit closePopup();
        emit acheterReserveClicked(jewelryCard);
    }
    else {
        QMessageBox::information(this, "Non achetable", "Il n'est pas possible d'acheter de carte à ce moment.", QMessageBox::Ok);
    }
}

//Methodes joueur global

PlayerQT::PlayerQT(Player &p, QWidget* parent) : QWidget(parent), player(p), reserveCardBought(false) {
    setWindowTitle("Player");
    popupButtonReserve = new QPushButton("Reserve", this);
    connect(popupButtonReserve, &QPushButton::clicked, this, &PlayerQT::showPopup);
    popupDialog = new QDialog(this);
    popupDialog->setWindowTitle("Reserve");

    popupLayout = new QHBoxLayout(popupDialog);

    popupDialog->setLayout(popupLayout);

    QString playerType = QString::fromStdString(typeToString(player.getType()));
    typeJoueur = new QLabel(playerType);
    cartes = new QLabel("Cartes : ");
    jetons = new QLabel("Jetons : ");
    couronne = new QLabel("Couronnes : ");
    Privilege = new QLabel("Privileges : ");
    Prestige = new QLabel("Prestige total : ");
    QString playerName = QString::fromStdString(player.getName());
    nameJoueur = new QLabel(playerName);
    numCrowns = new QLCDNumber;
    numPrivilege = new QLCDNumber;
    updatePrivilege();
    numPrestige = new QLCDNumber;
    layoutjoueur = new QHBoxLayout;
    layoutprivilege = new QHBoxLayout;
    layoutprestige = new QHBoxLayout;
    layoutcouronne = new QHBoxLayout;
    layoutCards = new QGridLayout;
    layoutTokens = new QGridLayout;
    layoutFullCards=new QVBoxLayout;
    layoutFullTokens=new QVBoxLayout;

    blueToken = new TokenWidget(this, TokenColor::BLEU);
    redToken = new TokenWidget(this, TokenColor::ROUGE);
    blackToken = new TokenWidget(this, TokenColor::NOIR);
    whiteToken = new TokenWidget(this, TokenColor::BLANC);
    pearlToken = new TokenWidget(this, TokenColor::PERLE);
    greenToken = new TokenWidget(this, TokenColor::VERT);
    goldToken = new TokenWidget(this, TokenColor::OR);

    layoutTokens->addWidget(blueToken, 0, 0);
    layoutTokens->addWidget(redToken, 0, 1);
    layoutTokens->addWidget(blackToken, 0, 2);
    layoutTokens->addWidget(whiteToken, 0, 3);
    layoutTokens->addWidget(greenToken, 1, 0);
    layoutTokens->addWidget(pearlToken, 1, 1);
    layoutTokens->addWidget(goldToken, 1, 2);

    blueCard = new CardWidget(this, TokenColor::BLEU);
    redCard = new CardWidget(this, TokenColor::ROUGE);
    greenCard = new CardWidget(this, TokenColor::VERT);
    blackCard = new CardWidget(this, TokenColor::NOIR);
    whiteCard = new CardWidget(this, TokenColor::BLANC);

    layoutCards->addWidget(blueCard, 0, 0);
    layoutCards->addWidget(redCard, 0, 1);
    layoutCards->addWidget(greenCard, 0, 2);
    layoutCards->addWidget(blackCard, 1, 0);
    layoutCards->addWidget(whiteCard, 1, 1);

    layoutMainJoueur = new QVBoxLayout;

    layoutjoueur->addWidget(typeJoueur);
    layoutjoueur->addWidget(nameJoueur);

    layoutprivilege->addWidget(Privilege);
    layoutprivilege->addWidget(numPrivilege);

    layoutprestige->addWidget(Prestige);
    layoutprestige->addWidget(numPrestige);

    layoutcouronne->addWidget(couronne);
    layoutcouronne->addWidget(numCrowns);

    layoutFullCards->addWidget(cartes);
    layoutFullCards->addLayout(layoutCards);

    layoutFullTokens->addWidget(jetons);
    layoutFullTokens->addLayout(layoutTokens);

    layoutMainJoueur->addLayout(layoutjoueur);
    layoutMainJoueur->addLayout(layoutFullCards);
    layoutMainJoueur->addWidget(popupButtonReserve);
    layoutMainJoueur->addLayout(layoutprestige);
    layoutMainJoueur->addLayout(layoutcouronne);
    layoutMainJoueur->addLayout(layoutprivilege);
    layoutMainJoueur->addLayout(layoutFullTokens);
    setLayout(layoutMainJoueur);

}

void PlayerQT::showPopup(bool update) {
    QLayoutItem* child;
    while ((child = popupLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    std::vector<JewelryCard*>& reserveCards = player.getReserve();
    reserveWidgets.clear();
    if (!reserveCards.empty()) {
        for (size_t i = 0; i < reserveCards.size(); ++i) {
            reserveQT* reserve = new reserveQT(reserveCards[i], popupDialog);
            reserve->setStatus(reserve->isReserveCardBuyable() ? reserveQT::ReserveStatus::buyable : reserveQT::ReserveStatus::notClickable);
            popupLayout->addWidget(reserve);

            // Store the pointer to the reserveQT instance
            reserveWidgets.push_back(reserve);
            if (update) {
                updateAllReserveStatus(reserveQT::ReserveStatus::buyable);
            }
            // Connect the signals and slots
            connect(reserve, &reserveQT::closePopup, popupDialog, &QDialog::close);
            connect(reserve, &reserveQT::acheterReserveClicked, this, &PlayerQT::onReserveCardSelected);
        }
    } else {
        QLabel* emptyLabel = new QLabel("Pas de cartes dans la reserve.", popupDialog);
        popupLayout->addWidget(emptyLabel);
    }

    popupDialog->exec();
}
void PlayerQT::onReserveCardSelected(JewelryCard* selectedCard) {
    qDebug() << "Card selected: Level " << selectedCard->getLevel() << " and ID " << selectedCard->getId();
    emit reserveCardSelected(selectedCard);
}


void PlayerQT::updateCrown(){
    int newCrown=player.getCrowns();
    numCrowns->display(newCrown);
}

void PlayerQT::updatePrivilege(){
    int newPrivilege=player.getNbPrivilege();
    numPrivilege->display(newPrivilege);
}

void PlayerQT::updateTotalPrestige(){
    int newPrestige=player.getPrestige();
    numPrestige->display(newPrestige);
}


void PlayerQT::updateTokens(){
    std::unordered_map<TokenColor, int> TokenSummary = player.getTokenSummary();

    int newBlueToken=TokenSummary.at(TokenColor::BLEU);
    blueToken->updateNumToken(newBlueToken);

    int newWhiteToken=TokenSummary.at(TokenColor::BLANC);
    whiteToken->updateNumToken(newWhiteToken);

    int newGreenToken=TokenSummary.at(TokenColor::VERT);
    greenToken->updateNumToken(newGreenToken);

    int newBlackToken=TokenSummary.at(TokenColor::NOIR);
    blackToken->updateNumToken(newBlackToken);

    int newRedToken=TokenSummary.at(TokenColor::ROUGE);
    redToken->updateNumToken(newRedToken);

    int newGoldToken=TokenSummary.at(TokenColor::OR);
    goldToken->updateNumToken(newGoldToken);

    int newPearlToken=TokenSummary.at(TokenColor::PERLE);
    pearlToken->updateNumToken(newPearlToken);

}


void PlayerQT::updateCards() {
    // Blue
    SummaryCard blueCardSummary = player.getBlueSummary();
    blueCard->updateBonus(blueCardSummary.getBonusNumber());
    blueCard->updatePrestige(blueCardSummary.getPrestigePoints());

    // Green
    SummaryCard greenCardSummary = player.getGreenSummary();
    greenCard->updateBonus(greenCardSummary.getBonusNumber());
    greenCard->updatePrestige(greenCardSummary.getPrestigePoints());

    // Red
    SummaryCard redCardSummary = player.getRedSummary();
    redCard->updateBonus(redCardSummary.getBonusNumber());
    redCard->updatePrestige(redCardSummary.getPrestigePoints());

    // Black
    SummaryCard blackCardSummary = player.getBlackSummary();
    blackCard->updateBonus(blackCardSummary.getBonusNumber());
    blackCard->updatePrestige(blackCardSummary.getPrestigePoints());

    // White
    SummaryCard whiteCardSummary = player.getWhiteSummary();
    whiteCard->updateBonus(whiteCardSummary.getBonusNumber());
    whiteCard->updatePrestige(whiteCardSummary.getPrestigePoints());
    // a voir pour la carte resume royale (est ce qu'elle est utile ?)
}

void PlayerQT::updateAllPlayer(){
    updatePrivilege();
    updateCrown();
    updateCards();
    updateTokens();
    updateTotalPrestige();
};

void PlayerQT::toggleTextBoldJoueur(bool isBold) {
    QFont font = typeJoueur->font();
    QColor color;
    if (isBold) {
        // Set the text to bold
        font.setBold(true);
        color = Qt::red;
    } else {
        // Set the text to normal
        font.setBold(false);
        color = palette().color(QPalette::WindowText);
    }

    typeJoueur->setFont(font);
    nameJoueur->setFont(font);
    typeJoueur->setStyleSheet(QString("QLabel { color: %1 }").arg(color.name()));
    nameJoueur->setStyleSheet(QString("QLabel { color: %1 }").arg(color.name()));
}

void PlayerQT::updateAllReserveStatus(reserveQT::ReserveStatus newStatus) {
    for (reserveQT* reserve : reserveWidgets) {
        if (reserve) {
            reserve->setStatus(newStatus);
        }
    }
}