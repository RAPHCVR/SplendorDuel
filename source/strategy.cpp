#include "Jeton.h"
#include "Cards.h"
#include "joueur.h"
#include "strategy.h"
#include "Controller.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>

// Celine
// nombre aleatoire entre min et max
int AiStrategy::choicemaker(int min, int max){
    int random;
    // générateur de nombres aléatoires avec le temps actuel
    srand(time(0));

    // nombre aléatoire dans l'intervalle [min, max]
    random = std::rand() % (max - min + 1) + min;
    return random;
}

int HumanStrategy::choicemaker(int min, int max) {
    unsigned int choice;
    std::cout << "Veuillez choisir entre " << min << " et " << max << std::endl;
    std::cin >> choice;
    while (choice < min || choice > max){
        std::cout << "Veuillez choisir entre " << min << " et " << max << std::endl;
        std::cin >> choice;
    }
    return choice;
}

// Celine
// permet a l'ia de choisir des jetons sur le plateau
std::vector<std::pair<int, int>> AiStrategy::choseTokensToTake(){
    std::vector<std::pair<int, int>> chosenTokens;
    Board *boardpoint = Board::getInstance();
    Board& board = *boardpoint;
    auto isTokenAvailable = [&](size_t i, size_t j) -> bool {
        return i < 5 && j < 5 && !board.isCellEmpty(i, j) && board.getCellColor(i,j) != TokenColor::OR;
    };

    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 5; ++j) {
            if (!isTokenAvailable(i, j)) continue;

            std::vector<std::pair<int, int>> tempTokens;

            // Horizontal, Vertical, and Diagonal Checks
            for (int dx : {0, 1}) {
                for (int dy : {0, 1}) {
                    if (dx == 0 && dy == 0) continue; // Skip if both dx and dy are zero

                    tempTokens.clear();
                    size_t x = i, y = j;

                    while (isTokenAvailable(x, y)) {
                        tempTokens.emplace_back(x, y);
                        if (tempTokens.size() == 3) {
                            return tempTokens; // Return if three tokens are found aligned
                        }
                        x += dx;
                        y += dy;
                    }

                    if (tempTokens.size() > 1) {
                        chosenTokens = tempTokens; // Store if more than one token is found aligned
                    }
                }
            }

            // If only one token is available and no better option has been found
            if (chosenTokens.empty()) {
                chosenTokens.emplace_back(i, j);
            }
        }
    }

    return chosenTokens;
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
    int randomIndex = choicemaker(0, chosableColors.size()-1);
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
    int nbOptionalAction = choicemaker(0,2);

    // lesquelles parmis 
    // utiliser privilege = prendre jeton (pas or) / remplir plateau + prendre un privilege

    // on ne realise pas d'action optionnelle
    if (nbOptionalAction == 0){
        AIsOptionalActions.push_back(OptionalActions::Empty);
    }
    //on realise une action optionnelle
    else if(nbOptionalAction==1){
        // choisir laquelle
        int whichOpAction = choicemaker(0,1);
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
    int whichCompulsoryAction = choicemaker(0,2);
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

const Token& AiStrategy::chooseToken(Board&board, Player&player, std::vector<std::pair<int, int>>* tokenIndexes) {
    Board::BoardIterator it = board.iterator();
    while (it.hasNext()) {
        const Token* token = it.next();
        if (token!=nullptr) {
            if (token->getColor()!=TokenColor::OR) {
                int col;
                int row;
                if( it.getCol() == 0){
                    col = 4;
                    row = it.getRow()-1;
                }
                else {
                    col = it.getCol()-1;
                    row = it.getRow();
                }
                token = &board.takeToken(row, col);
                player.addToken(*token);
                return *token;
            }
        }
    }
    return *new Token(TokenColor::None);
}


const Token& HumanStrategy::chooseToken(Board&board, Player&player, std::vector<std::pair<int, int>>* tokenIndexes) {
        if (!board.isEmpty()) {
            std::cout << "Info du joueur : \n";
            std::cout << player << std::endl;
            unsigned int x = 0;
            unsigned int y = 0;
            bool stop = false;
            if (tokenIndexes!= nullptr && !tokenIndexes->empty()) {
                std::cout << "Veuillez choisir un jeton ('STOP' pour arreter)" << std::endl;
                board.showBoard();
                std::cout << "Veuillez choisir entre 1 et 5" << std::endl;
                std::string rep;
                std::cin >> rep;
                if(rep != "STOP") {
                    x = std::stoi(rep);
                    if (x < 1 || x > 5) {
                        throw TokenException("Indice incorrect");
                    }
                    y = player.getStrategy()->choicemaker(1, 5);
                }
                else {
                    stop = true;
                }
            }
            else {
                std::cout << "Veuillez choisir un jeton" << std::endl;
                board.showBoard();
                x = player.getStrategy()->choicemaker(1, 5);
                y = player.getStrategy()->choicemaker(1, 5);
            }
            if (!stop) {
                if(Board::getInstance()->isCellEmpty(x-1,y-1))
                    throw TokenException("L'emplacement ne contient pas de jeton\n");
                if(Board::getInstance()->CellColor(x-1, y-1,TokenColor::OR))
                    throw TokenException("Impossible de prendre un jeton or\n");
                if (tokenIndexes!= nullptr) {
                    if (tokenIndexes->empty()) {
                        tokenIndexes->emplace_back(x-1,y-1);
                    }
                    else {
                        tokenIndexes->emplace_back(x-1,y-1);
                        if (not(areCoordinatesAlignedAndConsecutive(tokenIndexes))) {
                            throw TokenException("Les jetons ne sont pas alignés ou ne sont pas consécutifs");
                        }
                    }
                }
                const Token& token = board.takeToken(x-1, y-1);
                player.addToken(token);
                return token;
            }
            else {
                return *new Token(TokenColor::None);
            }
        }
        else {
            std::cout << "Plus de Jetons sur le plateau" << std::endl;
            return *new Token(TokenColor::None);
        }
}

void AiStrategy::chooseGoldenToken(Board&board, Player&player) {
        Board::getInstance()->showBoard();
        Board::BoardIterator it = board.iterator();
        while (it.hasNext()) {
            const Token* token = it.next();
            if (token!=nullptr) {
                if (token->getColor()==TokenColor::OR) {
                    int col;
                    int row;
                    if(it.getCol() == 0){
                        col = 4;
                        row = it.getRow()-1;
                    }
                    else {
                        col = it.getCol()-1;
                        row = it.getRow();
                    }
                    token = &board.takeToken(row, col);
                    player.addToken(*token);
                    return;
                }
            }
        }
}


void HumanStrategy::chooseGoldenToken(Board&board, Player&player) {
        std::cout << "Veuillez choisir un jeton OR" << std::endl;
        board.showBoard();
        unsigned int x = player.getStrategy()->choicemaker(1, 5);
        unsigned int y = player.getStrategy()->choicemaker(1, 5);
        // joueur humain
        if(board.isCellEmpty(x-1,y-1))
            throw TokenException("L'emplacement ne contient pas de jeton\n");
        if(not(board.CellColor(x-1, y-1,TokenColor::OR)))
            throw TokenException("Vous devez choisir un jeton or\n");
        const Token& token = board.takeToken(x-1, y-1);
        player.addToken(token);
}