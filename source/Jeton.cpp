//
// Created by utcpret on 08/11/2023.
//

#include "Jeton.h"

#include <algorithm>
#include <math.h>

Board* Board::instance = nullptr;

// In the corresponding .cpp file for the Bag class
Bag* Bag::instance = nullptr;

std::string toString(TokenColor c) {
    switch (c) {
        case TokenColor::BLANC: return "BLANC";
        case TokenColor::BLEU: return "BLEU";
        case TokenColor::NOIR: return "NOIR";
        case TokenColor::OR: return "OR";
        case TokenColor::PERLE: return "PERLE";
        case TokenColor::ROUGE: return "ROUGE";
        case TokenColor::VERT: return "VERT";
        case TokenColor::None: return "VIDE";
        default: throw std::invalid_argument("Couleur inconnue");
    }
}



TokenColor toTokenColor(std::string s) {
    if (s==("BLEU")) {
        return TokenColor::BLEU;
    }
    else if (s==("BLANC")) {
        return TokenColor::BLANC;
    }
    else if (s==("VERT")) {
        return TokenColor::VERT;
    }
    else if (s==("NOIR")) {
        return TokenColor::NOIR;
    }
    else if (s==("ROUGE")) {
        return TokenColor::ROUGE;
    }
    else if (s==("PERLE")) {
        return TokenColor::PERLE;
    }
    else if (s==("OR")) {
        return TokenColor::OR;
    }
    else {
        // Gérer le cas où la chaîne ne correspond à aucune couleur connue
        return TokenColor::None;
    }
}

std::ostream& operator<<(std::ostream& f, TokenColor c) {
    return f << toString(c);
}

std::ostream& operator<< (std::ostream& f, const Token& token) {
    return f << token.getColor();
}

TotalTokens::TotalTokens() {
    //On ajoute les jetons dans l'ordre de la couleur
    for (size_t i = 0; i < max_bleu; i++) {
        tokens.push_back(new Token(TokenColor::BLEU));
    }
    for (size_t i = 0; i < max_blanc; i++) {
        tokens.push_back(new Token(TokenColor::BLANC));
    }
    for (size_t i = 0; i < max_vert; i++) {
        tokens.push_back(new Token(TokenColor::VERT));
    }
    for (size_t i = 0; i < max_noir; i++) {
        tokens.push_back(new Token(TokenColor::NOIR));
    }
    for (size_t i = 0; i < max_rouge; i++) {
        tokens.push_back(new Token(TokenColor::ROUGE));
    }
    for (size_t i = 0; i < max_perle; i++) {
        tokens.push_back(new Token(TokenColor::PERLE));
    }
    for (size_t i = 0; i < max_or; i++) {
        tokens.push_back(new Token(TokenColor::OR));
    }
}

TotalTokens::~TotalTokens() {
    for (auto & token : tokens) {
        delete token;
    }
}

const TotalTokens& TotalTokens::getInstance() {
    static TotalTokens instance;
    return instance;
}

const Token& TotalTokens::getToken(size_t i) const {
    if (i >= tokens.size()) {
        throw TokenException("Indice de jeton incorrect");
    }
    return *tokens[i];
}

Bag::Bag(const TotalTokens& total) {
    //On ajoute les jetons dans l'ordre de la couleur
    for (size_t i = 0; i < total.getNbTokens(); i++) {
        tokens.push_back(&total.getToken(i));
    }
}


void Bag::addToken(const Token &j) {
    tokens.push_back(&j);
}

const Token& Bag::drawToken() {
    if (tokens.empty()) {
        throw TokenException("Le sac est vide");
    }
    srand(time(0));
    size_t i = rand() % tokens.size();
    const Token& j = *tokens[i];
    tokens.erase(tokens.begin() + i);
    return j;
}

bool Bag::containsOnly(TokenColor color) {
    std::all_of(tokens.begin(), tokens.end(), [color](const Token* token) {
        return token->getColor() == color;
    });
}


std::ostream& operator<< (std::ostream& f, const Privilege& privilege) {
    return f << "Privilege" << std::endl;
}

const Privilege& TotalPrivileges::getPrivilege(size_t i) const {
    if (i >= privileges.size()) {
        throw TokenException("Indice de privilege incorrect");
    }
    return *privileges[i];
}

TotalPrivileges::TotalPrivileges() {
    for (size_t i = 0; i < 3; i++) {
        privileges[i]=new Privilege();
    }
}

const TotalPrivileges& TotalPrivileges::getInstance() {
    static TotalPrivileges instance;
    return instance;
}

TotalPrivileges::~TotalPrivileges() {
    for (auto & privilege : privileges) {
        delete privilege;
    }
}
unsigned int Board::getNbPrivileges() const {
    unsigned int nb = privileges.size();
    for (size_t i = 0; i < privileges.size(); i++) {
        if (privileges[i] == nullptr) {
            nb--;
        }
    }
    return nb;
}
void Board::placePrivilege(const Privilege& privilege) {
    //place le privilege au bon endroit dans le tableau de privileges sur un array
    unsigned int nb = getNbPrivileges();
    if (nb < 3) {
        privileges[nb]=&privilege;
    }
    else
    throw TokenException("Il n'y a plus de place pour un privilège");
}

