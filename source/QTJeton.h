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
    explicit PrivilegeCounter(Board& board, QWidget* parent = nullptr) : QLabel(parent), board(board) {
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
        setText(QString::number(board.getNbPrivileges()));
    }

private:
    Board& board;
};

class CircleWidget : public QWidget {
public:
    CircleWidget(QWidget* parent,const Token& token);

    void paintEvent(QPaintEvent* event) override;

private:
    QColor convertColor(const Token &token);
};

class PlateWidget : public QWidget {
public:
    explicit PlateWidget(Board& board, QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<std::vector<CircleWidget*>> widgets;
};

class MainWindow : public QWidget, public Observer {
public:
    explicit MainWindow(Board& board, QWidget* parent = nullptr) : QWidget(parent), board(board) {
        auto* plateWidget = new PlateWidget(board, this);
        plateWidget->move(0, 0);

        privilegeCounter = new PrivilegeCounter(board, this);
        privilegeCounter->move(plateWidget->width() + 10, 0); // Position on the right side

        board.registerObserver(this);

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

    void update() override {
        updatePrivilegeCounter();
    }


private:
    Board& board;
    PrivilegeCounter* privilegeCounter;
};
#endif //LO21PROJECT_QTJETON_H
