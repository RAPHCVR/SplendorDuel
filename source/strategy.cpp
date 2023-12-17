#include "Jeton.h"
#include "Cards.h"
#include "joueur.h"
#include "strategy.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>


int AiStrategy::random(int min, int max){
    int random;
    // générateur de nombres aléatoires avec le temps actuel
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // nombre aléatoire dans l'intervalle [min, max]
    random = std::rand() % (max - min + 1) + min;
    return random;
}


// Celine
// permet de choisir le nombre d'actions optionnelles que fait l'ia
// et lesquelles il fait 
std::vector<OptionalActions> AiStrategy::choseOptionalActions(){
    std::vector<OptionalActions> AIsOptionalActions;
    // combien d'action optionnelles (entre 0 et 2)
    int nbOptionalAction = random(0,2);

    // lesquelles parmis 
    // utiliser privilege = prendre jeton (pas or) / remplir plateau + prendre un privilege

    // on ne realise pas d'action optionnelle
    if (nbOptionalAction == 0){
        AIsOptionalActions.push_back(OptionalActions::Empty);
    }
    //on realise une action optionnelle
    else if(nbOptionalAction==1){
        // choisir laquelle
        int whichOpAction = random(0,1);
        whichOpAction == 0? AIsOptionalActions.push_back(OptionalActions::UsePrivileges) : AIsOptionalActions.push_back(OptionalActions::FillBoard);
    }
    else{
        // faire les deux actions
        AIsOptionalActions.push_back(OptionalActions::UsePrivileges);
        AIsOptionalActions.push_back(OptionalActions::FillBoard);
    }
    return AIsOptionalActions;
}

// Celine
// permet de choisir l'action optionnelle que fait l'ia
std::vector<CompulsoryActions> AiStrategy::choseCompulsoryActions(){
    std::vector<CompulsoryActions> AIsCompulsoryAction;
    int nbCompulsoryActions = random(0,2);
    // prendre entre un et trois jetons
    if(nbCompulsoryActions == 0){
        AIsCompulsoryAction.push_back(CompulsoryActions::TakeCoins);
    }
    // reserver une carte
    else if(nbCompulsoryActions == 1){
        AIsCompulsoryAction.push_back(CompulsoryActions::ReserveCard);
    }
    else{
        AIsCompulsoryAction.push_back(CompulsoryActions::BuyCard);
    }

    return AIsCompulsoryAction;
}



std::vector<const Token*> AiStrategy::choseTokensToTake(){
    Board::BoardIterator it = iterator();
    // parcours du plateau
    while (it.hasNext()) {
        const Token* firstToken = it.next();
        // essayer de selectionner 3 jetons
        for (int count = 3; count >= 1; --count) {
            std::vector<const Token*> selectedTokens;
            std::vector<std::pair<int, int>> selectedCoordinates;

            // ajouter le premier jeton
            selectedTokens.push_back(firstToken);


            // !!!!!!!!!!!!!
            // it.getRow(), it.getCol() --> demander a raph comment y acceder
            selectedCoordinates.emplace_back(it.getRow(), it.getCol());




            // essayer d'ajouter les jetons restants
            for (int i = 1; i < count; ++i) {
                if (it.hasNext()) {
                    const Token* nextToken = it.next();
                    selectedTokens.push_back(nextToken);


                    // !!!!!!!!!!!!!
                    // it.getRow(), it.getCol() --> demander a raph comment y acceder
                    selectedCoordinates.emplace_back(it.getRow(), it.getCol());



                    // verif si les coordonnees sont alignees et consecutives
                    if (!areCoordinatesAlignedAndConsecutive(&selectedCoordinates)) {
                        // si non on reinit la selection
                        selectedTokens.clear();
                        selectedCoordinates.clear();
                        it.reset();
                        break;
                    }
                } else {
                    // si plus de jetons disponibles -> reinit la selection
                    selectedTokens.clear();
                    selectedCoordinates.clear();
                    it.reset();
                    break;
                }
            }

            // si la selection est ok -> renvoyer les jetons selectionnes
            if (selectedCoordinates.size() == count) {
                return selectedTokens;
            }
        }
    }

    // pas de selection valide --> essayer seulement avec 2 jetons
    it.reset(); // reinit l'iterateur

    while (it.hasNext()) {
        const Token* firstToken = it.next();

        // essayer de selectionner 2 jetons
        for (int count = 2; count >= 1; --count) {
            std::vector<const Token*> selectedTokens;
            std::vector<std::pair<int, int>> selectedCoordinates;

            // ajouter le premier jeton
            selectedTokens.push_back(firstToken);
            selectedCoordinates.emplace_back(it.getRow(), it.getCol());

            // essayer d'ajouter les jetons restants
            for (int i = 1; i < count; ++i) {
                if (it.hasNext()) {
                    const Token* nextToken = it.next();
                    selectedTokens.push_back(nextToken);
                    selectedCoordinates.emplace_back(it.getRow(), it.getCol());

                    // verif si les coord sont alignees et consecutives
                    if (!areCoordinatesAlignedAndConsecutive(&selectedCoordinates)) {
                        // si non -> reinit la selection
                        selectedTokens.clear();
                        selectedCoordinates.clear();
                        it.reset();
                        break;
                    }
                } else {
                    // si plus de jetons dispo -> reinit la sélection
                    selectedTokens.clear();
                    selectedCoordinates.clear();
                    it.reset();
                    break;
                }
            }

            // la sélection est valide -> renvoyer les jetons a piocher
            if (selectedCoordinates.size() == count) {
                return selectedTokens;
            }
        }
    }

    // pas trouve 2 jetons consecutifs, on en prend qu'un
    it.reset(); // reinit l'iterateur

    while (it.hasNext()) {
        const Token* currentToken = it.next();

        // verif si le jeton est piochable
        if (areCoordinatesAlignedAndConsecutive({std::make_pair(it.getRow(), it.getCol())})) {
            return {currentToken};
        }
    }

    // pas de jeton piochable sur la board
    return {}; 
}


