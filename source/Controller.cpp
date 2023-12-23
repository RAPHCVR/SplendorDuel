//
// Created by world on 03/12/2023.
//

#include "Controller.h"
#include "strategy.h"

std::string toString(OptionalActions a) {
    switch (a) {
        case OptionalActions::UsePrivileges: return "UsePrivileges";
        case OptionalActions::FillBoard: return "FillBoard";
        case OptionalActions::Empty: return "Empty";
        default: throw std::invalid_argument("Action inconnue");
    }
}

OptionalActions toOptionalActions(std::string s) {
    if (s==("UsePrivileges")) {
        return OptionalActions::UsePrivileges;
    }
    else if (s==("FillBoard")) {
        return OptionalActions::FillBoard;
    }
    else if (s==("Empty")) {
        return OptionalActions::Empty;
    }
}

std::string toString(CompulsoryActions a) {
    switch (a) {
        case CompulsoryActions::TakeCoins: return "TakeCoins";
        case CompulsoryActions::ReserveCard: return "ReserveCard";
        case CompulsoryActions::BuyCard: return "BuyCard";
        default: throw std::invalid_argument("Action inconnue");
    }
}

CompulsoryActions toCompulsoryActions(std::string s) {
    if (s==("TakeCoins")) {
        return CompulsoryActions::TakeCoins;
    }
    else if (s==("ReserveCard")) {
        return CompulsoryActions::ReserveCard;
    }
    else if (s==("BuyCard")) {
        return CompulsoryActions::BuyCard;
    }
}


Controller::Controller(const std::string& statut_partie, std::string pseudo1, std::string pseudo2, Type type1, Type type2) {
    auto* director = new Director();
    //A MODIF: Sauvegarde de la partie/Nouvelle partie

    if (statut_partie == "New") {
        GameBuilder* builder = new GameBuilder();
        director->set_builder(builder);
        director->BuildGame(pseudo1, type1, pseudo2, type2);
        Game* p = builder->GetProduct();
        delete director;
        game = p;
        srand(time(0));
        unsigned int i = rand() % 2;
        currentPlayer= game->getPlayer(i);
        getopposingPlayer().addPrivilege(game->getGameTable().getBoard().takePrivilege());
    }
    else {
        GameSaveBuilder* builder = new GameSaveBuilder();
        director->set_builder(builder);
        director->BuildSaveGame(pseudo1, type1, pseudo2, type2);
        Game* p = builder->GetProduct();
        delete director;
        game = p;
        game->getPlayer(0)->getName();
        currentPlayer= game->getPlayer(0);
        retrieveCurrentPlayer(*this);
    }
}

void Controller::setCurrentPlayer(unsigned int n) {
    currentPlayer = game->getPlayer(n);
}

void Controller::changeCurrentPlayer() {
    if (currentPlayer == game->getPlayer(0)) {
        currentPlayer = game->getPlayer(1);
    } else {
        currentPlayer = game->getPlayer(0);
    }
}

std::vector<OptionalActions> Controller::getOptionalActions(const Game &game, Player &player) const {
    std::vector<OptionalActions> result;
    if (player.getNbPrivilege() > 0 && not (game.getGameTable().getBoard().isEmpty())){
        result.push_back(OptionalActions::UsePrivileges);
    }
    if (not (game.getGameTable().getBag().isEmpty())){
        result.push_back(OptionalActions::FillBoard);
    }
    result.push_back(OptionalActions::Empty);
    return result;
}

std::vector<CompulsoryActions> Controller::getCompulsoryActions(const Game &game, Player &player) const{
    std::vector<CompulsoryActions> result;
    // On verifie si y a des jetons et que ce n'est pas que des jetons Gold
    if ((not (game.getGameTable().getBoard().isEmpty())) and (not game.getGameTable().getBoard().containsOnly(TokenColor::OR))){
        result.push_back(CompulsoryActions::TakeCoins);
    }

    // On verifie si on peut reserver : on verifie si on a au moins un jeton or et que les paquets ne sont pas vides
    if (game.getGameTable().getBoard().hasTokenOfColor(TokenColor::OR)){
        if (((not (game.getGameTable().getPyramid().isEmpty(1))) || (not (game.getGameTable().getPyramid().isEmpty(2))) || (not (game.getGameTable().getPyramid().isEmpty(3)))) && (player.canReserveCard())){
            result.push_back(CompulsoryActions::ReserveCard);
        }
    }

    // On verifie si le joueur peut acheter au moins une carte
    bool hasBeenAdded = false;
    for (auto card : game.getGameTable().getPyramid().getLevelCards(1)){
        if (player.canBuyCard(*card)){
            result.push_back(CompulsoryActions::BuyCard);
            hasBeenAdded = true;
            break;
        }
    }
    if (not hasBeenAdded){
        for (auto card : game.getGameTable().getPyramid().getLevelCards(2)){
            if (player.canBuyCard(*card)){
                result.push_back(CompulsoryActions::BuyCard);
                hasBeenAdded = true;
                break;
            }
        }
    }

    if (not hasBeenAdded) {
        for (auto card: game.getGameTable().getPyramid().getLevelCards(3)) {
            if (player.canBuyCard(*card)) {
                result.push_back(CompulsoryActions::BuyCard);
                break;
            }
        }
    }

    if (not hasBeenAdded && not player.getReserve().empty()){
        for (auto card: player.getReserve()) {
            if (player.canBuyCard(*card)) {
                result.push_back(CompulsoryActions::BuyCard);
                break;
            }
        }
    }
    return result;
}

