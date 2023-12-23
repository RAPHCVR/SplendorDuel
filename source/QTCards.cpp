#include "QTCards.h"
#include "QTGame.h"
//#include <QDesktopWidget>
#include <QLabel>
#include <QtCore/qmath.h>
#include <QGraphicsProxyWidget>
#include <QDebug>
#include <QString>
#include <QDir>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QMessageBox>
#include <thread>
#include <chrono>

namespace Utility {

    QString toHexaDecimal(TokenColor c){
        switch (c) {
        case TokenColor::BLANC:
            return "#FFFFFF";
        case TokenColor::BLEU:
            return "#001F3F";
        case TokenColor::NOIR:
            return "#333333";
        case TokenColor::ROUGE:
            return "#990000";
        case TokenColor::VERT:
            return "#006400";
        default:
            break;
        }
    }

}


Carte::Carte(JewelryCard* jewelrycard, QWidget *parent)
    : jewelryCard(jewelrycard), QLabel(parent), status(Carte::notClickable){
    afficher();
}

Carte::~Carte() {}

void Carte::afficher() {
    QString imagePath = getImagePath();

    QPixmap carteImage(imagePath);

    // Vérifier si le chargement de l'image a réussi
    if (carteImage.isNull()) {
        qDebug() << "Erreur : Impossible de charger l'image " << imagePath;
        return;
    }

    setPixmap(QPixmap(imagePath));
    setScaledContents(true);
}

QString Carte::getImagePath() const {

    //Id de la carte pour récupérer son image
    int id = jewelryCard->getId();
    //Répertoire courant
    QString pathbeforeproject = QFileInfo(".").absolutePath();
    //Récupération de l'image
    QString imagePath = QString("%1/Cards/%2.png").arg(pathbeforeproject).arg(id);

    return imagePath;
}

bool Carte::isCardBuyable(Carte* card) const{
    return card->getStatus() == Carte::buyable;
}

bool Carte::isCardReservable(Carte* card) const{
    return card->getStatus() == Carte::reservable;
}

void Carte::mousePressEvent(QMouseEvent* event) {
    // Émet le signal clicked lorsque la carte est cliquée
    if (isCardBuyable(this) || isCardReservable(this)){
        emit clicked(this);
    }
}


QTPyramid::QTPyramid(QWidget *parent) : QWidget(parent), pyramidcard(Pyramid_Cards::getInstance()) {
    grille = new QGridLayout(this);
    afficher();

    // Obtenir l'écran principal
    QScreen* screen = QGuiApplication::primaryScreen();
    // Obtenir la géométrie de l'écran
    QRect screenGeometry = screen->geometry();
    // Utiliser les dimensions de l'écran pour définir la taille du plateau
    //setFixedSize(screenGeometry.width(), screenGeometry.height());
}

void QTPyramid::afficher(){

    std::vector<JewelryCard*> currentRow;
    int k;
    for(int j=0; j<3; j++){ //j représente les lignes
        if(j==0){currentRow = pyramidcard->getRow3(); k=3;}
        else if(j==1){ currentRow = pyramidcard->getRow2(); k=4;}
        else if(j==2){ currentRow = pyramidcard->getRow1(); k=5;}

        for(int i=0; i<k; i++){ //i représente les lignes
            Carte* currentCarte = new Carte(currentRow[i]);
            grille->addWidget(currentCarte, j, i);
            connect(currentCarte, &Carte::clicked, this, &QTPyramid::carteClicked);
        }

    }
}

void QTPyramid::ajouterCarte(int rowlevel){
    std::vector<JewelryCard*> currentRow;
    Carte* currentCarte;
    switch(rowlevel){ //rowlevel par rapport à la grille 0 -> level 3
    case 2:
        currentRow = pyramidcard->getRow1();
        if (currentRow.empty()) {
            return;
        }
        currentCarte = new Carte(currentRow.back());
        connect(currentCarte, &Carte::clicked, this, &QTPyramid::carteClicked);
        grille->addWidget(currentCarte, 2, 4);
        break;

    case 1:
        currentRow = pyramidcard->getRow2();
        if (currentRow.empty()) {
            return;
        }
        currentCarte = new Carte(currentRow.back());
        connect(currentCarte, &Carte::clicked, this, &QTPyramid::carteClicked);
        grille->addWidget(currentCarte, 1, 3);
        break;

    case 0:
        currentRow = pyramidcard->getRow3();
        if (currentRow.empty()) {
            return;
        }
        currentCarte = new Carte(currentRow.back());
        connect(currentCarte, &Carte::clicked, this, &QTPyramid::carteClicked);
        grille->addWidget(currentCarte, 0, 2);
        break;
    }

    grille->update();

}

