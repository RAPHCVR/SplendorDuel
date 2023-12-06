//
// Fichier de gestion des jetons, et du sac de jetons
//
#ifndef LO21PROJECT_JETON_H
#define LO21PROJECT_JETON_H

#include <iostream>
#include <vector>
#include <array>

//Liste des couleurs possibles pour les jetons, avec l'affichage
enum class TokenColor { BLEU, BLANC, VERT, NOIR, ROUGE, PERLE, OR, None };
std::string toString(TokenColor c);
std::ostream& operator<<(std::ostream& f, TokenColor c);


class TokenException {
    //Classe de gestion des exception avec les jetons
private:
    std::string message; //Message d'exception
public:
    explicit TokenException(const std::string &msg) : message(msg) {} //Constructeur d'exception

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
    //design pattern singleton
    TotalTokens(); //Constructeur du total
    ~TotalTokens(); //Destructeur du total
public:
    //Le total initial est unique, pas de duplication
    TotalTokens& operator=(const TotalTokens&) = delete;
    TotalTokens(const TotalTokens&) = delete;

    size_t getNbTokens() const { return tokens.size(); } //Récupération du nombre de jetons
    const Token& getToken(size_t i) const; //Récupération d'un jeton
    const static TotalTokens& getInstance(); //Récupération de l'instance unique
};

class Bag {
    //Un sac est l'ensemble des jetons qui ne sont pas encore sur le plateau ou dans la réserve des joueurs
    //Lorsque la partie commence, le sac est vide car tous les jetons sont sur le plateau
private :
    std::vector<const Token*> tokens; //Liste des jetons
    //design pattern singleton
    explicit Bag(const TotalTokens& total); //Constructeur de sac avec tous les jetons
public :
    //Le sac de jeton est unique, pas de duplication
    Bag(const Bag&) = delete;
    Bag& operator=(const Bag&) = delete;

    static Bag& getInstance(); //Récupération de l'instance unique
    size_t getNbTokens() const { return tokens.size(); } //Récupération du nombre de jetons
    void addToken(const Token& j); //Ajout d'un jeton dans le sac
    const Token& drawToken(); //Pioche d'un jeton dans le sac
    bool isEmpty() const { return tokens.empty(); } //Vérification si le sac est vide
    bool containsOnly(TokenColor color) const; //Vérification si le sac ne contient que des jetons d'une couleur
};

//Les privilèges sont 3 objets indépendants entre eux, mais identiques
class Privilege {
};

class TotalPrivileges {
    //On définit initialement le maximum de privilèges dans une partie
private :
    std::array<const Privilege*, 3> privileges; //3 privilièges dans la liste
    //design pattern singleton
    TotalPrivileges(); //Constructeur du total
    ~TotalPrivileges(); //Destructeur du total

public :
    //pas de duplication du total
    TotalPrivileges(const TotalPrivileges&) = delete;
    TotalPrivileges& operator=(const TotalPrivileges&) = delete;

    size_t getNbPrivileges() const { return privileges.size(); } //Récupération du nombre de privilèges
    const Privilege& getPrivilege(size_t i) const; //Récupération d'un privilège
    const static TotalPrivileges& getInstance(); //Récupération de l'instance unique
};

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update() = 0;
};

class Board {
    //Classe plateau contenant les jetons et les privilèges en jeu
private :
    std::array<const Privilege*, 3> privileges{}; //Liste des privilèges
    std::array<std::array<const Token*, 5>, 5> tokens{}; //matrice de 5*5 pouvant être vide ou contenir un jeton
    std::vector<Observer*> observers;
    //design pattern singleton
    Board(); //Instanciation du plateau avec tous les jetons et le sac de jetons (Constructeur)
public :
    //pas de duplication du plateau
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

    const Token& takeToken(size_t i, size_t j); //Récupération d'un jeton sur le plateau, supprimé du plateau, à l'indice i,j

    static Board& getInstance(); //Récupération de l'instance unique

    class BoardIterator {
        //Classe itérateur pour parcourir le plateau
    private:
        Board& board;
        size_t row, col;

    public:
        explicit BoardIterator(Board& board) : board(board), row(0), col(0) {} //Constructeur de l'itérateur
        bool hasNext() const {
            return row < board.tokens.size() && col < board.tokens[row].size(); //Vérification si il y a un jeton suivant
        }
        const Token* next(); //Récupération du jeton suivant
    };

    BoardIterator iterator() { //Récupération de l'itérateur du plateau
        return BoardIterator(*this);
    }

    void showBoard();   //Affichage du plateau
    const Privilege& takePrivilege(); //Récupération d'un privilège
    void placeToken(const Token& token);    //Placement d'un jeton sur le plateau
    void placePrivilege(const Privilege& privilege); //Placement d'un privilège sur le plateau
    void fillBoard(Bag& bag); //Remplissage du plateau avec les jetons du sac
    bool isEmpty() const; //Vérification si le plateau est vide
    unsigned int getNbPrivileges() const; //Récupération du nombre de privilèges
    bool hasTokenOfColor(TokenColor color) const; //verification de la presence d'une couleur sur le plateau
    bool containsOnly(TokenColor color) const; //verification de la presence d'une couleur uniquement sur le plateau

    //Observer
    void registerObserver(Observer* observer) {
        observers.push_back(observer);
    }
    void notifyObservers() {
        for (Observer* observer : observers) {
            observer->update();
        }
    }

    // Call this method whenever a token is taken or a privilege is changed
    void actionPerformed() {
        // Perform the action...
        notifyObservers();
    }
};

#endif //LO21PROJECT_JETON_H