void Controller::applyOptionalAction(Game &game, Player &player, OptionalActions action) {
    switch (action) {
        case OptionalActions::UsePrivileges:
            usePriviledge(game.getGameTable().getBoard());
            break;
        case OptionalActions::FillBoard:
            fillBoard(game.getGameTable().getBoard(), game.getGameTable().getBag());
            break;
        case OptionalActions::Empty:
            break;
    }
}

void Controller::applyCompulsoryAction(Game &game, Player &player, CompulsoryActions action) {
    std::vector<const Token*> tokens;
    bool check = true;
    unsigned int counter = 0;
    std::vector<std::pair<int, int>>* indices = nullptr;
    TokenColor color = TokenColor::None;
    switch (action) {
        case CompulsoryActions::TakeCoins:
            if (currentPlayer->getType()==Type::Humain) {
                std::cout << game.getGameTable().getPyramid() << std::endl;
                indices = new std::vector<std::pair<int, int>>;
                for (unsigned int i = 0; i < 3; i++) {
                    tokens.push_back(&currentPlayer->getStrategy()->chooseToken(game.getGameTable().getBoard(), player, indices));
                    if (tokens[i]->getColor()==TokenColor::None) {
                        break;
                    }
                }
            }
            else {
                std::vector<std::pair<int, int>> indicesia;
                Board::getInstance()->showBoard();
                indicesia = currentPlayer->getStrategy()->choseTokensToTake();
                for (unsigned int i = 0; i < indicesia.size(); i++) {
                    const Token& token = Board::getInstance()->takeToken(indicesia[i].first, indicesia[i].second);
                    tokens.push_back(&token);
                    if (tokens[i]->getColor()==TokenColor::None) {
                        break;
                    }
                }
                for (auto token : tokens) {
                    if (token!=nullptr) {
                        currentPlayer->addToken(*token);
                    }
                }
            }
            if (tokens.size()!=3 || TokenColor::None == tokens[0]->getColor() || TokenColor::None == tokens[1]->getColor() || TokenColor::None == tokens[2]->getColor()) {
                check = false;
            }
            for (auto token : tokens) {
                if (token!=nullptr) {
                    if (color==TokenColor::None) {
                        color = token->getColor();
                    }
                    else if (color!=token->getColor()) {
                        check = false;
                    }
                }
            }
            if (not(check)) {
                for (auto token : tokens) {
                    if (token!=nullptr) {
                        if (token->getColor()==TokenColor::PERLE) {
                            counter++;
                        }
                    }
                }
                if (counter == 2) {
                    check = true;
                }
            }
            if (check) {
                if (game.getGameTable().getBoard().getNbPrivileges()>0) {
                    getopposingPlayer().addPrivilege(game.getGameTable().getBoard().takePrivilege());
                }
                else if (currentPlayer->getNbPrivilege()>0) {
                    getopposingPlayer().addPrivilege(currentPlayer->removePrivilege());
                }
            }
            break;
        case CompulsoryActions::ReserveCard:
            bookCard(game.getGameTable().getPyramid(), game.getGameTable());
            break;
        case CompulsoryActions::BuyCard:
            buyJewelryCard(game.getGameTable());
            break;
    }
}

void Controller::usePriviledge(Board& board) {
    unsigned int nbp = currentPlayer->getNbPrivilege();
    unsigned int nbt = board.getNbTokens();
    unsigned int nb = std::min(nbp,nbt);
    unsigned int nbPrivilege; // nombre de privilege a utiliser

    // le joueur est humain
    if (currentPlayer->getType()==Type::Humain){
        while(nbPrivilege < 1 || nbPrivilege > nb){
            std::cout << "Vous pouvez utiliser au plus " << nb << "privileges.\nCombien souhaitez-vous en utiliser ? "<< std::endl;
            std::cin >>nbPrivilege;
        }
    }
    // le joueur est IA
    else{
        nbPrivilege = currentPlayer->getStrategy()->choicemaker(1,nb);
    }
    
    //nbPrivilege = choiceMaker(0, nb);
    for (unsigned int i = 0; i < nbPrivilege; i++) {
        board.placePrivilege(currentPlayer->removePrivilege());
        currentPlayer->getStrategy()->chooseToken(board, *currentPlayer);
    }
}

