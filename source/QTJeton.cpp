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
        return QColor::fromRgb(255, 255, 0);
    } else if (token.getColor() == TokenColor::OR) {
        return QColor::fromRgb(255, 215, 0);
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