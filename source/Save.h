#ifndef LO21PROJECT_SAVE_H
#define LO21PROJECT_SAVE_H

#include <iostream> //a voir sans
#include <fstream> 
#include <string>
#include "Partie.h"
#include "sqlite/sqlite3.h"

class Save {
public:
    static void writeToDatabase(const Game& game);
};


#endif //LO21PROJECT_SAVE_H