bool areCoordinatesAlignedAndConsecutive(const std::vector<std::pair<int, int>>* coordinates) {
    if (!coordinates || coordinates->size() < 2 || coordinates->size() > 3) {
        return false;
    }

    // Créer une copie des coordonnées pour les trier
    std::vector<std::pair<int, int>> sortedCoords = *coordinates;

    // Trier les coordonnées selon x, puis selon y si x est le même
    std::sort(sortedCoords.begin(), sortedCoords.end());

    // Calculer la différence initiale entre les premières coordonnées
    int dx = sortedCoords[1].first - sortedCoords[0].first;
    int dy = sortedCoords[1].second - sortedCoords[0].second;

    // Vérifier si les coordonnées sont alignées horizontalement, verticalement, ou diagonalement
    if (!(dx == 0 || dy == 0 || std::abs(dx) == std::abs(dy))) {
        return false;
    }

    for (size_t i = 0; i < sortedCoords.size() - 1; ++i) {
        int currentDx = sortedCoords[i + 1].first - sortedCoords[i].first;
        int currentDy = sortedCoords[i + 1].second - sortedCoords[i].second;

        // Vérifier si les coordonnées sont consécutives et suivent la même direction
        if (std::abs(currentDx) > 1 || std::abs(currentDy) > 1 || currentDx != dx || currentDy != dy) {
            return false;
        }
    }

    return true;
}


void Controller::fillBoard(Board&board, Bag&bag) {
    if (board.getNbPrivileges()!=0) {
        getopposingPlayer().addPrivilege(board.takePrivilege());
    }
    else if (currentPlayer->getNbPrivilege()!=0) {
        getopposingPlayer().addPrivilege(currentPlayer->removePrivilege());
    }
    board.fillBoard(bag);
}

