#ifndef LO21PROJECT_SAVE_H
#define LO21PROJECT_SAVE_H

#include <iostream> //a voir sans
#include <fstream> 
#include <string>
#include "Partie.h"
#include "sqlite/sqlite3.h"

using namespace std;

string const save_file("/Data/save.db");

// Méthode pour effacer toutes les données du fichier save.db

// Méthode pour sauvegarder la partie

// Méthode pour affecter les données de l'ancienne partie



ofstream monFlux(save_file.c_str());
//Déclaration d'un flux permettant d'écrire dans le fichier save_file.



#endif //LO21PROJECT_SAVE_H