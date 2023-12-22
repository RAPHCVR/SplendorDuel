#ifndef LO21PROJECT_SAVE_H
#define LO21PROJECT_SAVE_H

#include <iostream> //a voir sans
#include <fstream> 
#include <string>
#include "Partie.h"
#include "Controller.h"
#include "Cards.h"
#include "sqlite/sqlite3.h"

void cleanTable(const std::string& dbPath, const std::string& tableName);
void writeToDatabase(const Game& game);
Pyramid_Cards* retrevePyramid();

#endif //LO21PROJECT_SAVE_H