//
// Fichier de gestion des jetons, et du sac de jetons
//
#include <iostream>
#include <vector>
#include <array>

#ifndef LO21PROJECT_JETON_H
#define LO21PROJECT_JETON_H

//Liste des couleurs possibles pour les jetons, avec l'affichage
enum class TokenColor { BLEU, BLANC, VERT, NOIR, ROUGE, PERLE, OR, None };
std::string toString(TokenColor c);
std::ostream& operator<<(std::ostream& f, TokenColor c);


class TokenException {
    //Classe de gestion des exception avec les jetons
private:
    std::string message; //Message d'exception
public:
    TokenException(const std::string &msg) : message(msg) {} //Constructeur d'exception

    ~TokenException() = default;

    std::string getMessage() const { return message; } //Récupération du message d'exception
};

class Token {
    //Classe Jeton qui permet de gérer au détail chaque pièce
private :
    const TokenColor color;
public :
    explicit Token(TokenColor c) : color(c) {} //Constructeur de jeton avec une couleur
    Token(const Token& j) : color(j.color) {} //Constructeur de copie
    TokenColor getColor() const { return color; } //Récupération de la couleur du jeton
};
std::ostream& operator<< (std::ostream& f, const Token& token);

class TotalTokens{
    //On définit initialement le maximum de jetons de chaque couleur possible dans une partie
private :
    std::vector<const Token*> tokens; //Liste des jetons
    const size_t max_or = 3;       //Nombre maximum de jetons de chaque couleur
    const size_t max_perle = 2;
    const size_t max_rouge = 4;
    const size_t max_noir = 4;
    const size_t max_bleu = 4;
    const size_t max_blanc = 4;
    const size_t max_vert = 4;
public:
    size_t getNbTokens() const { return tokens.size(); } //Récupération du nombre de jetons
    const Token& getToken(size_t i) const; //Récupération d'un jeton
    TotalTokens();
    ~TotalTokens();

    //Le total initial est unique, pas de duplication
    TotalTokens(const TotalTokens&) = delete;
    TotalTokens& operator=(const TotalTokens&) = delete;
};

class Bag {
    //Un sac est l'ensemble des jetons qui ne sont pas encore sur le plateau ou dans la réserve des joueurs
    //Lorsque la partie commence, le sac est vide car tous les jetons sont sur le plateau
private :
    std::vector<const Token*> tokens; //Liste des jetons
public :
    explicit Bag(const TotalTokens& total); //Constructeur de sac avec tous les jetons

    size_t getNbTokens() const { return tokens.size(); } //Récupération du nombre de jetons
    void addToken(const Token& j); //Ajout d'un jeton dans le sac
    const Token& drawToken(); //Pioche d'un jeton dans le sac
    bool isEmpty() const { return tokens.empty(); } //Vérification si le sac est vide

    //Le sac de jeton est unique, pas de duplication
    Bag(const Bag&) = delete;
    Bag& operator=(const Bag&) = delete;
};
//Les privilèges sont 3 objets indépendants entre eux, mais identiques
class Privilege {
};

class TotalPrivileges {
private :
    std::vector<const Privilege*> privileges; //3 privilièges dans la liste
public :
    size_t getNbPrivileges() const { return privileges.size(); } //Récupération du nombre de privilèges
    const Privilege& getPrivilege(size_t i) const; //Récupération d'un privilège
    TotalPrivileges();
    ~TotalPrivileges();

    //pas de duplication du total
    TotalPrivileges(const TotalPrivileges&) = delete;
    TotalPrivileges& operator=(const TotalPrivileges&) = delete;
};


class Board {
    //Classe plateau contenant les jetons et les privilèges en jeu
private :
    std::array<std::array<const Token*, 5>, 5> tokens{}; //matrice de 5*5 pouvant être vide ou contenir un jeton
    std::vector<const Privilege*> privileges; //Liste des privilèges
public :
    Board(Bag& bag, const TotalPrivileges& total); //Instanciation du plateau avec tous les jetons et le sac de jetons
    const Token& takeToken(size_t i, size_t j); //Récupération d'un jeton sur le plateau, supprimé du plateau, à l'indice i,j
    void showBoard();   //Affichage du plateau
    const Privilege& takePrivilege(); //Récupération d'un privilège
    void placeToken(const Token& token);    //Placement d'un jeton sur le plateau
    void placePrivilege(const Privilege& privilege); //Placement d'un privilège sur le plateau
    void fillBoard(Bag& bag); //Remplissage du plateau avec les jetons du sac
    bool isEmpty() const; //Vérification si le plateau est vide

    //pas de duplication du plateau
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;
};

#endif //LO21PROJECT_JETON_H