int QTPyramid::retirerCarte(Carte* carte){

    int row, col, rowspan, columnspan;
    grille->getItemPosition(grille->indexOf(carte), &row, &col, &rowspan, &columnspan);

    // Retirez la carte du layout
    grille->removeWidget(carte);

    // Supprimez l'objet Carte après que la carte est retirée de la grille
    carte->deleteLater();

    // Mettez à jour la disposition pour refléter les modifications
    grille->update();

    int rowSize = 0;
    switch (row) {
    case 0:
        rowSize = 3;
        break;
    case 1:
        rowSize = 4;
        break;
    case 2:
        rowSize = 5;
        break;
    default:
        break;
    }
    // Déplacez les cartes à droite et comblez l'espace
    for (int j = col; j < rowSize - 1; j++) {
        QWidget* widget = grille->itemAtPosition(row, j + 1)->widget();
        if(widget){
            grille->removeWidget(widget);
            grille->addWidget(widget, row, j);
        }
    }


    grille->update();


    qDebug() << "Carte supprimée : " << carte->getJewelryCard()->getId() << " à la position [" << row << ", " << col << "]";


    return row;
}

void QTPyramid::carteClicked(Carte* carte) {
    qDebug() << "Carte cliquée : Level : " << carte->getJewelryCard()->getLevel() << " et id : " << carte->getJewelryCard()->getId();

    QMessageBox messageBox;
    messageBox.setText("Que voulez-vous faire avec cette carte?");

    QPushButton* acheterButton = messageBox.addButton("Acheter", QMessageBox::ActionRole);
    QPushButton* reserverButton = messageBox.addButton("Réserver", QMessageBox::ActionRole);
    QPushButton* annulerButton = messageBox.addButton("Annuler", QMessageBox::RejectRole);
    // Obtenez l'état de la carte
    Carte::CardStatus status = carte->getStatus();

    if (status == Carte::buyable && status == Carte::reservable) {
        acheterButton->setText("Acheter");
        reserverButton->setText("Réserver");
    } else if (status == Carte::buyable) {
        acheterButton->setText("Acheter");
        messageBox.removeButton(reserverButton);
    } else if (status == Carte::reservable) {
        reserverButton->setText("Réserver");
            messageBox.removeButton(acheterButton);
    } else {
        messageBox.setText("Aucune action possible avec cette carte.");
        messageBox.removeButton(acheterButton);
        messageBox.removeButton(reserverButton);
    }

    int choix = messageBox.exec();
    std::cout << choix << std::endl;
    if (messageBox.clickedButton() == acheterButton) {
        // Action "Acheter"
        if (status == Carte::buyable) {
            emit acheterCarteClicked(carte);
            //Partie qui gérait le back end (mtn dans QTGame)
            //int level = abs(row - 3); // Convertir la ligne en level
            //pyramidcard->Pyramid_Cards::drawCard(level);

        }
    } else if (messageBox.clickedButton() == reserverButton) {
        // Actions "Réserver" ou "Annuler"
        if (status == Carte::reservable) {
            std::cout << "Test carteClicked from Pioche" << std::endl;
            emit reserverCarteClicked(carte);
            //Partie qui gérait le back end (mtn dans QTGame)
            //int level = abs(row - 3); // Convertir la ligne en level
            //pyramidcard->Pyramid_Cards::drawCard(level);

        }
    }

}


void QTPyramid::updateAllCardStatus(Carte::CardStatus newStatus){

    for (int i = 0; i < grille->rowCount(); i++) {
        for (int j = 0; j < grille->columnCount(); j++) {
            if((i == 0 && j<3) || (i==1 && j<4) || (i == 2 && j<5)){
                QLayoutItem* item = grille->itemAtPosition(i, j);
                if(item){
                    Carte* carte = dynamic_cast<Carte*>(grille->itemAtPosition(i, j)->widget());
                    if (carte) {
                        carte->setStatus(newStatus);
                    }
                }
            }
        }
    }
}

QTPioche::QTPioche(Deck_level_one* Deck1, Deck_level_two* Deck2, Deck_level_three* Deck3, QWidget *parent)
    : Deck1(Deck1), Deck2(Deck2), Deck3(Deck3), status(PiocheStatus::notClickable){

    afficher();

}

QTPioche::~QTPioche() {}

