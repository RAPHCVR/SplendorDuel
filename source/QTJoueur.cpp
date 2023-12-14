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
        return QColor(0, 0, 255);  // Blue
    case TokenColor::BLANC:
        return QColor(255, 255, 255);  // White
    case TokenColor::VERT:
        return QColor(0, 255, 0);  // Green
    case TokenColor::NOIR:
        return QColor(0, 0, 0);  // Black
    case TokenColor::ROUGE:
        return QColor(255, 0, 0);  // Red
    case TokenColor::PERLE:
        // Replace with the desired color values for PERLE
        return QColor(200, 200, 200);  // Example gray color
    case TokenColor::OR:
        return QColor(255, 215, 0);  // Gold
    case TokenColor::None:
    default:
        return QColor();  // Default to an invalid color
    }
}

TokenWidget::TokenWidget(QWidget* parent, const TokenColor c) : QWidget(parent), color(c) {
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

void TokenWidget::setColor(const TokenColor* color) {
    QColor newColor = convertToQColorFromTokenColor(*color);
    QPalette palette;
    palette.setColor(QPalette::Base, newColor);
    setAutoFillBackground(true);
    setPalette(palette);
    QWidget::update();
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

CardWidget::CardWidget(QWidget *parent, const TokenColor color)
    : QWidget(parent) {

    // Set la couleur de fond
    QColor newColor=convertToQColorFromTokenColor(color);
    setStyleSheet(QString("background-color: %1;").arg(newColor.name()));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);

    bonus = new QLCDNumber(this);
    bonus->display(0);
    layout->addWidget(bonus);

    prestige = new QLCDNumber(this);
    prestige->display(0);
    layout->addWidget(prestige);
}

//Methodes Joueur global

PlayerQT::PlayerQT(const Player player, QWidget* parent) : QWidget(parent) {
    setWindowTitle("Player");

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
    numPrestige=new QLCDNumber;
    layoutjoueur = new QHBoxLayout;
    layoutprivilege = new QHBoxLayout;
    layoutprestige = new QHBoxLayout;
    layoutcouronne = new QHBoxLayout;
    layoutCards = new QGridLayout;
    layoutTokens = new QGridLayout;


    CardWidget* blueCard=new CardWidget(this, TokenColor::BLEU);
    CardWidget* redCard=new CardWidget(this,TokenColor::ROUGE);
    CardWidget* greenCard=new CardWidget(this, TokenColor::VERT);
    CardWidget* blackCard=new CardWidget(this, TokenColor::NOIR);
    CardWidget* whiteCard=new CardWidget(this, TokenColor::BLANC);
    CardWidget* royalCard=new CardWidget(this, TokenColor::OR);

    layoutCards->addWidget(blueCard,0,0);
    layoutCards->addWidget(redCard,0,1);
    layoutCards->addWidget(greenCard,0,2);
    layoutCards->addWidget(blackCard,1,0);
    layoutCards->addWidget(whiteCard,1,1);
    layoutCards->addWidget(royalCard,1,2);

    setLayout(layoutCards);

    TokenWidget* blueToken=new TokenWidget(this,TokenColor::BLEU);
    TokenWidget* redToken=new TokenWidget(this,TokenColor::ROUGE);
    TokenWidget* blackToken=new TokenWidget(this,TokenColor::NOIR);
    TokenWidget* whiteToken=new TokenWidget(this,TokenColor::BLANC);
    TokenWidget* pearlToken=new TokenWidget(this,TokenColor::PERLE);
    TokenWidget* greenToken=new TokenWidget(this,TokenColor::VERT);
    TokenWidget* goldToken=new TokenWidget(this,TokenColor::OR);

    layoutTokens->addWidget(blueToken,0,0);
    layoutTokens->addWidget(redToken,0,1);
    layoutTokens->addWidget(blackToken,0,2);
    layoutTokens->addWidget(whiteToken,0,3);
    layoutTokens->addWidget(greenToken,1,0);
    layoutTokens->addWidget(pearlToken,1,1);
    layoutTokens->addWidget(goldToken,1,2);

    setLayout(layoutTokens);

    layoutMainJoueur = new QVBoxLayout;

    layoutMainJoueur->addLayout(layoutTokens);
    setLayout(layoutMainJoueur);
    layoutjoueur->addWidget(typeJoueur);
    layoutjoueur->addWidget(nameJoueur);

    layoutprivilege->addWidget(Privilege);
    layoutprivilege->addWidget(numPrivilege);

    layoutprestige->addWidget(Prestige);
    layoutprestige->addWidget(numPrestige);

    layoutcouronne->addWidget(couronne);
    layoutcouronne->addWidget(numCrowns);

    layoutMainJoueur->addLayout(layoutjoueur);
    layoutMainJoueur->addLayout(layoutcouronne);
    layoutMainJoueur->addLayout(layoutprivilege);

}

void PlayerQT::updateCrown(const Player player){
    int newCrown=player.getCrowns();
    numCrowns->display(newCrown);
}

void PlayerQT::updatePrivilege(const Player player){
    int newPrivilege=player.getPrivilege();
    numPrivilege->display(newPrivilege);
}

void PlayerQT::updateTotalPrestige(const Player player){
    int newPrestige=player.getPrestige();
    numPrestige->display(newPrestige);
}


void PlayerQT::updateTokens(Player player){
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


void PlayerQT::updateCards(Player player) {
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
