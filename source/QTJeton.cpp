//
// Created by utcpret on 24/11/2023.
//

#include "QTJeton.h"
#include <QtWidgets>
#include <QColor>
#include <QPainter>
#include <QRandomGenerator>


CircleWidget::CircleWidget(QWidget* parent, const Token* token, unsigned int rad, position* p): QPushButton(parent), token(token), pos(p) {
    color = convertColor(*token);
    backGroundColor = convertBackgroundColor(*token);
    borderColor = convertBorderColor(*token);
    radius = rad;
    setFixedSize(2 * rad, 2 * rad);
    setStyleSheet("QPushButton:pressed { border: none; }");
    selected = false;
    setFlat(true);
}

QColor CircleWidget::convertColor(const Token &token) {
    if (token.getColor() == TokenColor::BLEU) {
        return QColor::fromRgb(0, 0, 255);
    } else if (token.getColor() == TokenColor::BLANC) {
        return QColor::fromRgb(255, 255, 255);
    } else if (token.getColor() == TokenColor::VERT) {
        return QColor::fromRgb(0, 255, 0);
    } else if (token.getColor() == TokenColor::NOIR) {
        return QColor::fromRgb(0, 0, 0);
    } else if (token.getColor() == TokenColor::ROUGE) {
        return QColor::fromRgb(255, 0, 0);
    } else if (token.getColor() == TokenColor::PERLE) {
        //rgb color purple brillant
        return QColor::fromRgb(153, 51, 255);
    } else if (token.getColor() == TokenColor::OR) {
        return QColor::fromRgb(255, 215, 0);
    }
    else {
        return QColor::fromRgb(255, 255, 255);
    }
}

QColor CircleWidget::convertBackgroundColor(const Token& token) {
    if (token.getColor() == TokenColor::BLEU) {
        return QColor("#2FC5FF");
    } else if (token.getColor() == TokenColor::BLANC) {
        return QColor::fromRgb(255, 255, 255);
    } else if (token.getColor() == TokenColor::VERT) {
        return QColor("#88C85A");
    } else if (token.getColor() == TokenColor::NOIR) {
        return QColor::fromRgb(0, 0, 0);
    } else if (token.getColor() == TokenColor::ROUGE) {
        return QColor("#CD534E");
    } else if (token.getColor() == TokenColor::PERLE) {
        //rgb color purple brillant
        return QColor("#EFC7FC");
    } else if (token.getColor() == TokenColor::OR) {
        return QColor("#E6DE73");
    }
    else{
        return QColor::fromRgb(255, 255, 255);
    }
}

QColor CircleWidget::convertBorderColor(const Token& token) {
    if (token.getColor() == TokenColor::BLANC) {
        return QColor::fromRgb(0, 0, 0);
    } else if (token.getColor() == TokenColor::NOIR) {
        return QColor::fromRgb(255, 255, 255);
    } else{
        return QColor::fromRgb(255, 255, 255);
    }
}

PlateWidget::PlateWidget(QWidget* parent, unsigned int h, unsigned int w, unsigned int nbTokens, unsigned int tokenSize, std::vector<CircleWidget*>* butt): QWidget(parent), nbTokens(nbTokens), h(h), w(w), tokenSize(tokenSize), buttons(butt) {

    rnbTokens = sqrt(nbTokens);
    for (unsigned int i = 0; i < rnbTokens; i++) {
        for (unsigned int j = 0; j < rnbTokens; j++) {
            QRect* rect = new QRect(2 * i * (tokenSize + 5) + (w - (2 * (tokenSize + 5)) * rnbTokens)/2, 2 * j * (tokenSize + 5), 2 * (tokenSize + 5), 2 * (tokenSize + 5));
            rectangles.push_back(rect);
        }
    }
    setFixedSize(w, h);

    /*
    grille = new QGridLayout(this);
    rnbTokens = sqrt(nbTokens);
    unsigned int k = 0;
    for (unsigned int i = 0; i < rnbTokens; i++) {
        for (unsigned int j = 0; j < rnbTokens; j++) {
            grille->addWidget((*butt)[k], i, j);
            k++;
        }
    }
    */

}

