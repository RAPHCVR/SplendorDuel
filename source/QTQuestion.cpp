//
// Created by utcpret on 15/12/2023.
//

#include "QTQuestion.h"

askPopUp::askPopUp(QWidget* parent, std::string info) : QWidget(parent){ //PopUp pour faire des validations
    yes = new QPushButton("oui"); //Bouton Oui
    no = new QPushButton("non"); //Bouton Non

    std::string tempPhrase = texte1 + info + texte3; //Def la question qui sera poser
    this->info = new QLabel;
    this->info->setText(QString::fromStdString(tempPhrase)); //Def le texte sur le QLabel

    boutonLayout = new QHBoxLayout;
    layout = new QVBoxLayout;

    boutonLayout->addWidget(yes);
    boutonLayout->addWidget(no);

    layout -> addWidget(this->info);
    layout -> addLayout(boutonLayout);

    setLayout(layout);
}

popUpInfo::popUpInfo(QWidget* parent, std::string info) : QWidget(parent){ //PopUp pour montrer des infos
    this->info = new QLabel;
    this->info->setText(QString::fromStdString(info));

    layout = new QVBoxLayout;
    layout -> addWidget(this->info);

    setLayout(layout);
}