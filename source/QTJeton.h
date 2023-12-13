//
// Created by utcpret on 24/11/2023.
//

#ifndef LO21PROJECT_QTJETON_H
#define LO21PROJECT_QTJETON_H

#include <QtWidgets>
#include <QColor>
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


class PrivilegeCounter : public QLabel {
public:
    explicit PrivilegeCounter(QWidget* parent = nullptr) : QLabel(parent) {
        // Set the stylesheet
        setStyleSheet("QLabel {"
                      "background-color : black;"
                      "color : white;"
                      "border: 2px solid white;"
                      "border-radius: 10px;"
                      "padding: 5px;"
                      "font: bold 14px;"
                      "}");

        updateCounter();
    }

    void updateCounter() {
        setText(QString::number(Board::getInstance().getNbPrivileges()));
    }
};

class CircleWidget : public QPushButton {
    Q_OBJECT
public:
    CircleWidget(QWidget* parent = nullptr, const Token* token = nullptr, unsigned int rad = 0, position* pos = nullptr);

    void paintEvent(QPaintEvent* event) override;

    void changeSelect(){selected = !selected; update();};
    void unselect(){selected = false; update();};
    void appear(){show();};
    void disappear(){hide();};
    position* getPosition(){return pos;}
    const Token* getJeton(){return token;}

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
};

class PlateView : public QWidget {
    Q_OBJECT
public:
    PlateView(QWidget* parent = nullptr, unsigned int h = 0, unsigned int w = 0);

    void updatePrivilegeCounter() {
        privilegeCounter->updateCounter();
    }

    void clickOnToken(unsigned int i);
    void unselectToken();
    bool isSelected(CircleWidget* button);
    std::vector<const Token*> validateTokens();
    void hideElements();
    void showTokens(){for(unsigned int i = 0; i < nbTokens; i++){buttons[i]->show();}update();}
    signals:
        void tokensValidated(std::vector<const Token*> tokens);

private:
    PrivilegeCounter* privilegeCounter;
    PlateWidget* plateWidget;
    std::vector<CircleWidget*> buttons;
    unsigned int nbTokens;
    unsigned int rnbTokens;
    std::array<CircleWidget*, 3> selectedTokens;
    unsigned int nbSelectedTokens = 0;
    unsigned int h;
    unsigned int w;
    unsigned int xHGButton;
    unsigned int yHGButton;

    QPushButton* validateButton;
    QVBoxLayout* layout;

};
#endif //LO21PROJECT_QTJETON_H