void PlateWidget::placeTokens() {
    for (auto button : *buttons) {
        unsigned int j = button->getPosition()->getx() + button->getPosition()->gety() * rnbTokens;
        unsigned int posx = rectangles[j]->center().x() - button->width() / 2 + 2.5;
        unsigned int posy = rectangles[j]->center().y() - button->height() / 2 + 2.5;
        button->move(posx, posy);
    }
}

void PlateWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor("black"), 4));
    painter.setBrush(QColor("orange"));
    for (auto rect : rectangles) {
        painter.drawRect(*rect);
    }
}


void CircleWidget::paintEvent(QPaintEvent* event) {
    QPushButton::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    unsigned int size = std::min(width(), height());
    QPoint center = rect().center();

    if(selected) {
        painter.setPen(QPen(Qt::gray, 4));
    }
    else {
        painter.setPen(QPen(borderColor,3));
    }
    int Radius = size / 2;
    painter.setBrush(QBrush(backGroundColor));
    painter.drawEllipse(center, Radius - 3, Radius - 3);

    QVector<QPoint> tokenShape;
    const int width = size * 2 / 3;
    const int height = size * 3 / 4;
    // Define the points for the token shape
    tokenShape << QPoint(center.x(), center.y() - height / 2); // Top point
    tokenShape << QPoint(center.x() + width / 2, center.y()); // Right point
    tokenShape << QPoint(center.x(), center.y() + height / 2); // Bottom point
    tokenShape << QPoint(center.x() - width / 2, center.y()); // Left point

    // Draw the token shape
    painter.setPen(QPen(borderColor, 2)); // Set the pen for the token shape
    painter.setBrush(QBrush(color)); // Set the brush for the token shape
    painter.drawPolygon(tokenShape); // Draw the token shape
    if (this->isDown()) {
        painter.setBrush(QColor(0,0,0,30));
        painter.setPen(Qt::NoPen);
        painter.drawPolygon(tokenShape);
        painter.drawEllipse(center, Radius, Radius);
    }
}

PlateView::PlateView(QWidget* parent, unsigned height, unsigned width): h(height),w(width), status("take3tokens"){
    nbTokens = 25;
    rnbTokens = sqrt(nbTokens);
    setFixedSize(w, h); //Fixe la taille du plateau
    max_nbSelectedTokens = 0;
    int TokenSize = (h - 100)/(2*rnbTokens) - 5;

    PrivilegeCounter *counter = new PrivilegeCounter(this);
    privilegeCounter=counter;
    privilegeCounter->setFixedWidth(35);
    plateWidget = new PlateWidget(nullptr, h-100, w, nbTokens, TokenSize, &buttons);
    Board::BoardIterator it = Board::getInstance().iterator();
    unsigned int j = 0;
    for(int i = 0; i < nbTokens; i++){
        //Creer un getteur pour les Jetons
        const Token* token = it.next();
        if (token != nullptr) {
            buttons.push_back(new CircleWidget(plateWidget, token, TokenSize, new position((i/rnbTokens), (i%rnbTokens))));
            QObject::connect(buttons[j], &CircleWidget::clicked, [this, j]() {
            clickOnToken(j); //Permet d'appeler la fonction boutonClique(int i) lorsque le bouton i est clique;
            });
            j++;
        }
    }
    for(int i = 0; i < 3; i++){
        selectedTokens[i] = nullptr; //Initialise jetonSelection avec nullptr
    }

    plateWidget->placeTokens();
    validateButton = new QPushButton("Valider le choix des jetons"); //Creer le bouton valider (pour la selection des jetons)
    validateButton->setStyleSheet("color blue;");

    layout = new QVBoxLayout; //Layout pour mettre le Grid + les boutons en dessous
    layout->addWidget(privilegeCounter);
    layout -> addWidget(plateWidget); //Ajoute layoutJetons au layout vertical
    layout -> addWidget(validateButton); //Ajoute layoutJetons au layout vertical (faire un QHBoxLayout pour ajouter aussi un bouton desselctionner)

    setLayout(layout); //Set le layout

    connect(validateButton, &QPushButton::clicked, this, &PlateView::validateTokens); //connect boutonValider avec valliderJetons
}