TokenColor AiStrategy::choseTokenColor(){
    int tokenChoice = random(0,6);
    switch (tokenChoice){
    case 0:
        return TokenColor::BLEU;
        break;
    case 1:
        return TokenColor::BLANC;
        break;
    case 2:
        return TokenColor::VERT;
        break;
    case 3:
        return TokenColor::NOIR;
        break;
    case 4:
        return TokenColor::ROUGE;
        break;
    case 5:
        return TokenColor::PERLE;
        break;
    case 6:
        return TokenColor::OR;
        break;
    
    default:
        break;
    }
    
}

vector<OptionalActions> HumanStrategy::choseOptionalActions(){
    std::vector<OptionalActions> HumanOptionalActions;
    OptionalActions choice;
    int nbOptionalActions;
    cout << "Combien d'action(s) optionnelle(s) souhaitez vous faire (0, 1 ou 2) : ";
    cin >> nbOptionalActions;
    if(nbOptionalActions==0){
        return {};
    }
    else{
        for(int i = 0; i < nbOptionalActions; i++){
            while(choice!=OptionalActions::FillBoard||choice!=OptionalActions::UsePrivileges ){
                cout << "Entrez une action valide (FillBoard ou UsePrivileges) : ";
                cin >> choice;
            }
            HumanOptionalActions.push_back(choice);

        }
        return HumanOptionalActions;
    }

}


vector<CompulsoryActions> HumanStrategy::choseCompulsoryActions(){
    std::vector<CompulsoryActions> HumanCompulsoryActions;
    CompulsoryActions choice;
    cout << "Entrez une action valide (FillBoard ou UsePrivileges) : ";
    while(choice!=CompulsoryActions::BuyCard || choice!=CompulsoryActions::ReserveCard || choice!=CompulsoryActions::TakeCoins){
        cout << "Entrez une action valide (BuyCard, ReserveCard ou TakeCoins) : ";
        cin >> choice;
    }
    HumanOptionalActions.push_back(choice);
    return HumanOptionalActions;
}




TokenColor HumanStrategy::choseTokenColor(){
    TokenColor color;
    while(color!=TokenColor::BLEU || color!=TokenColor::BLANC || color!=TokenColor::VERT || color!=TokenColor::NOIR || color!=TokenColor::ROUGE || color!=TokenColor::PERLE || color!=TokenColor::OR){
        cout << "Entrez une couleur valide (BLEU, BLANC, VERT, NOIR, ROUGE, PERLE, OR) : ";
        cin >> color;
    }
    return color;
        
}

