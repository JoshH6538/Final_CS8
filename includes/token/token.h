#ifndef TOKEN_H
#define TOKEN_H
#include <vector>
#include <iostream>
#include <string>
using namespace std;

enum class TokenType
{
    STRING,
    RELATIONAL,
    LOGICAL,
    AND,
    OR,
    LEFT_PAREN,
    RIGHT_PAREN,
    RECORD_SET
};

class Token
{
public:
    Token();
    Token(string s, TokenType t);
    Token(TokenType t);


    //operators
    friend ostream& operator<<(ostream& outs, Token* t);
    friend ostream& operator<<(ostream& outs, Token t);
    friend string& operator<<(string& outs, Token* t);

    //accessors
    virtual string get_tokenstr();
    TokenType get_type();
    string get_typestr();

    //mutators
    virtual void set_tokenstr(string s);
    void set_type(TokenType t);
private:
    string _token;
    TokenType _type;  
};

#endif