//
// Created by utcpret on 24/11/2023.
//

#ifndef LO21PROJECT_QTJETON_H
#define LO21PROJECT_QTJETON_H

#include <QtWidgets>
#include <QColor>
#include <QPalette>
#include "Jeton.h"

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

class CircleWidget : public QWidget {
public:
    CircleWidget(QWidget* parent,const Token* token);

    void paintEvent(QPaintEvent* event) override;

    void updateToken(const Token* newToken);

private:
    QColor convertColor(const Token &token);
    const Token* token;
};

class PlateWidget : public QWidget {
public:
    explicit PlateWidget(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;
    void updateTokens();

private:
    std::vector<std::vector<CircleWidget*>> widgets;
};

class MainWindow : public QWidget, public Observer {
public:
    explicit MainWindow(QWidget* parent = nullptr) : QWidget(parent) {
        plateWidget = new PlateWidget(this);
        plateWidget->move(0, 0);

        privilegeCounter = new PrivilegeCounter(this);
        privilegeCounter->move(plateWidget->width() + 10, 0); // Position on the right side

        Board::getInstance().registerObserver(this);

        // Set the size of the MainWindow to fit the PlateWidget and PrivilegeCounter
        setFixedSize(plateWidget->width() + privilegeCounter->width(), plateWidget->height());

        // Change the background color of the MainWindow
        QPalette palette;
        palette.setColor(QPalette::Window, QColor::fromRgb(100, 100, 100)); // Change to your desired color
        setAutoFillBackground(true);
        setPalette(palette);
    }

    void updatePrivilegeCounter() {
        privilegeCounter->updateCounter();
    }

    void updatePlateWidget() {
        plateWidget->updateTokens();
    }

    void update() override {
        updatePlateWidget();
        updatePrivilegeCounter();
    }


private:
    PrivilegeCounter* privilegeCounter;
    PlateWidget* plateWidget;
};
#endif //LO21PROJECT_QTJETON_H
