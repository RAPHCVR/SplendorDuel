//
// Created by utcpret on 13/12/2023.
//

#include "QTGame.h"

QTGame::QTGame(QWidget* parent) : QWidget(parent) {
    controller = new Controller();
    screen = QGuiApplication::primaryScreen();
    size = new QSize(screen->size()/2);
    width = size->width();
    height = size->height();
    setFixedSize(width, height);
    mainlayout = new QVBoxLayout(this);
    first = new QHBoxLayout();
    plateView = new PlateView(nullptr, height-100,width/2);
    first->addWidget(plateView);
    mainlayout->addLayout(first);
    setLayout(mainlayout);
    connect(plateView, &PlateView::tokensValidated, this, &QTGame::handleTokenSelection);
}

void QTGame::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 5));
    painter.drawRect(rect());
}

void QTGame::handleTokenSelection(std::vector<const Token*> tokens) {
    for (auto token : tokens) {
        controller->getcurrentPlayer().addToken(*token);
    }
}