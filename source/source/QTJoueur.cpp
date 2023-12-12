#include "QTJoueur.h"

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
    QColor randomColor = convertToQColorFromTokenColor(*color);
    palette.setColor(QPalette::Base, randomColor);
    setAutoFillBackground(true);
    setPalette(palette);
    numToken = new QLCDNumber(this);
    numToken->setDigitCount(1);
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

void TokenWidget::updateNumToken(const TokenColor* color){ //ne marche pas
    std::unordered_map<TokenColor, int> resume player.getTokenSummary();
    int newNum=resume.at(color);
    numToken->display(newNum);

}

//Methodes widget carte resume

void CardWidget::updateBonus(SummaryCard carte){
    int newBonus=carte.getBonusNumber();
    bonus->display(newBonus);
};

void CardWidget::updatePrestige(SummaryCard carte){
    int newPrestige=carte.getPrestigePoints();
    prestige->display(newPrestige);
};

CardWidget::CardWidget(const TokenColor color, QWidget *parent)
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

    joueur = new QLabel("Joueur ");
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
    layoutCards = new QGridLayout;  // Use layoutCards instead of gridLayout
    layoutTokens = new QGridLayout;

    for (int i = 0; i < 6; ++i) {
        CardWidget *card = new CardWidget(static_cast<TokenColor>(i), this);
        SummaryCards.push_back(card);
    }

    // Adding widgets from the vector to the grid layout
    int row = 0;
    int col = 0;

    for (CardWidget* card : SummaryCards) {
        layoutCards->addWidget(card, row, col);

        ++col;
        if (col > 2) {
            col = 0;
            ++row;
        }
    }

    for (int i = 0; i < 6; ++i) {
        TokenWidget* tokenWidget = new TokenWidget(this, static_cast<TokenColor>(i));
        SummaryTokens.push_back(tokenWidget);
    }


    // Adding widgets from the vector to the grid layout
    int tokenRow = 0;
    int tokenCol = 0;

    for (TokenWidget* tokenWidget : SummaryTokens) {
        layoutTokens->addWidget(tokenWidget, tokenRow, tokenCol);

        ++tokenCol;
        if (tokenCol > 2) {
            tokenCol = 0;
            ++tokenRow;
        }
    }

    setLayout(layoutCards);

    layoutMainJoueur = new QVBoxLayout;

    layoutMainJoueur->addLayout(layoutTokens);
    setLayout(layoutMainJoueur);
    layoutjoueur->addWidget(joueur);
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

    layoutCards->addLayout(layoutMainJoueur, row, col);

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
