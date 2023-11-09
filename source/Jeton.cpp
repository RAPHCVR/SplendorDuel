//
// Created by utcpret on 08/11/2023.
//

#include "Jeton.h"
#include <math.h>

std::string toString(TokenColor c) {
    switch (c) {
        case TokenColor::BLANC: return "Blanc";
        case TokenColor::BLEU: return "Bleu";
        case TokenColor::NOIR: return "Noir";
        case TokenColor::OR: return "Or";
        case TokenColor::PERLE: return "Perle";
        case TokenColor::ROUGE: return "Rouge";
        case TokenColor::VERT: return "Vert";
        default: throw std::invalid_argument("Couleur inconnue");
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
    size_t i = rand() % tokens.size();
    const Token& j = *tokens[i];
    tokens.erase(tokens.begin() + i);
    return j;
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
        privileges.push_back(new Privilege());
    }
}

TotalPrivileges::~TotalPrivileges() {
    for (auto & privilege : privileges) {
        delete privilege;
    }
}

void Board::placePrivilege(const Privilege& privilege) {
    if (privileges.size() >= 3) {
        throw TokenException("Il y a déjà 3 privilèges sur le plateau");
    }
    privileges.push_back(&privilege);
}

void Board::showBoard(){
    std::cout << "Plateau:" << std::endl;

    for (auto & token : tokens) {
        for (auto & j : token) {
            std::string s = "";
            if (j == nullptr) s = "."; // Cas de la case vide
            else {
                switch (j->getColor()) {
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
        }
        std::cout << std::endl;
    }
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
        showBoard();
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
    if (privileges.empty()) {
        throw TokenException("Il n'y a pas de privilège sur le plateau");
    }
    const Privilege& privilege = *privileges.back();
    privileges.pop_back();
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

Board::Board(Bag& bag, const TotalPrivileges& totalPrivileges){
    //On ajoute les les privilèges
    for (size_t i = 0; i < totalPrivileges.getNbPrivileges(); i++) {
        placePrivilege(totalPrivileges.getPrivilege(i));
    }
    //On initialise plateau
    for (auto & token : tokens) {
        for (auto & j : token) {
            j = nullptr;
        }
    }
    fillBoard(bag);
}
