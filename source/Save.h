#ifndef LO21PROJECT_SAVE_H
#define LO21PROJECT_SAVE_H

#include <iostream> //a voir sans
#include <fstream> 
#include <string>
#include "Partie.h"
#include "sqlite/sqlite3.h"

using namespace std;

string const save_file("/Data/save.db");

// M�thode pour effacer toutes les donn�es du fichier save.db

// M�thode pour sauvegarder la partie

// M�thode pour affecter les donn�es de l'ancienne partie



ofstream monFlux(save_file.c_str());
//D�claration d'un flux permettant d'�crire dans le fichier save_file.



#endif //LO21PROJECT_SAVE_H