void QTPioche::afficher() {

    QString imagePath = getImagePath();

    QPixmap piocheImage(imagePath);

    // Vérifier si le chargement de l'image a réussi
    if (piocheImage.isNull()) {
        qDebug() << "Erreur : Impossible de charger l'image " << imagePath;
        return;
    }

    setPixmap(QPixmap(imagePath));
    setScaledContents(true);
}

QString QTPioche::getImagePath() const {

    //Id de la carte pour récupérer son image
    int backnumber = 0;
    if (Deck1 != nullptr && Deck2 == nullptr && Deck3 == nullptr){ backnumber = 3100;}
    if (Deck2 != nullptr && Deck1 == nullptr && Deck3 == nullptr){ backnumber = 3200; std::cout << "Deck2-2" << std::endl;}
    if (Deck3 != nullptr && Deck1 == nullptr && Deck2 == nullptr){ backnumber = 3300; std::cout << "Deck3-3" << std::endl;}

    //Répertoire courant
    QString pathbeforeproject = QFileInfo(".").absolutePath();
    //Récupération de l'image
    QString imagePath = QString("%1/Cards/%2.png").arg(pathbeforeproject).arg(backnumber);

    return imagePath;
}

bool QTPioche::isCardReservable(QTPioche* pioche) const{
    return pioche->getStatus() == PiocheStatus::reservable;
}
void QTPioche::mousePressEvent(QMouseEvent* event) {
    // Émet le signal clicked lorsque la carte est cliquée
    if (isCardReservable(this)){
        emit clicked(this);
    }
}

QTRangeePioches::QTRangeePioches(QWidget *parent){

    grille = new QGridLayout(this);
    afficher();
}

void QTRangeePioches::afficher(){

    Deck_level_one* Deck1 = Deck_level_one::getInstance();
    Deck_level_two* Deck2 = Deck_level_two::getInstance();
    Deck_level_three* Deck3 = Deck_level_three::getInstance();
    QTPioche* currentPioche;
    for(int i=0; i<3; i++){
        if(i==0){ currentPioche = new QTPioche(nullptr, nullptr, Deck3); std::cout << "Deck3" << std::endl;}
        else if(i==1){ currentPioche = new QTPioche(nullptr, Deck2, nullptr); std::cout << "Deck2" << std::endl;}
        else if(i==2){ currentPioche = new QTPioche(Deck1, nullptr, nullptr); std::cout << "Deck1" << std::endl;}

        grille->addWidget(currentPioche, i, 0); //en une seule colonne
        connect(currentPioche, &QTPioche::clicked, this, &QTRangeePioches::piocheClicked);
    }

}

void QTRangeePioches::piocheClicked(QTPioche* pioche) {

    //qDebug() << "Pioche cliquée : Level : " << pioche->getDeck1()->getPioche().front()->getLevel() << endl;

    QMessageBox messageBox;
    messageBox.setText("Que voulez-vous faire ?");
    messageBox.addButton("Réserver la première carte de la pioche", QMessageBox::AcceptRole);
    messageBox.addButton("Annuler", QMessageBox::RejectRole);
    int choix = messageBox.exec();
    if (choix == QMessageBox::AcceptRole) {
        emit reserverCarteClicked(pioche);
    } else if (choix == QMessageBox::RejectRole) {

    }
}

void QTRangeePioches::updateAllPiocheStatus(QTPioche::PiocheStatus newStatus){

    for (int i = 0; i < grille->rowCount(); i++) {
        for (int j = 0; j < grille->columnCount(); j++) {
            QTPioche* pioche = dynamic_cast<QTPioche*>(grille->itemAtPosition(i, j)->widget());
            if (pioche) {
                pioche->setStatus(newStatus);
            }
        }
    }
}

QTCardRoyal::QTCardRoyal(RoyalCard* rc, QLabel* parent):royalcard(rc), QLabel(parent), status(QTCardRoyal::notClickable){
    afficher();
}

QTCardRoyal::~QTCardRoyal() {}

void QTCardRoyal::afficher(){
    QString imagePath = getImagePath();

    QPixmap carteImage(imagePath);

    // Vérifier si le chargement de l'image a réussi
    if (carteImage.isNull()) {
        qDebug() << "Erreur : Impossible de charger l'image " << imagePath;
        return;
    }

    setPixmap(QPixmap(imagePath));
    setScaledContents(true);

}

QString QTCardRoyal::getImagePath() const {

    //Id de la carte pour récupérer son image
    int id = royalcard->getId();
    std::cout << "Id : " << id << std::endl;
    //Répertoire courant
    QString pathbeforeproject = QFileInfo(".").absolutePath();
    //Récupération de l'image
    QString imagePath = QString("%1/Cards/%2.png").arg(pathbeforeproject).arg(id);

    return imagePath;
}

