//
// Fichier de gestion des jetons, et du sac de jetons
//
#include <iostream>
#include <vector>
#include <array>

#ifndef LO21PROJECT_JETON_H
#define LO21PROJECT_JETON_H

//Liste des couleurs possibles pour les jetons, avec l'affichage
enum class TokenColor { BLEU, BLANC, VERT, NOIR, ROUGE, PERLE, OR };
std::string toString(TokenColor c);
std::ostream& operator<<(std::ostream& f, TokenColor c);


class TokenException {
    //Classe de gestion des exception avec les jetons
private:
    std::string message;
public:
    TokenException(const std::string &msg) : message(msg) {}

    ~TokenException() = default;

    std::string getMessage() const { return message; }
};

class Token {
    //Classe Jeton qui permet de gérer au détail chaque pièce
private :
    const TokenColor color;
public :
    explicit Token(TokenColor c) : color(c) {}
    Token(const Token& j) : color(j.color) {}
    TokenColor getColor() const { return color; }
};
std::ostream& operator<< (std::ostream& f, const Token& token);

class TotalTokens{
    //On définit initialement le maximum de jetons de chaque couleur possible dans une partie
private :
    std::vector<const Token*> tokens;
    const size_t max_or = 3;
    const size_t max_perle = 2;
    const size_t max_rouge = 4;
    const size_t max_noir = 4;
    const size_t max_bleu = 4;
    const size_t max_blanc = 4;
    const size_t max_vert = 4;
public:
    size_t getNbTokens() const { return tokens.size(); }
    const Token& getToken(size_t i) const;
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
    std::vector<const Token*> tokens;
public :
    explicit Bag(const TotalTokens& lot);

    size_t getNbTokens() const { return tokens.size(); }
    void addToken(const Token& j);
    const Token& drawToken();
    bool isEmpty() const { return tokens.empty(); }

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
    size_t getNbPrivileges() const { return privileges.size(); }
    const Privilege& getPrivilege(size_t i) const;
    TotalPrivileges();
    ~TotalPrivileges();

    //pas de duplication du lot
    TotalPrivileges(const TotalPrivileges&) = delete;
    TotalPrivileges& operator=(const TotalPrivileges&) = delete;
};


class Board {
    //Classe plateau contenant les jetons et les privilèges en jeu
private :
    std::array<std::array<const Token*, 5>, 5> tokens{}; //matrice de 5*5 pouvant être vide ou contenir un jeton
    std::vector<const Privilege*> privileges;
public :
    Board(Bag& bag, const TotalPrivileges& total); //Instanciation du plateau avec tous les jetons et le sac de jetons
    const Token& takeToken(size_t i, size_t j);
    void showBoard();
    const Privilege& takePrivilege();
    void placeToken(const Token& token);
    void placePrivilege(const Privilege& privilege);
    void fillBoard(Bag& bag);
    bool isEmpty() const;

    //pas de duplication du plateau
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;
};

#endif //LO21PROJECT_JETON_H