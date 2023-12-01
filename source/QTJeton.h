//
// Created by utcpret on 24/11/2023.
//

#ifndef LO21PROJECT_QTJETON_H
#define LO21PROJECT_QTJETON_H

#include <QtWidgets>
#include <QColor>
#include <QPalette>
#include "Jeton.h"

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

class MainWindow : public QWidget {
public:
    explicit MainWindow(Board& board, QWidget* parent = nullptr) : QWidget(parent) {
        auto* plateWidget = new PlateWidget(board, this);
        plateWidget->move(0, 0);

        // Set the size of the MainWindow to fit the PlateWidget
        setFixedSize(plateWidget->width(), plateWidget->height());

        // Change the background color of the MainWindow
        QPalette palette;
        palette.setColor(QPalette::Window, QColor::fromRgb(100, 100, 100)); // Change to your desired color
        setAutoFillBackground(true);
        setPalette(palette);
    }
};
#endif //LO21PROJECT_QTJETON_H
