#include "Save.h"

void cleanTable(const std::string& dbPath, const std::string& tableName) {
    sqlite3* db;
    int rc = sqlite3_open(dbPath.c_str(), &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Error opening the database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    std::string deleteQuery = std::string("DELETE FROM " + tableName + ";");
    //std::cout << deleteQuery;

    rc = sqlite3_exec(db, deleteQuery.c_str(), nullptr, nullptr, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Table " << tableName << " cleaned." << std::endl;
    }

    sqlite3_close(db);
}

void writeToDatabase(const Game& game) {
    sqlite3* db;
    // Obtient le chemin du fichier source actuel (__FILE__)
    std::filesystem::path sourceFilePath = std::filesystem::path(__FILE__);
    // Obtient le répertoire du fichier source
    std::filesystem::path sourceDirectory = sourceFilePath.parent_path();
    std::string pathtodatabase = sourceDirectory.string() + "/Data/save.db";
    int rc = sqlite3_open(pathtodatabase.c_str(), &db);

    if (rc != SQLITE_OK) { //si il y a une erreur lors de l'ouverture de la database, on renvoie un message d'ereur
        std::cerr << "Error opening the database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    //Clean de la table
    const std::string& tablePyra = "pyramid";
    cleanTable(pathtodatabase, tablePyra);

    /*--------------------------------------------------------------------------*/

    // Stockage de la pyramide
    for (auto card : game.getGameTable().getPyramid().getRow1()) { // faire pour chaque row

        // Requete sql pour écrire dans la database
        std::string insertQuery1 = std::string("INSERT INTO pyramid (idCard, prestige, crown, cost_w, cost_v, cost_n, cost_p, cost_r, cost_b, color, position, ability1, ability2, bonus_nb) VALUES (") +
            "'" + std::to_string(card->getId()) + "', " +
            "'" + std::to_string(card->getPrestige()) + "', " +
            "'" + std::to_string(card->getCrowns()) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::BLANC)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::VERT)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::NOIR)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::PERLE)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::ROUGE)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::BLEU)) + "', " +
            "'" + toString(card->getBonus().bonus_color) + "', " +
            "'" + std::to_string(card->getLevel()) + "', " +
            "'" + toString(card->getAbility1()) + "', " +
            "'" + toString(card->getAbility2()) + "', " +
            "'" + std::to_string(card->getBonus().bonus_number) + "');";

        // Execution de la requete sql
        rc = sqlite3_exec(db, insertQuery1.c_str(), nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
          std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
        }
    }

    for (auto card : game.getGameTable().getPyramid().getRow2()) { // faire pour chaque row

        // Requete sql pour écrire dans la database
        std::string insertQuery2 = std::string("INSERT INTO pyramid (idCard, prestige, crown, cost_w, cost_v, cost_n, cost_p, cost_r, cost_b, color, position, ability1, ability2, bonus_nb) VALUES (") +
            "'" + std::to_string(card->getId()) + "', " +
            "'" + std::to_string(card->getPrestige()) + "', " +
            "'" + std::to_string(card->getCrowns()) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::BLANC)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::VERT)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::NOIR)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::PERLE)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::ROUGE)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::BLEU)) + "', " +
            "'" + toString(card->getBonus().bonus_color) + "', " +
            "'" + std::to_string(card->getLevel()) + "', " +
            "'" + toString(card->getAbility1()) + "', " +
            "'" + toString(card->getAbility2()) + "', " +
            "'" + std::to_string(card->getBonus().bonus_number) + "');";

        // Execution de la requete sql
        rc = sqlite3_exec(db, insertQuery2.c_str(), nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
            std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
        }
    }

    for (auto card : game.getGameTable().getPyramid().getRow3()) {
        // faire pour chaque row

        // Requete sql pour écrire dans la database
        std::string insertQuery3 = std::string("INSERT INTO pyramid (idCard, prestige, crown, cost_w, cost_v, cost_n, cost_p, cost_r, cost_b, color, position, ability1, ability2, bonus_nb) VALUES (") +
            "'" + std::to_string(card->getId()) + "', " +
            "'" + std::to_string(card->getPrestige()) + "', " +
            "'" + std::to_string(card->getCrowns()) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::BLANC)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::VERT)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::NOIR)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::PERLE)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::ROUGE)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::BLEU)) + "', " +
            "'" + toString(card->getBonus().bonus_color) + "', " +
            "'" + std::to_string(card->getLevel()) + "', " +
            "'" + toString(card->getAbility1()) + "', " +
            "'" + toString(card->getAbility2()) + "', " +
            "'" + std::to_string(card->getBonus().bonus_number) + "');";
        std::cout << insertQuery3 << std::endl;
        // Execution de la requete sql
        rc = sqlite3_exec(db, insertQuery3.c_str(), nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
            std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
        }
    }

    /*--------------------------------------------------------------------------*/

    // Stockage de la pioche de cartes de level 1

    const std::string& tableD1 = "deck1";
    cleanTable(pathtodatabase, tableD1);

    for (auto card : Deck_level_one::getInstance()->getPioche()) {

        // Requete sql pour écrire dans la database
        std::string insertQuery = std::string("INSERT INTO deck1 (idCard, prestige, crown, cost_w, cost_v, cost_n, cost_p, cost_r, cost_b, color, level, ability1, ability2, bonus_nb) VALUES (") +
            "'" + std::to_string(card->getId()) + "', " +
            "'" + std::to_string(card->getPrestige()) + "', " +
            "'" + std::to_string(card->getCrowns()) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::BLANC)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::VERT)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::NOIR)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::PERLE)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::ROUGE)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::BLEU)) + "', " +
            "'" + toString(card->getBonus().bonus_color) + "', " +
            "'" + std::to_string(card->getLevel()) + "', " +
            "'" + toString(card->getAbility1()) + "', " +
            "'" + toString(card->getAbility2()) + "', " +
            "'" + std::to_string(card->getBonus().bonus_number) + "');";
        //std::cout << insertQuery << std::endl;
        // Execution de la requete sql
        rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
            std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
        }
    }

    const std::string& tableD2 = "deck2";
    cleanTable(pathtodatabase, tableD2);

    // Stockage de la pioche de cartes de level 2
    for (auto card : Deck_level_two::getInstance()->getPioche()) {

        // Requete sql pour écrire dans la database
        std::string insertQuery = std::string("INSERT INTO deck2 (idCard, prestige, crown, cost_w, cost_v, cost_n, cost_p, cost_r, cost_b, color, level, ability1, ability2, bonus_nb) VALUES (") +
            "'" + std::to_string(card->getId()) + "', " +
            "'" + std::to_string(card->getPrestige()) + "', " +
            "'" + std::to_string(card->getCrowns()) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::BLANC)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::VERT)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::NOIR)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::PERLE)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::ROUGE)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::BLEU)) + "', " +
            "'" + toString(card->getBonus().bonus_color) + "', " +
            "'" + std::to_string(card->getLevel()) + "', " +
            "'" + toString(card->getAbility1()) + "', " +
            "'" + toString(card->getAbility2()) + "', " +
            "'" + std::to_string(card->getBonus().bonus_number) + "');";
        //std::cout << insertQuery << std::endl;
        // Execution de la requete sql
        rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
            std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
        }
    }

    // Stockage de la pioche de cartes de level 3

    const std::string& tableD3 = "deck3";
    cleanTable(pathtodatabase, tableD3);

    for (auto card : Deck_level_three::getInstance()->getPioche()) {

        // Requete sql pour écrire dans la database
        std::string insertQuery = std::string("INSERT INTO deck3 (idCard, prestige, crown, cost_w, cost_v, cost_n, cost_p, cost_r, cost_b, color, level, ability1, ability2, bonus_nb) VALUES (") +
            "'" + std::to_string(card->getId()) + "', " +
            "'" + std::to_string(card->getPrestige()) + "', " +
            "'" + std::to_string(card->getCrowns()) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::BLANC)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::VERT)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::NOIR)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::PERLE)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::ROUGE)) + "', " +
            "'" + std::to_string(card->getCost().at(TokenColor::BLEU)) + "', " +
            "'" + toString(card->getBonus().bonus_color) + "', " +
            "'" + std::to_string(card->getLevel()) + "', " +
            "'" + toString(card->getAbility1()) + "', " +
            "'" + toString(card->getAbility2()) + "', " +
            "'" + std::to_string(card->getBonus().bonus_number) + "');";
        //std::cout << insertQuery << std::endl;
        // Execution de la requete sql
        rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
            std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
        }
    }

    // Stockage du deck royal

    const std::string& tableDR = "boardRoyal";
    cleanTable(pathtodatabase, tableDR);

    for (size_t i = 0; i < Deck_Royal::getInstance()->getCards().size(); ++i) {
        RoyalCard& card = *Deck_Royal::getInstance()->getCards()[i];

        // Requete sql pour écrire dans la database
        std::string insertQuery = std::string("INSERT INTO boardRoyal (idCard, ability, prestige) VALUES (") +
            "'" + std::to_string(card.getId()) + "', " +
            "'" + toString(card.getAbility()) + "', " +
            "'" + std::to_string(card.getPrestige()) + "');";


        // Execution de la requete sql
        rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
            std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
        }
    }

    // Stockage des informations du joueur dans la table player de la database save.db

    const std::string& tablePL = "player";
    cleanTable(pathtodatabase, tablePL);

    for (size_t i = 0; i < 2; ++i) { // il y a deux joueurs donc on répète les opérations 2 fois
        Player* player = game.getPlayer(i);

        if (player) {
            std::string playerName = player->getName();
            Type playerType = player->getType();
            int playerPrivileges = player->getNbPrivilege();
            int playerPrestiges = player->getPrestige();
            int playerCrowns = player->getCrowns();
            SummaryCard& playerBleu = player->getBlueSummary();
            SummaryCard& playerBlanc = player->getWhiteSummary();
            SummaryCard& playerVert = player->getGreenSummary();
            SummaryCard& playerNoir = player->getBlackSummary();
            SummaryCard& playerRouge= player->getRedSummary();
            std::unordered_map<TokenColor, int> tokenSummary = player->getTokenSummary();

            // Requete sql pour écrire dans la database
            std::string insertQuery = std::string("INSERT INTO player (idPlayer, name, typePlayer, privileges, prestiges, crowns, tokenBlue, tokenWhite, tokenGreen, tokenBlack, tokenRed, tokenPerle, tokenGold, SummaryBlueBonusNb, SummaryBluePrestigeNb, SummaryWhiteBonusNb, SummaryWhitePrestigeNb, SummaryGreenBonusNb, SummaryGreenPrestigeNb, SummaryBlackBonusNb, SummaryBlackPrestigeNb, SummaryRedBonusNb, SummaryRedPrestigeNb) VALUES (") +
                "'" + std::to_string(i) + "', " +
                "'" + playerName + "', " +
                "'" + toString(playerType) + "', " +
                "'" + std::to_string(playerPrivileges) + "', " +
                "'" + std::to_string(playerPrestiges) + "', " +
                "'" + std::to_string(playerCrowns) + "', " +
                "'" + std::to_string(tokenSummary[TokenColor::BLEU]) + "', " +
                "'" + std::to_string(tokenSummary[TokenColor::BLANC]) + "', " +
                "'" + std::to_string(tokenSummary[TokenColor::VERT]) + "', " +
                "'" + std::to_string(tokenSummary[TokenColor::NOIR]) + "', " +
                "'" + std::to_string(tokenSummary[TokenColor::ROUGE]) + "', " +
                "'" + std::to_string(tokenSummary[TokenColor::PERLE]) + "', " +
                "'" + std::to_string(tokenSummary[TokenColor::OR]) + "', " +
                "'" + std::to_string(playerBleu.getBonusNumber()) + "', " +
                "'" + std::to_string(playerBleu.getPrestigePoints()) + "', " +
                "'" + std::to_string(playerBlanc.getBonusNumber()) + "', " +
                "'" + std::to_string(playerBlanc.getPrestigePoints()) + "', " +
                "'" + std::to_string(playerVert.getBonusNumber()) + "', " +
                "'" + std::to_string(playerVert.getPrestigePoints()) + "', " +
                "'" + std::to_string(playerNoir.getBonusNumber()) + "', " +
                "'" + std::to_string(playerNoir.getPrestigePoints()) + "', " +
                "'" + std::to_string(playerRouge.getBonusNumber()) + "', " +
                "'" + std::to_string(playerRouge.getPrestigePoints())
            + "');";
            // Execution de la requete sql
            rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

            if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
                std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
            }
        }
    }

    // Stockage des cartes réservées

    const std::string& tableCR = "reservedCards";
    cleanTable(pathtodatabase, tableCR);

    for (size_t i = 0; i < 2; ++i) { // il y a deux joueurs donc on répète les opérations 2 fois
        Player* player = game.getPlayer(i);

        for (auto card : player->getReserve()) {


            std::string insertQuery = std::string("INSERT INTO reservedCards (idPlayer, idCard, prestige, crown, cost_w, cost_v, cost_n, cost_p, cost_r, cost_b, color, level, ability1, ability2, bonus_nb) VALUES (") +
                "'" + std::to_string(i) + "', " +
                "'" + std::to_string(card->getId()) + "', " +
                "'" + std::to_string(card->getPrestige()) + "', " +
                "'" + std::to_string(card->getCrowns()) + "', " +
                "'" + std::to_string(card->getCost().at(TokenColor::BLANC)) + "', " +
                "'" + std::to_string(card->getCost().at(TokenColor::VERT)) + "', " +
                "'" + std::to_string(card->getCost().at(TokenColor::NOIR)) + "', " +
                "'" + std::to_string(card->getCost().at(TokenColor::PERLE)) + "', " +
                "'" + std::to_string(card->getCost().at(TokenColor::ROUGE)) + "', " +
                "'" + std::to_string(card->getCost().at(TokenColor::BLEU)) + "', " +
                "'" + toString(card->getBonus().bonus_color) + "', " +
                "'" + std::to_string(card->getLevel()) + "', " +
                "'" + toString(card->getAbility1()) + "', " +
                "'" + toString(card->getAbility2()) + "', " +
                "'" + std::to_string(card->getBonus().bonus_number) + "');";

            // Execution de la requete sql
            rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

            if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
                std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
            }
        }
    }

    // Stockage des jetons du plateau dans la table boardToken de la database save.db

    const std::string& tableBT = "boardToken";
    cleanTable(pathtodatabase, tableBT);

    Board::BoardIterator it = Board::getInstance()->iterator();
    while (it.hasNext()) {

        const Token* token = it.next();

        if (token) {
            TokenColor color = token->getColor();
            int posX;
            int posY;
            if( it.getCol() == 0){
                posY = 4;
                posX = it.getRow()-1;
            }
            else {
                posY = it.getCol()-1;
                posX = it.getRow();
            }
            // Requete sql pour écrire dans la database
            std::string insertQuery = std::string("INSERT INTO boardToken (xToken, yToken, colorToken) VALUES (") +
                "'" + std::to_string(posX) + "', " +
                "'" + std::to_string(posY) + "', " +
                "'" + toString(color) + "');";

            // Execution de la requete sql
            rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

            if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
                std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
            }
        }
    }

    // Stockage des jetons du sac dans la table bag de la database save.db

    const std::string& tableBag = "bag";
    cleanTable(pathtodatabase, tableBag);

    std::map<TokenColor, size_t> colorCounts;  // Map asociant chaque couleur de jeton au nombre d'instances de ce type de couleur

    Bag::BagIterator itbag = Bag::getInstance()->iterator();
    while (itbag.hasNext()) {
        const Token* tokenbag = itbag.next();
        TokenColor color = tokenbag->getColor();

        // Update du nombre de jeton pour cette couleur
        colorCounts[color]++;
    }

    // Requete sql pour écrire le nombre de tokens de chaque couleur dans la database
    std::string insertCountQuery = std::string("INSERT INTO bag (tokenBlue, tokenWhite, tokenGreen, tokenBlack, tokenRed, tokenPerle, tokenGold, tokenNone) VALUES (") +
        "'" + std::to_string(colorCounts[TokenColor::BLEU]) + "', " +
        "'" + std::to_string(colorCounts[TokenColor::BLANC]) + "', " +
        "'" + std::to_string(colorCounts[TokenColor::VERT]) + "', " +
        "'" + std::to_string(colorCounts[TokenColor::NOIR]) + "', " +
        "'" + std::to_string(colorCounts[TokenColor::ROUGE]) + "', " +
        "'" + std::to_string(colorCounts[TokenColor::PERLE]) + "', " +
        "'" + std::to_string(colorCounts[TokenColor::OR]) + "', " +
        "'" + std::to_string(colorCounts[TokenColor::None]) + "');";

    // Execution de la requete sql
    rc = sqlite3_exec(db, insertCountQuery.c_str(), nullptr, nullptr, nullptr);

    if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
        std::cerr << "Error executing count query: " << sqlite3_errmsg(db) << std::endl;
    }

    // Stockage des informations de la partie dans la table infopartie de la database save.db

    const std::string& tablePartie = "infopartie";
    cleanTable(pathtodatabase, tablePartie);

    // Requete sql pour écrire dans la database
    std::string insertQuery = std::string("INSERT INTO infopartie (turn, currentPlayer, opposingPlayer, nbPrivileges) VALUES (") +
        "'" + std::to_string(game.getRound()) + "', " +
        "'" + game.getPlayer(0)->getName() + "', " +
        "'" + game.getPlayer(1)->getName() + "', " +
        "'" + std::to_string(Board::getInstance()->getNbPrivileges()) + "');";

    // Execution de la requete sql
    rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

    if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
        std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_close(db);
}

Pyramid_Cards& retrevePyramid() {
    // Obtient le chemin du fichier source actuel (__FILE__)
    std::filesystem::path sourceFilePath = std::filesystem::path(__FILE__);
    // Obtient le répertoire du fichier source
    std::filesystem::path sourceDirectory = sourceFilePath.parent_path();
    std::string databaseSavePath = sourceDirectory.string() + "/Data/save.db";

    // Reconstruction de la pyramide
    Pyramid_Cards pyramid(databaseSavePath);

    return pyramid;
}
