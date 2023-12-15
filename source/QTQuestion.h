//
// Created by utcpret on 15/12/2023.
//

#ifndef QTQUESTION_H
#define QTQUESTION_H

#include <QPushButton>
#include <QLabel>
#include <string>
#include <QVBoxLayout>

class askPopUp : public QWidget{
    Q_OBJECT
private:
    QPushButton* yes;
    QPushButton* no;
    std::string texte1 = "Voulez-vous ";
    std::string texte3 = " ?";

    QLabel* info; //Recupere les infos a donner dans la question

    QHBoxLayout* boutonLayout; //Layout avec les boutons
    QVBoxLayout* layout; //Layout principal
public:
    askPopUp(QWidget* parent, std::string info);
    QPushButton* getButtonYes(){return yes;}
    QPushButton* getButtonNo(){return no;}
};

class popUpInfo : public QWidget{
    Q_OBJECT
private:
    QLabel* info;
    QVBoxLayout* layout;
public:
    popUpInfo(QWidget* parent, std::string info);
};

#endif //QTQUESTION_H