void Controller::bookCard(Pyramid_Cards& pyramid, GameTable& gametable) {
    std::cout << "Reservation d'une carte" << std::endl;
    unsigned int choiceDeckOrPyramid;
    // le joueur est un humain
    if(currentPlayer->getType() == Type::Humain){
        while(choiceDeckOrPyramid != 1 && choiceDeckOrPyramid!=2){
            std::cout << "Voulez-vous reserver une carte d'une des pioches (1) ou une carte de la pyramide (2) ?" << std::endl;
            std::cin >> choiceDeckOrPyramid;
        }
    }
    //le joueur est une ia
    else{
        std::vector<int> choices = {1,2};
        if (gametable.getPyramid().isEmpty(1) && gametable.getPyramid().isEmpty(2) && gametable.getPyramid().isEmpty(3)) {
            choices.erase(std::remove(choices.begin(), choices.end(), 2), choices.end());
        }
        if (getGame().getGameTable().getDeckLevelOne().getPioche().empty() && getGame().getGameTable().getDeckLevelTwo().getPioche().empty() && getGame().getGameTable().getDeckLevelThree().getPioche().empty()) {
            choices.erase(std::remove(choices.begin(), choices.end(), 1), choices.end());
        }
        //make a random choice between the numbers in the choices
        choiceDeckOrPyramid = choices[currentPlayer->getStrategy()->choicemaker(0, choices.size()-1)];
    }
    //unsigned int choice = choiceMaker(1, 2);
    // reservation carte de la pioche
    if (choiceDeckOrPyramid == 1) {
        unsigned int choiceDeckLevel;
        //unsigned int choice2 = choiceMaker(1, 3);

        // le joueur est un humain
        if(currentPlayer->getType() == Type::Humain){
            while(choiceDeckLevel != 1 && choiceDeckLevel != 2 && choiceDeckLevel != 3){
                std::cout << "Veuillez choisir une pioche" << std::endl;
                std::cout << "1. Pioche niveau 1" << std::endl;
                std::cout << "2. Pioche niveau 2" << std::endl;
                std::cout << "3. Pioche niveau 3" << std::endl;
                std::cout << "Veuillez choisir une pioche (1, 2 ou 3) : ";
                std::cin >> choiceDeckLevel;
            }
        }
        // le joueur est une ia
        else{
            std::vector<int> levels = {1,2,3};
            if (gametable.getDeckLevelOne().getPioche().empty()) {
                levels.erase(std::remove(levels.begin(), levels.end(), 1), levels.end());
            }
            if (gametable.getDeckLevelTwo().getPioche().empty()) {
                levels.erase(std::remove(levels.begin(), levels.end(), 2), levels.end());
            }
            if (gametable.getDeckLevelThree().getPioche().empty()) {
                levels.erase(std::remove(levels.begin(), levels.end(), 3), levels.end());
            }
            //make a random choice between the numbers in the levels
            choiceDeckLevel = levels[currentPlayer->getStrategy()->choicemaker(0, levels.size()-1)];
        }
        
        JewelryCard& card = takeCard(choiceDeckLevel);
        currentPlayer->getStrategy()->chooseGoldenToken(game->getGameTable().getBoard(), *currentPlayer);
        currentPlayer->reserveOneCard(card);
    }
    // reservation carte de la pyramide
    else {
        // choix du level de la carte a prendre
        unsigned int cardLevel;
        //unsigned int level = choiceMaker(1, 3);

        // le joueur est un humain
        if(currentPlayer->getType() == Type::Humain){
            std::cout << gametable.getPyramid() << std::endl;
            while(cardLevel != 1 && cardLevel != 2 && cardLevel != 3){
                std::cout << "Veuillez choisir un niveau de carte" << std::endl;
                std::cout << "1. Carte de niveau 1" << std::endl;
                std::cout << "2. Carte de niveau 2" << std::endl;
                std::cout << "3. Carte de niveau 3" << std::endl;
                std::cout << "Veuillez choisir un niveau (1, 2 ou 3) : ";
                std::cin >> cardLevel;
            }
        }
        
        // le joueur est une ia
        else{
            std::vector<int> levels = {1,2,3};
            if (gametable.getPyramid().isEmpty(1)) {
                levels.erase(std::remove(levels.begin(), levels.end(), 1), levels.end());
            }
            if (gametable.getPyramid().isEmpty(2)) {
                levels.erase(std::remove(levels.begin(), levels.end(), 2), levels.end());
            }
            if (gametable.getPyramid().isEmpty(3)) {
                levels.erase(std::remove(levels.begin(), levels.end(), 3), levels.end());
            }
            //make a random choice between the numbers in the levels
            cardLevel = levels[currentPlayer->getStrategy()->choicemaker(0, levels.size()-1)];
        }
        
        unsigned int nb = pyramid.getLevelCards(cardLevel).size(); // nombre de cartes de niveau level dans pyramid
        unsigned int cardPosition = 0;
        //unsigned int nbCard = choiceMaker(1, nb);
        // choix de la position de la carte dans le niveau de la pyramide choisi
        // le joueur est un humain
        if(currentPlayer->getType() == Type::Humain){
            while(cardPosition < 1 || cardPosition > nb){
                std::cout << "Veuillez choisir la position de la carte" << std::endl;
                std::cin >> cardPosition;
            }
        }
        // le joueur est une ia
        else{
            cardPosition = currentPlayer->getStrategy()->choicemaker(1, nb);
        }
        currentPlayer->getStrategy()->chooseGoldenToken(gametable.getBoard(), *currentPlayer);
        currentPlayer->reserveOneCard(pyramid.takeCard(cardLevel,cardPosition-1));
        pyramid.drawCard(cardLevel);
    }
}

