#include "Save.h"

void Save::writeToDatabase(const Game& game) {
    sqlite3* db;
    int rc = sqlite3_open(save_file.c_str(), &db);

    if (rc != SQLITE_OK) { //si il y a une erreur lors de l'ouverture de la database, on renvoie un message d'ereur
        std::cerr << "Error opening the database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // Stocage de la pyramide
    for (size_t i = 0; i < pyramid.getSize(); ++i) {
        const JewelryCard& card = pyramid.getCard(i);

        // Requete sql pour écrire dans la database
        std::string insertQuery = "INSERT INTO pyramid (id, position) VALUES (" +
            std::to_string(card.getId()) + ", " +
            std::to_string(card.getLevel()) + ");";

        // Execution de la requete sql
        rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK) {
            std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
        }
    }

    // Stocage du deck royal
    for (size_t i = 0; i < Deck_Royal.getSize(); ++i) {
        const RoyalCard& card = Deck_Royal.getCard(i);

        // Requete sql pour écrire dans la database
        std::string insertQuery = "INSERT INTO boardRoyal (idCard) VALUES (" +
            std::to_string(card.getId()) + ");";

        // Execution de la requete sql
        rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

        if (rc != SQLITE_OK) {
            std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
        }
    }

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
            // Requete sql pour écrire dans la database
            std::string insertQuery = "INSERT INTO player (name, type, privileges, prestiges, crowns, tokenBlue, tokenWhite, tokenGreen, tokenBlack, tokenRed, tokenPerle, tokenGold) VALUES ('" +
                playerName + "', '" + 
                playerType + "', " +
                std::to_string(playerPrivileges) + ", " +
                std::to_string(playerPrestiges) + ", " +
                std::to_string(playerCrowns) + ", " +
                std::to_string(playerBlue) + ", " +
                std::to_string(playerWhite) + ", " +
                std::to_string(playerGreen) + ", " +
                std::to_string(playerBlack) + ", " +
                std::to_string(playerRed) + "
                );";

            // Execution de la requete sql
            rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);

            if (rc != SQLITE_OK) { // si il y a une erreur lors de l'écriture des données, on renvoie un message d'erreur
                std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
            }
        }
    }

    sqlite3_close(db);
}