void Board::showBoard(){
    std::cout << "Plateau:" << std::endl;

    BoardIterator it = iterator();
    size_t i = 0;
    while (it.hasNext()) {
        if (i % 5 == 0) {
            std::cout << std::endl;
        }
        const Token* token = it.next();
        std::string s = "";
        if (token == nullptr) s = "."; // Cas de la case vide
        else {
            switch (token->getColor()) {
                case TokenColor::BLANC: s = "W"; break;
                case TokenColor::BLEU: s = "B"; break;
                case TokenColor::NOIR: s = "N"; break;
                case TokenColor::OR: s = "O"; break;
                case TokenColor::PERLE: s = "P"; break;
                case TokenColor::ROUGE: s = "R"; break;
                case TokenColor::VERT: s = "V"; break;
            }
        }
        std::cout << s << " ";
        i++;
    }
    std::cout << std::endl;
}


void Board::placeToken(const Token &token) {
    size_t rows = tokens.size();
    size_t cols = tokens[0].size();
    size_t x = 0; // current position; x
    size_t y = 0; // current position; y
    int d = 3; // current direction; 0=RIGHT, 1=DOWN, 2=LEFT, 3=UP
    int s = 1; // chain size
    bool ok = false;
    x = (size_t) round(rows/2);
    y = (size_t) round(cols/2);
    for (int k=1; k<=(rows-1); k++)
    {
        for (int j=0; j<(k<(cols-1)?2:3); j++)
        {
            if (ok) break;

            for (int i=0; i<s; i++)
            {
                if (tokens[x][y] == nullptr) {
                    tokens[x][y] = &token;
                    ok=true;
                    break;
                }

                switch (d)
                {
                    case 0: y = y + 1; break;
                    case 1: x = x + 1; break;
                    case 2: y = y - 1; break;
                    case 3: x = x - 1; break;
                }
            }
            d = (d+1)%4;
        }
        s = s + 1;
    }
    if (!ok){
        if (tokens[0][0] == nullptr) {
            tokens[0][0] = &token;
            ok=true;
        }
        else{
            throw TokenException("Le plateau est plein");
        }
    }
}

void Board::fillBoard(Bag& bag) {
    if (bag.isEmpty()) {
        throw TokenException("Le sac est vide");
    }
    while(!bag.isEmpty()){
        placeToken(bag.drawToken());
    }
}

const Token& Board::takeToken(const size_t i, const size_t j) {
    if (i >= tokens.size() || j < 0 || j >= tokens[0].size()) {
        throw TokenException("Indice de jeton incorrect");
    }
    if (tokens[i][j] == nullptr) {
        throw TokenException("Il n'y a pas de jeton à cet emplacement");
    }
    const Token& token = *tokens[i][j];
    tokens[i][j] = nullptr;
    return token;
}

const Privilege& Board::takePrivilege() {
    if (getNbPrivileges() == 0) {
        throw TokenException("Il n'y a pas de privilège sur le plateau");
    }
    unsigned int size = getNbPrivileges()-1;
    const Privilege& privilege = *privileges[size];
    privileges[size] = nullptr;
    return privilege;
}

bool Board::isEmpty() const {
    for (const auto & token : tokens) {
        for (auto j : token) {
            if (j != nullptr) {
                return false;
            }
        }
    }
    return true;
}

Board::Board(){
    //On ajoute les privilèges
    for (size_t i = 0; i < TotalPrivileges::getInstance().getNbPrivileges(); i++) {
        placePrivilege(TotalPrivileges::getInstance().getPrivilege(i));
    }
    //On initialise plateau
    for (auto & token : tokens) {
        for (auto & j : token) {
            j = nullptr;
        }
    }
    //On rempli le plateau enb vidant le sac
    fillBoard(*Bag::getInstance());
}

const Token* Board::BoardIterator::next() {
    if (!hasNext()) {
        throw TokenException("Il n'y a plus de jeton");
    }
    const Token* token = board.tokens[row][col];
    col++;
    if (col >= board.tokens[row].size()) {
        col = 0;
        row++;
    }
    return token;
}

bool Board::hasTokenOfColor(TokenColor color) const {
    for (const auto& row : tokens) {
        for (const auto& token : row) {
            if (token && token->getColor() == color) {
                return true;
            }
        }
    }
    return false;
}

bool Board::containsOnly(const TokenColor color) {
    BoardIterator it = iterator();
    while (it.hasNext()) {
        const Token* token = it.next();
        if (token!=nullptr) {
            if (token->getColor() != color) {
                return false;
            }
        }
    }
    return true;
}

unsigned Board::getNbTokens() const {
    unsigned int nb = 0;
    for (const auto& row : tokens) {
        for (const auto& token : row) {
            if (token) {
                nb++;
            }
        }
    }
    return nb;
}