void Controller::buyJewelryCard(GameTable& gametable) {
    JewelryCard* cardbought;
    if (currentPlayer->getType() == Type::Humain) {
        bool bought = false;
        std::cout << "Jetons disponibles : " << std::endl;
        std::cout << *currentPlayer << std::endl;
        if (currentPlayer->getReserve().size() > 0) {
            //unsigned int choice = choiceMaker(1, 2);
            unsigned int buyReservedOrRegularCard;
            // joueur humain
            while (buyReservedOrRegularCard != 1 && buyReservedOrRegularCard!=2){
                std::cout << "Voulez-vous acheter une carte reservee ?" << std::endl;
                std::cout << "1. Oui" << std::endl;
                std::cout << "2. Non" << std::endl;
                std::cout << "Choix (1 ou 2) : ";
                std::cin >> buyReservedOrRegularCard;
            }
            if (buyReservedOrRegularCard == 1) {
                bought = true;
                for (auto card : currentPlayer->getReserve()) {
                    std::cout << *card << std::endl;
                }
                //unsigned int nbCard = choiceMaker(1, currentPlayer->getReserve().size());
                unsigned int reservedCardChoice = 0;
                // joueur H
                while(reservedCardChoice < 1 || reservedCardChoice > currentPlayer->getReserve().size()){
                    std::cout << "Veuillez choisir la carte reservee a acheter" << std::endl;
                    std::cout << "Entrez un nombre entre 1 et currentPlayer->getReserve().size() : ";
                    std::cin >> reservedCardChoice;
                }
                cardbought = currentPlayer->getReserve()[reservedCardChoice - 1];
                if (currentPlayer->canBuyCard(*cardbought)) {
                    currentPlayer->actionBuyReservedCard(*cardbought);
                }
            }
        }
        // achat d'une carte dans la pyramide
        if (not(bought)) {
            // choix du niveau de la carte
            //unsigned int level = choiceMaker(1, 3);
            unsigned int cardLevel;
            // joueur H
            while(cardLevel != 1 && cardLevel != 2 && cardLevel != 3){
                std::cout << "Veuillez choisir un niveau de carte" << std::endl;
                std::cout << "1. Carte de niveau 1" << std::endl;
                std::cout << "2. Carte de niveau 2" << std::endl;
                std::cout << "3. Carte de niveau 3" << std::endl;
                std::cout << "Veuillez choisir un niveau (1, 2 ou 3) : ";
                std::cin >> cardLevel;
            }
            unsigned int nb = gametable.getPyramid().getLevelCards(cardLevel).size();
            std::cout << "Cartes de niveau " << cardLevel << " : \n";
            for (auto card: gametable.getPyramid().getLevelCards(cardLevel)) {
                std::cout << *card << std::endl;
            }
            // choix de la position de la carte dans le niveau de la pyramide choisi
            //unsigned int nbCard = choiceMaker(1, nb);
            unsigned int cardPosition = 0;
            // le joueur est un humain
            while(cardPosition < 1 || cardPosition > nb){
                std::cout << "Veuillez choisir la position de la carte choisie (entre 1 et "<<nb<<") : " << std::endl;
                std::cin >> cardPosition;
            }
            cardbought = &gametable.getPyramid().takeCard(cardLevel, cardPosition - 1);
            gametable.getPyramid().drawCard(cardLevel);
            if (currentPlayer->canBuyCard(*cardbought)) {
                currentPlayer->actionBuyCard(*cardbought);
            }
            else {
                throw JewelryCardError("Vous ne pouvez pas acheter cette carte");
            }
        }
    }
    else {
        bool bought = false;
        //find the first buyable card in the reserve, then in the pyramid
        for (auto card : currentPlayer->getReserve()) {
            if (currentPlayer->canBuyCard(*card)) {
                cardbought = card;
                currentPlayer->actionBuyReservedCard(*card);
                bought = true;
                break;
            }
        }
        if (not bought) {
            for (int i = 1; i <= 3; i++) {
                for (int j = 0; j < gametable.getPyramid().getLevelCards(i).size(); j++) {
                    if (currentPlayer->canBuyCard(*gametable.getPyramid().getLevelCards(i)[j])) {
                        cardbought = gametable.getPyramid().getLevelCards(i)[j];
                        gametable.getPyramid().takeCard(i, j);
                        gametable.getPyramid().drawCard(i);
                        currentPlayer->actionBuyCard(*cardbought);
                        bought = true;
                        break;
                    }
                }
                if (bought) {
                    break;
                }
            }
        }
    }
    applyCardSkills(*game, *currentPlayer, getopposingPlayer(),*cardbought);
}

void Controller::buyNobleCard() {
    unsigned int nbCard = 0;
    // joueur humain
    if (currentPlayer->getType()==Type::Humain){
        for (auto card : Deck_Royal::getInstance()->getCards()) {
            std::cout << *card << std::endl;
        }
        while (nbCard < 1|| nbCard > Deck_Royal::getInstance()->getCards().size()){
            std::cout << "Veuillez choisir une carte" << std::endl;
            std::cout << "Choix entre 1 et " << Deck_Royal::getInstance()->getCards().size() << " : ";
            std::cin >> nbCard;
        }
    }
    //joueur IA
    else{
        nbCard = currentPlayer->getStrategy()->choicemaker(1,Deck_Royal::getInstance()->getCards().size());
    }

    RoyalCard& card = *Deck_Royal::getInstance()->getCards()[nbCard - 1];
    currentPlayer->addRoyalCard(card, nbCard - 1);
    applyRoyalCardSkills(*game,*currentPlayer,getopposingPlayer(),card);
}

