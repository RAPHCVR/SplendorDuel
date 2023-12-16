//
// Created by utcpret on 24/11/2023.
//

#ifndef LO21PROJECT_QTJETON_H
#define LO21PROJECT_QTJETON_H

#include <QtWidgets>
#include <QColor>
#include <utility>
#include "Controller.h"

class position{
private:
    int x;
    int y;
public:
    position(int x, int y) : x(x), y(y){}
    int getx() const {return x;}
    int gety() const {return y;}
};


class PrivilegeCounter : public QWidget {
    Q_OBJECT

public:
    PrivilegeCounter(QWidget *parent = nullptr) : QWidget(parent), valeur(Board::getInstance().getNbPrivileges()) {}

    void updateValue() {
        valeur = Board::getInstance().getNbPrivileges();
        update(); // Redessine le widget
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        int diameter = qMin(width(), height());

        // Créer un rectangle pour définir les limites du cercle
        QRect circleRect(0, 0, diameter, diameter);
        circleRect.moveCenter(rect().center()); // Centrer le cercle dans le widget

        // Dessiner le fond circulaire
        QLinearGradient gradient(circleRect.topLeft(), circleRect.bottomRight());
        gradient.setColorAt(0, Qt::lightGray);
        gradient.setColorAt(1, Qt::darkGray);
        painter.setBrush(gradient);
        painter.setPen(Qt::black);
        painter.drawEllipse(circleRect);

        // Configurer la police et la taille
        QFont font("Arial", 30, QFont::Bold);
        painter.setFont(font);
        painter.setPen(Qt::white); // Couleur du texte

        // Dessiner le texte
        painter.drawText(circleRect, Qt::AlignCenter, QString::number(valeur));
    }
private:
    unsigned int valeur;
};

class CircleWidget : public QPushButton {
    Q_OBJECT
public:
    CircleWidget(QWidget* parent = nullptr, const Token* token = nullptr, unsigned int rad = 0, position* pos = nullptr);

    void paintEvent(QPaintEvent* event) override;

    void changeSelect(){selected = !selected; update();};
    void setToken(const Token* newToken);
    void unselect(){selected = false; update();};
    void appear(){show();};
    void disappear(){hide();};
    position* getPosition(){return pos;}
    const Token* getToken(){return token;}

private:
    QColor convertColor(const Token &token);
    QColor convertBackgroundColor(const Token &token);
    QColor convertBorderColor(const Token &token);
    const Token* token;
    position* pos;
    QColor color;
    QColor borderColor;
    QColor backGroundColor;
    bool selected;
    unsigned int radius;
};

class PlateWidget : public QWidget {
    Q_OBJECT
public:
    PlateWidget(QWidget* parent = nullptr, unsigned int h = 0, unsigned int w = 0, unsigned int nbTokens = 0, unsigned int tokenSize = 0, std::vector<CircleWidget*>* buttons = nullptr);
    void paintEvent(QPaintEvent* event) override;
    void placeTokens();

private:
    std::vector<CircleWidget*>* buttons;
    std::vector<QRect*> rectangles;
    unsigned int nbTokens;
    unsigned int rnbTokens;
    unsigned int h;
    unsigned int w;
    unsigned int tokenSize;

    //QGridLayout *grille;
};

class PlateView : public QWidget {
    Q_OBJECT
public:
    PlateView(QWidget* parent = nullptr, unsigned int h = 0, unsigned int w = 0);


    void clickOnToken(unsigned int i);
    void unselectToken();
    bool isSelected(CircleWidget* button);
    void validateTokens();
    void hideElements();
    void showTokens(){for(unsigned int i = 0; i < nbTokens; i++){buttons[i]->show();}update();}
    void updateWidgetsFromBoard();
    void updateStatus(std::string status){this->status = std::move(status); update();};
    std::string& getStatus(){return status;};
    void updateMaxNbSelectedTokens(const unsigned int nb){max_nbSelectedTokens = nb;};
    signals:
        void tokensValidated(std::vector<const Token*> tokens);
        void privilegeUsed(unsigned int nb);
        void endOfTurn();

private:
    //PrivilegeCounter* privilegeCounter;
    PlateWidget* plateWidget;
    std::vector<CircleWidget*> buttons;
    unsigned int nbTokens;
    unsigned int rnbTokens;
    unsigned int max_nbSelectedTokens;
    std::array<CircleWidget*, 3> selectedTokens;
    unsigned int nbSelectedTokens = 0;
    unsigned int h;
    unsigned int w;
    std::string status;
    QPushButton* validateButton;
    QGridLayout* layout;

};
#endif //LO21PROJECT_QTJETON_H
