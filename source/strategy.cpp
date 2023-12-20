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


std::vector<const Token*> AiStrategy::choseTokensToTake(){
    Board::BoardIterator it = iterator();
    // parcours du plateau
    while (it.hasNext()) {
        const Token* firstToken = it.next();
        // essayer de selectionner 3 jetons
        for (int count = 3; count >= 1; --count) {
            //std::vector<const Token*> selectedTokens;
            std::vector<std::pair<int, int>> selectedCoordinates;

            // ajouter le premier jeton
            //selectedTokens.push_back(firstToken);
            selectedCoordinates.emplace_back(it.getRow(), it.getCol());

            // essayer d'ajouter les jetons restants
            for (int i = 1; i < count; ++i) {
                if (it.hasNext()) {
                    const Token* nextToken = it.next();
                    //selectedTokens.push_back(nextToken);
                    selectedCoordinates.emplace_back(it.getRow(), it.getCol());

                    // verif si les coordonnees sont alignees et consecutives
                    if (!areCoordinatesAlignedAndConsecutive(&selectedCoordinates)) {
                        // si non on reinit la selection
                        //selectedTokens.clear();
                        selectedCoordinates.clear();
                        it.reset();
                        break;
                    }
                } else {
                    // si plus de jetons disponibles -> reinit la selection
                    //selectedTokens.clear();
                    selectedCoordinates.clear();
                    it.reset();
                    break;
                }
            }

            // si la selection est ok -> renvoyer les jetons selectionnes
            if (selectedCoordinates.size() == count) {
                return selectedCoordinates;
            }
        }
    }

    // pas de selection valide --> essayer seulement avec 2 jetons
    it.reset(); // reinit l'iterateur

    while (it.hasNext()) {
        const Token* firstToken = it.next();

        // essayer de selectionner 2 jetons
        for (int count = 2; count >= 1; --count) {
            //std::vector<const Token*> selectedTokens;
            std::vector<std::pair<int, int>> selectedCoordinates;

            // ajouter le premier jeton
            //selectedTokens.push_back(firstToken);
            selectedCoordinates.emplace_back(it.getRow(), it.getCol());

            // essayer d'ajouter les jetons restants
            for (int i = 1; i < count; ++i) {
                if (it.hasNext()) {
                    const Token* nextToken = it.next();
                    //selectedTokens.push_back(nextToken);
                    selectedCoordinates.emplace_back(it.getRow(), it.getCol());

                    // verif si les coord sont alignees et consecutives
                    if (!areCoordinatesAlignedAndConsecutive(&selectedCoordinates)) {
                        // si non -> reinit la selection
                        //selectedTokens.clear();
                        selectedCoordinates.clear();
                        it.reset();
                        break;
                    }
                } else {
                    // si plus de jetons dispo -> reinit la sélection
                    //selectedTokens.clear();
                    selectedCoordinates.clear();
                    it.reset();
                    break;
                }
            }

            // la sélection est valide -> renvoyer les jetons a piocher
            if (selectedCoordinates.size() == count) {
                return selectedCoordinates;
            }
        }
    }

    // pas trouve 2 jetons consecutifs, on en prend qu'un
    it.reset(); // reinit l'iterateur

    while (it.hasNext()) {
        const Token* currentToken = it.next();
        selectedCoordinates.emplace_back(it.getRow(), it.getCol());
        return {currentToken};
        
    }

    // pas de jeton piochable sur la board
    return {}; 
}

std::vector<const Token*> HumanStrategy::choseTokensToTake(){
    int x;
    int y;
    while (x!=1 || x!=2 || x!=3 || x!=4 || x!=5){
        std::cout << "Coordonnée x du jeton (entre 1 et 5) : ";
        std::cin >> x;
    }
    
    while (x!=1 || x!=2 || x!=3 || x!=4 || x!=5){
        std::cout << "Coordonnée y du jeton (entre 1 et 5) : ";
        std::cin >> y;
    }
    
}


TokenColor HumanStrategy::choseTokenColor(std::vector<TokenColor>& chosableColors){
    while (true) {
        // Display available colors
        std::cout << "Couleurs possibles : ";
        for (const auto& color : chosableColors) {
            std::cout << static_cast<int>(color) << ": " << color << ";\n";
        }

        int choice;
        std::cout << "Entrez le nombre correspondant a la couleur choisie : ";
        std::cin >> choice;

        auto iter = std::find(chosableColors.begin(), chosableColors.end(), static_cast<TokenColor>(choice));
        if (iter != chosableColors.end()) {
            return static_cast<TokenColor>(choice);
        } else {
            std::cout << "Couleur invalide, veuillez choisir une couleur parmis la liste." << std::endl;
        }
    }
}



TokenColor AiStrategy::choseTokenColor(std::vector<TokenColor>& chosableColors){
    int randomIndex = random(0, chosableColors.size()-1);
    return chosableColors[randomIndex];
}


std::vector<OptionalActions> HumanStrategy::choseOptionalActions(){
    std::vector<OptionalActions> HumanOptionalActions;
    OptionalActions choice;
    int nbOptionalActions;
    std::cout << "Combien d'action(s) optionnelle(s) souhaitez vous faire (0, 1 ou 2) : ";
    std::cin >> nbOptionalActions;
    if(nbOptionalActions==0){
        return {};
    }
    else{
        for(int i = 0; i < nbOptionalActions; i++){
            while(choice!=OptionalActions::FillBoard||choice!=OptionalActions::UsePrivileges ){
                std::cout << "Entrez une action valide (FillBoard ou UsePrivileges) : ";
                std::cin >> choice;
            }
            HumanOptionalActions.push_back(choice);

        }
        return HumanOptionalActions;
    }

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


std::vector<CompulsoryActions> HumanStrategy::choseCompulsoryActions(){
    std::vector<CompulsoryActions> HumanCompulsoryActions;
    CompulsoryActions choice;
    std::cout << "Entrez une action valide (FillBoard ou UsePrivileges) : ";
    while(choice!=CompulsoryActions::BuyCard || choice!=CompulsoryActions::ReserveCard || choice!=CompulsoryActions::TakeCoins){
        std::cout << "Entrez une action valide (BuyCard, ReserveCard ou TakeCoins) : ";
        std::cin >> choice;
    }
    HumanOptionalActions.push_back(choice);
    return HumanOptionalActions;
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

