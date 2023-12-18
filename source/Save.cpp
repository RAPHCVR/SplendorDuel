#include "Save.h"

void Save::writeToDatabase(const Game& game) {
    sqlite3* db;
    int rc = sqlite3_open(save_file.c_str(), &db);

    if (rc != SQLITE_OK) { //si il y a une erreur lors de l'ouverture de la database, on renvoie un message d'ereur
        std::cerr << "Error opening the database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    /*--------------------------------------------------------------------------*/

    // Stockage de la pyramide
    for (auto card : game.getGameTable().getPyramid()) {
    /*
    for (size_t i = 0; i < pyramid.getSize(); ++i) {
        const JewelryCard& card = pyramid_cards.getCard(i);
*/
        // Requete sql pour écrire dans la database
        std::string insertQuery = "INSERT INTO pyramid (idCard, prestige, crown, cost_w, cost_v, cost_n, cost_p, cost_r, cost_b, color, position, ability1, ability2, bonus_nb) VALUES (" +
            std::to_string(card.getId()) + ", " +
            std::to_string(card.getPrestige()) + ", " +
            std::to_string(card.getCrowns()) + ", " +
            std::to_string(card.getCost(TokenColor::BLANC)) + ", " +
            std::to_string(card.getCost(TokenColor::VERT)) + ", " +
            std::to_string(card.getCost(TokenColor::NOIR)) + ", " +
            std::to_string(card.getCost(TokenColor::PERLE)) + ", " +
            std::to_string(card.getCost(TokenColor::ROUGE)) + ", " +
            std::to_string(card.getCost(TokenColor::BLEU)) + ", " +
            std::to_string(card.getColor()) + ", " +
            std::to_string(card.getLevelCards()) + " +
            std::to_string(card.getAbility1()) + ", " +
            std::to_string(card.getAbility2()) + ", " +
            std::to_string(card.getBonus()) + ");";

        // Execution de la requete sql
        rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
            std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
        }
    }

    /*--------------------------------------------------------------------------*/

    // Stockage du deck royal
    for (size_t i = 0; i < Deck_Royal.getSize(); ++i) {
        const RoyalCard& card = Deck_Royal.getCard(i);

        // Requete sql pour écrire dans la database
        std::string insertQuery = "INSERT INTO boardRoyal (idCard) VALUES (" +
            std::to_string(card.getId()) + ");";

        // Execution de la requete sql
        rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
            std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
        }
    }

    /*--------------------------------------------------------------------------*/

    // Stockage des informations du joueur dans la table player de la database save.db
    for (size_t i = 0; i < 2; ++i) { // il y a deux joueurs donc on répète les opérations 2 fois
        const Player* player = game.getPlayer(i);

        if (player) {
            std::string playerName = player->getName();
            std::string playerType = player->getType();
            int playerPrivileges = player->getNbPrivilege();
            int playerPrestiges = player->getPrestige();
            int playerCrowns = player->getCrowns();
            SummaryCard& playerBlue = player->getBlueSummary();
            SummaryCard& playerWhite = player->getWhiteSummary();
            SummaryCard& playerGreen = player->getGreenSummary();
            SummaryCard& playerBlack = player->getBlackSummary();
            SummaryCard& playerRed = player->getRedSummary();

            // Requete sql pour écrire dans la database
            std::string insertQuery = "INSERT INTO player (idPlayer, name, type, privileges, prestiges, crowns, tokenBlue, tokenWhite, tokenGreen, tokenBlack, tokenRed, tokenPerle, tokenGold) VALUES ('" +
                i + "', '" +
                playerName + "', '" + 
                playerType + "', " +
                std::to_string(playerPrivileges) + ", " +
                std::to_string(playerPrestiges) + ", " +
                std::to_string(playerCrowns) + ", " +
                std::to_string(playerBlue) + ", " +
                std::to_string(playerWhite) + ", " +
                std::to_string(playerGreen) + ", " +
                std::to_string(playerBlack) + ", " +
                std::to_string(playerRed) + ");";

            // Execution de la requete sql
            rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

            if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
                std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
            }
        }
    }

    /*--------------------------------------------------------------------------*/

    // Stockage des jetons du plateau dans la table boardToken de la database save.db
    Board::BoardIterator it = iterator();
    while (it.hasNext()) {

        const Token* token = it.next();

        if (token) {
            TokenColor color = token->getColor(); 
            size_t posX = it.getCol(); 
            size_t posY = it.getRow(); 

            // Requete sql pour écrire dans la database
            std::string insertQuery = "INSERT INTO boardToken (xToken, yToken, colorToken) VALUES (" +
                std::to_string(posX) + ", " + 
                std::to_string(posY) + ", '" + 
                toString(color) + "');";

            // Execution de la requete sql
            rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

            if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
                std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
            }
        }
    }

    /*
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 5; ++j) {
            const Token* token = game.getTokenAt(i, j);

            if (token) {
                TokenColor color = token->getColor();
                size_t posX = i;
                size_t posY = j;

                // Requete sql pour écrire dans la database
                std::string insertQuery = "INSERT INTO boardToken (xToken, yToken, colorToken) VALUES (" +
                    std::to_string(posX) + ", " +
                    std::to_string(posY) + " +
                    std::to_string(static_cast<int>(color)) + ");";

                // Execution de la requete sql
                rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

                if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
                    std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
                }
            }
        }
    } */

    /*--------------------------------------------------------------------------*/

    // Stockage des informations de la partie dans la table infopartie de la database save.db
    // Requete sql pour écrire dans la database
    std::string insertQuery = "INSERT INTO infopartie (turn, currentPlayer, opposingPlayer) VALUES ('" +
        std::to_string(game.getTurn()) + ", '" +
        currentPlayer.getName() + "', '" +
        opposingPlayer.getName() + "');";

        // Execution de la requete sql
        rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

    if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
        std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_close(db);
}