void Controller::applyCardSkills(Game&game, Player&cardOwner, Player&opponent, JewelryCard&card) {
    if (card.getAbility2()==Abilities::None) {
        if (card.getAbility1() == Abilities::take_privilege) {
            if (getGame().getGameTable().getBoard().getNbPrivileges()!=0) {
                cardOwner.addPrivilege(getGame().getGameTable().getBoard().takePrivilege());
            }
            else if (getopposingPlayer().getNbPrivilege()!=0) {
                cardOwner.addPrivilege(getopposingPlayer().removePrivilege());
            }
        }
        else if (card.getAbility1() == Abilities::take_bonus_token) {
            currentPlayer->getStrategy()->chooseToken(game.getGameTable().getBoard(), cardOwner);
        }
        else if (card.getAbility1() == Abilities::steal_token) {
            if (opponent.getNbTokens()!=0) {
                std::cout << "Voici les jetons du joueur adverse : \n";
                std::cout << opponent << std::endl;
                //std::string rep;
                TokenColor color;

                // joueur H
                std::vector<TokenColor> colors = {TokenColor::BLANC, TokenColor::VERT, TokenColor::ROUGE, TokenColor::BLEU, TokenColor::PERLE, TokenColor::NOIR};
                if(currentPlayer->getType()==Type::Humain){
                    color =currentPlayer->getStrategy()->choseTokenColor(colors);
                }
                // ia
                else{
                    color =currentPlayer->getStrategy()->choseTokenColor(colors);
                }
                // std::cin >> rep;
                // if (rep != "NOIR" && rep != "ROUGE" && rep != "BLEU" && rep != "VERT" && rep != "BLANC" && rep != "PERLE") {
                //     throw TokenException("Couleur invalide");
                // }

                //TokenColor color = toTokenColor(rep);
                if (getopposingPlayer().getTokenSummary().find(color)->second==0){
                    std::cout << "Le joueur adverse n'a pas de jeton de cette couleur" << std::endl;
                }
                else {
                    const Token& token = getopposingPlayer().removeToken(color);
                    cardOwner.addToken(token);
                }
            }
            else {
                std::cout << "Le joueur adverse n'a pas de jetons" << std::endl;
            }
        }
        else if (card.getAbility1() == Abilities::cameleon) {
            // std::cout << "Veuillez choisir une couleur de jeton  (NOIR, ROUGE, BLEU, VERT, BLANC)" << std::endl;
            // std::string rep;
            // std::cin >> rep;
            // if (rep != "NOIR" && rep != "ROUGE" && rep != "BLEU" && rep != "VERT" && rep != "BLANC") {
            //     throw TokenException("Couleur invalide");
            // }
            //std::string rep;
            TokenColor color = TokenColor::ROUGE;
            // joueur H
            std::vector<TokenColor> colors = {TokenColor::BLANC, TokenColor::VERT, TokenColor::ROUGE, TokenColor::BLEU, TokenColor::NOIR};
            if(currentPlayer->getType()==Type::Humain){
                color =currentPlayer->getStrategy()->choseTokenColor({colors});
            }
            // ia
            else{
                for (int i = 0; i < 5; i++) {
                    if (cardOwner.getColorSummary(colors[i]).getBonusNumber() != 0) {
                        color = colors[i];
                        break;
                    }
                }
            }


            //TokenColor color = toTokenColor(rep);

            if (cardOwner.getColorSummary(color).getBonusNumber() == 0 ){
                std::cout<<"Vous n'avez pas de cartes de cette couleur" << std::endl;
                if (card.getPrestige()!=0){
                    cardOwner.addPrestige(card.getPrestige(),TokenColor::None);
                }
            }
            else {
                std::cout<<"Bonus ajouté" << std::endl;
                cardOwner.getColorSummary(color).addBonusNumber(1);
                cardOwner.addPrestige(card.getPrestige(),color);
            }
        }
        else if (card.getAbility1() == Abilities::repeat_turn) {
            std::cout<<"Vous rejouez" << std::endl;
            playTurn();
        }
    }
    else {
        TokenColor color = TokenColor::ROUGE;
        // joueur H
        std::vector<TokenColor> colors = {TokenColor::BLANC, TokenColor::VERT, TokenColor::ROUGE, TokenColor::BLEU, TokenColor::NOIR};
        if(currentPlayer->getType()==Type::Humain){
            color =currentPlayer->getStrategy()->choseTokenColor({colors});
        }
        // ia
        else{
            for (int i = 0; i < 5; i++) {
                if (cardOwner.getColorSummary(colors[i]).getBonusNumber() != 0) {
                    color = colors[i];
                    break;
                }
            }
        }


        //TokenColor color = toTokenColor(rep);

        if (cardOwner.getColorSummary(color).getBonusNumber() == 0 ){
            std::cout<<"Vous n'avez pas de cartes de cette couleur" << std::endl;
            if (card.getPrestige()!=0){
                cardOwner.addPrestige(card.getPrestige(),TokenColor::None);
            }
        }
        else {
            cardOwner.getColorSummary(color).addBonusNumber(1);
            cardOwner.addPrestige(card.getPrestige(),color);
        }
        std::cout<<"La totale" << std::endl;
        playTurn();
    }

}

