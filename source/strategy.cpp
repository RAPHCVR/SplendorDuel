#include "Jeton.h"
#include "Cards.h"
#include "joueur.h"
#include "strategy.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>

// Celine
// nombre aleatoire entre min et max
int AiStrategy::random(int min, int max){
    int random;
    // générateur de nombres aléatoires avec le temps actuel
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // nombre aléatoire dans l'intervalle [min, max]
    random = std::rand() % (max - min + 1) + min;
    return random;
}

// Celine
// permet a l'ia de choisir des jetons sur le plateau
std::vector<std::pair<int, int>> AiStrategy::choseTokensToTake(){
    Board::BoardIterator it = Board::getInstance()->iterator();
    std::vector<std::pair<int, int>> selectedCoordinates;

    // parcours du plateau
    while (it.hasNext()) {
        const Token* firstToken = it.next();
        // essayer de selectionner 3 jetons
        for (int count = 3; count >= 1; count--) {
            //std::vector<const Token*> selectedTokens;

            // ajouter le premier jeton
            //selectedTokens.push_back(firstToken);
            selectedCoordinates.emplace_back(it.getRow(), it.getCol());

            // essayer d'ajouter les jetons restants
            for (int i = 1; i < count; i++) {
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
        for (int count = 2; count >= 1; count--) {
            //std::vector<const Token*> selectedTokens;
            std::vector<std::pair<int, int>> selectedCoordinates;

            // ajouter le premier jeton
            //selectedTokens.push_back(firstToken);
            selectedCoordinates.emplace_back(it.getRow(), it.getCol());

            // essayer d'ajouter les jetons restants
            for (int i = 1; i < count; i++) {
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

            // la selection est valide -> renvoyer les jetons a piocher
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
        return {selectedCoordinates};
        
    }

    // pas de jeton piochable sur la board
    return {}; 
}


// Celine
// permet a l'huamin de choisir des jetons sur le plateau
std::vector<std::pair<int, int>> HumanStrategy::choseTokensToTake(){
    int x;
    int y;
    int nbOfToken = 0;
    while(nbOfToken != 1 && nbOfToken != 2 && nbOfToken != 3){
        std::cout << "Nombre de jetons (entre 1 et 3) que vous souhaitez prendre : ";
        std::cin >> nbOfToken;
    }
    std::vector<std::pair<int, int>> selectedTokensCoordinates;
    for(int i = 0; i < nbOfToken; i++){
        while (x!=1 && x!=2 && x!=3 && x!=4 && x!=5){
        std::cout << "Coordonnée x du jeton (entre 1 et 5) : ";
        std::cin >> x;
        }
    
        while (y!=1 && y!=2 && y!=3 && y!=4 && y!=5){
            std::cout << "Coordonnée y du jeton (entre 1 et 5) : ";
            std::cin >> y;
        }

        selectedTokensCoordinates.emplace_back(x, y);

        // reinit les coordonnes
        x=0;
        y=0;

    }
    return selectedTokensCoordinates;
    
}


// Celine
// permet a l'humain de choisir une couleur parmis les couleurs proposes
TokenColor HumanStrategy::choseTokenColor(std::vector<TokenColor>& chosableColors){
    while(true){   
        // afficher les couleurs
        std::cout << "Couleurs possibles : \n";
        for (const auto& color : chosableColors) {
            std::cout << toString(color) << " ";
        }

        // choix de la couleur
        std::string choice;
        std::cout << "\nEntrez la couleur choisie (respecter la casse): ";
        std::cin >> choice;

        // test si couleur valide
        auto iter = std::find(chosableColors.begin(), chosableColors.end(), toTokenColor(choice));
        if (iter != chosableColors.end()) {
            return toTokenColor(choice);
        } else {
            std::cout << "Couleur invalide, veuillez choisir une couleur parmis la liste." << std::endl;
        }
    }
}



// Celine
// permet a l'IA de choisir une couleur parmis les couleurs proposes
TokenColor AiStrategy::choseTokenColor(std::vector<TokenColor>& chosableColors){
    int randomIndex = random(0, chosableColors.size()-1);
    return chosableColors[randomIndex];
}

// Celine
// permet a l'humain d'entrer les actions optionnelles qu'il souhaite realiser
std::vector<OptionalActions> HumanStrategy::choseOptionalActions(){
    std::vector<OptionalActions> HumanOptionalActions;
    int choice = 0;
    int nbOptionalActions = -1;

    while(nbOptionalActions != 0 && nbOptionalActions != 1 && nbOptionalActions != 2){
        std::cout << "Combien d'action(s) optionnelle(s) souhaitez vous faire (0, 1 ou 2) : ";
        std::cin >> nbOptionalActions;
    }
    if(nbOptionalActions==0){
        HumanOptionalActions.push_back(OptionalActions::Empty);
        return HumanOptionalActions;
    }
    else if(nbOptionalActions==1){
        while(choice!=1 && choice!=2){
            std::cout << "Entrez l'action que vous souhaitez faire (1 pour remplir le plateau ou 2 pour utiliser vos privileges) : ";
            std::cin >> choice;
        }
        if(choice == 1){
            HumanOptionalActions.push_back(OptionalActions::FillBoard);
            return HumanOptionalActions;
        }
        else{
            HumanOptionalActions.push_back(OptionalActions::UsePrivileges);
            return HumanOptionalActions;
        }
    }
    else{
        HumanOptionalActions.push_back(OptionalActions::FillBoard);
        HumanOptionalActions.push_back(OptionalActions::UsePrivileges);
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

// Celine
// permet de choisir l'action obligatoire que fait l'humain
CompulsoryActions HumanStrategy::choseCompulsoryAction(){
    int choice = 0;
    while(choice!=1 && choice!=2 && choice!=3){
        std::cout << "Entrez l'action obligatoire que vous souhaitez realiser. \n1 pour acheter une carte, 2 pour reserver une carte ou 3 pour prendre des jetons : ";
        std::cin >> choice;
    }
    if(choice==1){
        return CompulsoryActions::BuyCard;
    }
    else if(choice == 2){
        return CompulsoryActions::ReserveCard;
    }
    else{
        return CompulsoryActions::TakeCoins;
    }
}

// Celine
// permet de choisir l'action obligatoire que fait l'ia
CompulsoryActions AiStrategy::choseCompulsoryAction(){
    CompulsoryActions AIsCompulsoryAction;
    int whichCompulsoryAction = random(0,2);
    // prendre entre un et trois jetons
    if(whichCompulsoryAction == 0){
        return CompulsoryActions::BuyCard;
    }
    // reserver une carte
    else if(whichCompulsoryAction == 1){
        return CompulsoryActions::ReserveCard;
    }
    // acheter carte
    else{
        return CompulsoryActions::TakeCoins;
    }

}
