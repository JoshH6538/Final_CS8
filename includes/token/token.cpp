#include "token.h"

//-------------------------------------
//              C T O R S
//-------------------------------------

Token::Token()
{
    
}

Token::Token(string s, TokenType t):_token(s), _type(t)
{
    
}

Token::Token(TokenType t):_type(t)
{
    
}

//-------------------------------------
//          O P E R A T O R S
//-------------------------------------

ostream& operator<<(ostream& outs, Token* t)
{
    outs<<t->get_tokenstr();
    return outs;
}

ostream& operator<<(ostream& outs, Token t)
{
    outs<<t.get_tokenstr();
    return outs;
}

string& operator<<(string& outs, Token* t)
{
    outs=t->get_tokenstr();
    return outs;
}

//-------------------------------------
//          A C C E S S O R S
//-------------------------------------

string Token::get_tokenstr()
{
    return _token;
}

TokenType Token::get_type()
{
    return _type;
}

string Token::get_typestr()
{
    switch(_type)
    {
        case TokenType::STRING:
            return "STRING";
        case TokenType::RELATIONAL:
            return "RELATIONAL";
        case TokenType::LOGICAL:
            return "LOGICAL";
        default:
            return "INVALID";
    }
}

//-------------------------------------
//          M U T A T O R S
//-------------------------------------

void Token::set_tokenstr(string s)
{
    _token=s;
}
void Token::set_type(TokenType t)
{
    _type=t;
}