void Controller::applyRoyalCardSkills(Game&game, Player&cardOwner, Player&opponent, RoyalCard&card) {
    if (card.getAbility() == Abilities::take_privilege) {
        if (getGame().getGameTable().getBoard().getNbPrivileges()!=0) {
            cardOwner.addPrivilege(getGame().getGameTable().getBoard().takePrivilege());
        }
        else if (getopposingPlayer().getNbPrivilege()!=0) {
            cardOwner.addPrivilege(getopposingPlayer().removePrivilege());
        }
    }
    else if (card.getAbility() == Abilities::steal_token) {
        if (opponent.getNbTokens()!=0) {
            std::cout << "Voici les jetons du joueur advrese : \n";
            std::cout << opponent << std::endl;
            // std::cout << "Veuillez choisir une couleur de jeton  (NOIR, ROUGE, BLEU, VERT, BLANC, PERLE)" << std::endl;
            // std::string rep;
            // std::cin >> rep;
            // if (rep != "NOIR" && rep != "ROUGE" && rep != "BLEU" && rep != "VERT" && rep != "BLANC" && rep != "PERLE") {
            //     throw TokenException("Couleur invalide");
            // }
            // TokenColor color = toTokenColor(rep);
            TokenColor color;
            // joueur H
            std::vector<TokenColor> colors = {TokenColor::BLANC, TokenColor::VERT, TokenColor::ROUGE, TokenColor::BLEU, TokenColor::PERLE, TokenColor::NOIR};
            if(currentPlayer->getType()==Type::Humain){
                color =currentPlayer->getStrategy()->choseTokenColor({colors});
            }
            // ia
            else{
                color =currentPlayer->getStrategy()->choseTokenColor({colors});
            }
            
            if (getopposingPlayer().getTokenSummary().find(color)->second==0){
                std::cout << "Le joueur adverse n'a pas de jeton de cette couleur" << std::endl;
            }
            else {
                const Token& token = getopposingPlayer().removeToken(color);
                cardOwner.addToken(token);
            }
        }
        else {
            std::cout << "Le joueur adverse n'a pas de jetons" << std::endl;
        }
    }
    else if (card.getAbility() == Abilities::repeat_turn) {
        playTurn();
    }
}

void Controller::play() {
    while (not checkIfPlayerWins(*game, getopposingPlayer())) {
        getGame().setRound(getGame().getRound()+0.5);
        playTurn();
        if(currentPlayer->getCrowns() >=3 && currentPlayer->getRoyalCards().empty() || currentPlayer->getCrowns() >= 6 && currentPlayer->getRoyalCards().size()==1){
            buyNobleCard();
        }
        changeCurrentPlayer();
    }
    std::cout << "Le joueur " << getopposingPlayer().getName() << " a gagne !" << std::endl;
}

