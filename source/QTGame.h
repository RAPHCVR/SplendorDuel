//
// Created by utcpret on 13/12/2023.
//

#ifndef QTGAME_H
#define QTGAME_H
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScreen>
#include <QSize>
#include "Controller.h"
#include "QTJeton.h"

class QTGame : public QWidget {
    Q_OBJECT
private:
    Controller* controller;
    PlateView* plateView;

    QHBoxLayout* first;
    QVBoxLayout* mainlayout;
    QScreen* screen;
    QSize* size;
    unsigned int width;
    unsigned int height;
public:
    void quit() {
        this->close();
    }
    void paintEvent(QPaintEvent* event) override;
    QTGame(QWidget* parent = nullptr);
    void handleTokenSelection(std::vector<const Token*> tokens);
};

#endif //QTGAME_H
