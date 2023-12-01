//
// Created by utcpret on 24/11/2023.
//

#include "QTJeton.h"
#include <QtWidgets>
#include <QColor>
#include <QPainter>
#include <QRandomGenerator>
#include <QPalette>


CircleWidget::CircleWidget(QWidget* parent,const Token& token) : QWidget(parent) {
    setFixedSize(50, 50);

    QPalette palette;
    QColor randomColor = convertColor(token);
    palette.setColor(QPalette::Base, randomColor);
    setAutoFillBackground(true);
    setPalette(palette);
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
}

PlateWidget::PlateWidget(Board& board, QWidget* parent) : QWidget(parent) {
    int tokenSize = 50; // The size of each token
    Board::BoardIterator it = board.iterator();
    unsigned int i = 0;
    while (it.hasNext()) {
        std::vector<CircleWidget*> widgetRow;
        for (int j = 0; j < 5; ++j) {
            const auto& token = it.next();;
            if (token != nullptr) {
                CircleWidget* widget = new CircleWidget(this, *token);
                widget->move(j * tokenSize, i * tokenSize); // Position the widget
                widgetRow.push_back(widget);
                std::cout << "Created CircleWidget for token\n";
            } else {
                widgetRow.push_back(nullptr);
            }
        }
        ++i;
        widgets.push_back(widgetRow);
    }
    // Set the size of the PlateWidget to fit the board
    setFixedSize(5 * tokenSize, 5 * tokenSize);
}

void PlateWidget::paintEvent(QPaintEvent* event) {
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


void CircleWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().base());
    painter.drawEllipse(rect().adjusted(2, 2, -2, -2));
}