void Controller::playTurn() {
    std::cout << "C'est au tour de " << currentPlayer->getName() << std::endl;
    if (currentPlayer->getType()==Type::Humain) {
        std::cout << *currentPlayer << std::endl;
        std::cout << "Pyramide: " << std::endl;
        std::cout << getGame().getGameTable().getPyramid() << std::endl;
    }
    std::vector<CompulsoryActions> compulsoryActions1 = getCompulsoryActions(*game, *currentPlayer);
    unsigned int choice = 0;
    std::vector<OptionalActions> optionalActions;
    if (compulsoryActions1.empty()) {
        if (game->getGameTable().getBag().isEmpty()) {
            throw TokenException("Egalité des IA");
        }
        applyOptionalAction(*game, *currentPlayer, OptionalActions::FillBoard);
        std::cout << "Pas d'action obligatoires possibles, remplisage du plateau" << std::endl;
    }
    else {
        optionalActions = getOptionalActions(*game, *currentPlayer);
        while (choice != optionalActions.size() && optionalActions.size() !=1) {
            std::cout << "Veuillez choisir une action optionnelle" << std::endl;
            for (auto action : optionalActions) {
                switch (action) {
                    case OptionalActions::Empty:
                        std::cout << "Ne rien faire" << std::endl;
                    break;
                    case OptionalActions::UsePrivileges:
                        std::cout << "Utiliser des privileges" << std::endl;
                    break;
                    case OptionalActions::FillBoard:
                        std::cout << "Remplir le plateau" << std::endl;
                    break;
                }
            }
            choice = currentPlayer->getStrategy()->choicemaker(1, optionalActions.size());
            applyOptionalAction(*game, *currentPlayer, optionalActions[choice-1]);
            if (optionalActions[choice-1] == OptionalActions::FillBoard || currentPlayer->getNbPrivilege() == 0 || (currentPlayer->getNbPrivilege()>0 && game->getGameTable().getBoard().isEmpty())){
                choice = optionalActions.size();
            }
        }
    }
    std::vector<CompulsoryActions> compulsoryActions = getCompulsoryActions(*game, *currentPlayer);
    if (compulsoryActions.empty()) {
        if (game->getGameTable().getBag().isEmpty()) {
            throw TokenException("Egalité des IA");
        }
        applyOptionalAction(*game, *currentPlayer, OptionalActions::FillBoard);
        std::cout << "Pas d'action obligatoires possibles, remplisage du plateau" << std::endl;
    }
    compulsoryActions = getCompulsoryActions(*game, *currentPlayer);
    std::cout << "Veuillez choisir une action obligatoire" << std::endl;
    for (auto action : compulsoryActions) {
        switch (action) {
            case CompulsoryActions::TakeCoins:
                std::cout << "Prendre des jetons" << std::endl;
            break;
            case CompulsoryActions::ReserveCard:
                std::cout << "Reserver une carte" << std::endl;
            break;
            case CompulsoryActions::BuyCard:
                std::cout << "Acheter une carte" << std::endl;
            break;
        }
    }
    choice = currentPlayer->getStrategy()->choicemaker(1, compulsoryActions.size());
    applyCompulsoryAction(*game, *currentPlayer, compulsoryActions[choice-1]);
    if (currentPlayer->getNbTokens()>10) {
        if (currentPlayer->getType()==Type::Humain) {
            unsigned int nb = - (10 - currentPlayer->getNbTokens());
            std::cout << "Voici vos Jetons, vous devez en retirer " << nb << " pour n'en conserver que 10 : " << std::endl;
            std::cout << *currentPlayer << std::endl;
            while (nb != 0) {
                std::cout << "Veuillez choisir une couleur de jeton  (NOIR, ROUGE, BLEU, VERT, BLANC, PERLE, OR)" << std::endl;
                std::string rep;
                std::cin >> rep;
                if (rep != "NOIR" && rep != "ROUGE" && rep != "BLEU" && rep != "VERT" && rep != "BLANC" && rep != "PERLE" && rep != "OR") {
                    throw TokenException("Couleur invalide");
                }
                TokenColor color = toTokenColor(rep);
                std::cout << "Choisissez combien vous voulez en retirer " << std::endl;
                unsigned int tot = currentPlayer->getStrategy()->choicemaker(1, currentPlayer->getTokenSummary().find(color)->second);
                for (unsigned int i = 0; i < tot; i++) {
                    game->getGameTable().getBag().addToken(currentPlayer->removeToken(color));
                    nb--;
                }
            }
        }
        else {
            int nb = - (10 - currentPlayer->getNbTokens());
            while (nb!=0) {
                //check in all the tokens and remobve until having only 10
                std::vector<TokenColor> colors;
                for (auto token : currentPlayer->getTokenSummary()) {
                    if (token.second > 0) {
                        colors.push_back(token.first);
                    }
                }
                //on prend une couleur random
                TokenColor color = colors[currentPlayer->getStrategy()->choicemaker(0, colors.size()-1)];
                unsigned int tot = currentPlayer->getStrategy()->choicemaker(1, std::min(currentPlayer->getTokenSummary().find(color)->second,nb));
                for (unsigned int i = 0; i < tot; i++) {
                    game->getGameTable().getBag().addToken(currentPlayer->removeToken(color));
                    nb--;
                }
            }
        }
    }
}

bool Controller::checkIfPlayerWins(Game& game, Player& player) {
    if (player.getPrestige() >= 20) {
        return true;
    }
    else if (player.getCrowns()>=10) {
        return true;
    }
    else if(player.getMaxPrestigeColor()>=10) {
        return true;
    }

    return false;
}

void Controller::reinit() {
    // Reset all the instances as needed
    this->getGame().getGameTable().getDeckRoyal().resetInstance();
    this->getGame().getGameTable().getDeckLevelOne().resetInstance();
    this->getGame().getGameTable().getDeckLevelTwo().resetInstance();
    this->getGame().getGameTable().getDeckLevelThree().resetInstance();
    this->getGame().getGameTable().getBag().resetInstance();
    this->getGame().getGameTable().getBoard().resetInstance();
    this->getGame().getGameTable().getPyramid().resetInstance();
}

void retrieveCurrentPlayer(Controller& controller) {
    // Get the path to the database
    std::filesystem::path sourceFilePath = std::filesystem::path(__FILE__).parent_path();
    std::string databaseSavePath = sourceFilePath.string() + "/Data/save.db";

    sqlite3* db;
    int rc = sqlite3_open(databaseSavePath.c_str(), &db);
    if (rc) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    std::ostringstream oss;
    oss << "SELECT * FROM infopartie";
    std::string queryRes = oss.str();

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, queryRes.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing query: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Extract data from each column based on the INSERT query structure
        std::string currentplayer = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        if(controller.getcurrentPlayer().getName()!=currentplayer) {
            controller.changeCurrentPlayer();
        }
    }
    // Finalize and close database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}