bool QTCardRoyal::isCardBuyable(QTCardRoyal* card) const{
    return card->getStatus() == QTCardRoyal::buyable;
}

void QTCardRoyal::mousePressEvent(QMouseEvent* event) {
    // Émet le signal clicked lorsque la carte est cliquée
    if (isCardBuyable(this)){
        emit clicked(this);
    }
}

QTBoardRoyal::QTBoardRoyal(QWidget *parent) : QWidget(parent), deckroyal(Deck_Royal::getInstance()) {
    grille = new QGridLayout(this);
    afficher();

    // Obtenir l'écran principal
    //QScreen* screen = QGuiApplication::primaryScreen();
    // Obtenir la géométrie de l'écran
    //QRect screenGeometry = screen->geometry();
    // Utiliser les dimensions de l'écran pour définir la taille du plateau
    //setFixedSize(screenGeometry.width(), screenGeometry.height());
}

void QTBoardRoyal::afficher(){

    int k = 0;
    for(int i=0; i<2; i++){ //i lignes
        for(int j = 0; j<2; j++){
            QTCardRoyal* currentcarte = new QTCardRoyal(deckroyal->getCards()[k]);
            grille->addWidget(currentcarte, i, j);
            connect(currentcarte, &QTCardRoyal::clicked, this, &QTBoardRoyal::carteClicked);
            k++;
        }
    }

}


void QTBoardRoyal::carteClicked(QTCardRoyal* carte){

    QMessageBox messageBox;
    messageBox.setText("Voulez-vous acheter cette carte Royale ?");
    messageBox.addButton("Oui", QMessageBox::AcceptRole);
    messageBox.addButton("Annuler", QMessageBox::RejectRole);
        int choix = messageBox.exec();
        if (choix == QMessageBox::AcceptRole) {
            emit acheterCarteClicked(carte);
        } else if (choix == QMessageBox::RejectRole) {

        }
}

void QTBoardRoyal::updateAllCardStatus(QTCardRoyal::CardRoyalStatus newStatus){

    for (int i = 0; i < grille->rowCount(); i++) {
        for (int j = 0; j < grille->columnCount(); j++) {
            QLayoutItem* item = grille->itemAtPosition(i, j);
            if(item){
                QTCardRoyal* carte = dynamic_cast<QTCardRoyal*>(grille->itemAtPosition(i, j)->widget());
                if (carte) {
                    carte->setStatus(newStatus);
                }
            }
        }
    }
}

void QTBoardRoyal::retirerCarte(QTCardRoyal* carte){

    int row, col, rowspan, columnspan;
    grille->getItemPosition(grille->indexOf(carte), &row, &col, &rowspan, &columnspan);
    int rowCount = grille->rowCount();
    int columnCount = grille->columnCount();

    // Retirez la carte du layout
    grille->removeWidget(carte);

    // Supprimez l'objet Carte après que la carte est retirée de la grille
    carte->deleteLater();


    // Mettez à jour la disposition pour refléter les modifications
    grille->update();

    // Déplacez les cartes à droite et comblez l'espace
    for (int i=row; i<2-rowCount; ++i){
        for(int j=col; j<2-columnCount; ++j){

            QWidget* widget = grille->itemAtPosition(i+1, j+1)->widget();
            if(widget){
                grille->removeWidget(widget);
                grille->addWidget(widget, i, j);
            }
        }
    }


    grille->update();


    //qDebug() << "Carte supprimée : " << carte->getJewelryCard()->getId() << " à la position [" << row << ", " << col << "]";
}

Carte& QTPyramid::getCarte(int row, int col){
    //take the row and col of the card and return the card
    //if row = 3, row = 0, if row = 2, row = 1, if row = 1, row = 2
    if(row == 3){row = 0;}
    else if(row == 2){row = 1;}
    else if(row == 1){row = 2;}
    return *dynamic_cast<Carte*>(grille->itemAtPosition(row, col)->widget());
}

QTPioche& QTRangeePioches::getPioche(int row){
    if(row == 3){row = 0;}
    else if(row == 2){row = 1;}
    else if(row == 1){row = 2;}
    return *dynamic_cast<QTPioche*>(grille->itemAtPosition(row, 0)->widget());
}

QTCardRoyal& QTBoardRoyal::getCarte(int indice){
    return *dynamic_cast<QTCardRoyal*>(grille->itemAtPosition(indice/2, indice%2)->widget());
}