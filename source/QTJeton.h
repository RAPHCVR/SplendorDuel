//
// Created by utcpret on 24/11/2023.
//

#ifndef LO21PROJECT_QTJETON_H
#define LO21PROJECT_QTJETON_H

#include <QtWidgets>
#include <QColor>
#include <QPainter>
#include <QRandomGenerator>
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
    explicit PlateWidget(Board& board, QWidget* parent = nullptr) : QWidget(parent) {
        int tokenSize = 50; // The size of each token
        for (int i = 0; i < board.tokens.size(); ++i) {
            const auto& row = board.tokens[i];
            std::vector<CircleWidget*> widgetRow;
            for (int j = 0; j < row.size(); ++j) {
                const auto& token = row[j];
                if (token != nullptr) {
                    CircleWidget* widget = new CircleWidget(this, *token);
                    widget->move(j * tokenSize, i * tokenSize); // Position the widget
                    widgetRow.push_back(widget);
                    std::cout << "Created CircleWidget for token\n";
                } else {
                    widgetRow.push_back(nullptr);
                }
            }
            widgets.push_back(widgetRow);
        }
        // Set the size of the PlateWidget to fit the board
        setFixedSize(board.tokens.size() * tokenSize, board.tokens[0].size() * tokenSize);
    }
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event)

        QPainter painter(this);
        for (const auto& row : widgets) {
            for (const auto& widget : row) {
                if (widget != nullptr) {
                    widget->update();
                    std::cout << "Updated CircleWidget\n";
                }
            }
        }
    }

private:
    std::vector<std::vector<CircleWidget*>> widgets;
};

class MainWindow : public QWidget {
public:
    explicit MainWindow(Board& board, QWidget* parent = nullptr) : QWidget(parent) {
        PlateWidget* plateWidget = new PlateWidget(board, this);
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