void PlateView::clickOnToken(unsigned i) {
    unsigned int j = 0;
    if (isSelected(buttons[i])) {
        for (j = 0; j < max_nbSelectedTokens; j++) {
            if (selectedTokens[j] == buttons[i]) {
                selectedTokens[j] = nullptr;
                buttons[i] -> changeSelect();
                nbSelectedTokens -= 1;
            }
        }
    }
    else {
        if (nbSelectedTokens < max_nbSelectedTokens) {
            buttons[i] -> changeSelect();
            while(selectedTokens[j] != nullptr) {
                j++;
            }
            selectedTokens[j] = buttons[i];
            nbSelectedTokens += 1;
        }
    }
}

void PlateView::unselectToken() {
    for (unsigned int i = 0; i < 3; i++) {
        if (selectedTokens[i] != nullptr) {
            selectedTokens[i] -> changeSelect();
            selectedTokens[i]->disappear();
            selectedTokens[i] = nullptr;
        }
    }
    nbSelectedTokens = 0;
}

void PlateView::validateTokens() {
    std::vector<std::pair<int, int>>* pos = new std::vector<std::pair<int, int>>;
    std::vector<const Token*> tokens;
    for(int j = 0; j < 3; j++){
        if(selectedTokens[j] != nullptr) {
            if(Board::getInstance().CellColor(selectedTokens[j]->getPosition()->getx(),selectedTokens[j]->getPosition()->gety(),TokenColor::OR)){
                if (status != "gold") {
                    throw TokenException("Impossible de prendre un jeton or");
                }
            }
            pos->push_back(std::make_pair(selectedTokens[j]->getPosition()->getx(), selectedTokens[j]->getPosition()->gety()));
        }
    }
    if ((areCoordinatesAlignedAndConsecutive(pos) || pos->size()==1) &&status=="take3tokens") {
        for (auto pair: *pos) {
            tokens.push_back(&Board::getInstance().takeToken(pair.first, pair.second));
        }
        unselectToken();
        emit tokensValidated(tokens);
        updateMaxNbSelectedTokens(0);
    }
    else if (status == "privileges") {
        for (auto pair: *pos) {
            tokens.push_back(&Board::getInstance().takeToken(pair.first, pair.second));
        }
        unselectToken();
        emit tokensValidated(tokens);
        emit privilegeUsed(tokens.size());
        updateMaxNbSelectedTokens(0);
    }
    else if (status == "gold") {
        for (auto pair: *pos) {
            tokens.push_back(&Board::getInstance().takeToken(pair.first, pair.second));
        }
        unselectToken();
        emit tokensValidated(tokens);
        updateMaxNbSelectedTokens(0);
        status = "take3tokens";
    }
    else {
        throw TokenException("Les jetons ne sont pas alignes ou consecutifs");
    }
    emit endOfTurn();
}

bool PlateView::isSelected(CircleWidget* button) {
    for (unsigned int i = 0; i < 3; i++) {
        if (selectedTokens[i] == button) {
            return true;
        }
    }
    return false;
}

void PlateView::hideElements() {
    for (unsigned int i = 0; i < nbTokens; i++) {
        buttons[i]->disappear();
    }
    validateButton->hide();
}

void PlateView::updateWidgetsFromBoard() {
    // Get the Board instance and create a BoardIterator
    Board& board = Board::getInstance();
    Board::BoardIterator it = board.iterator();

    // Iterate over the tokens in the Board
    for (int i = 0; i < nbTokens; ++i) {
        const Token* token = it.next();
        if (token != nullptr) {
            // Find the corresponding CircleWidget and update its token
            CircleWidget* circleWidget = buttons[i];
            circleWidget->setToken(token);
            circleWidget->show();
        }
    }

    // Update the PlateView to reflect the changes
    update();
}

void CircleWidget::setToken(const Token* newToken) {
    // Update the token
    token = newToken;

    // Update the colors based on the new token
    color = convertColor(*newToken);
    backGroundColor = convertBackgroundColor(*newToken);
    borderColor = convertBorderColor(*newToken);

    // Redraw the widget
    update();
}

