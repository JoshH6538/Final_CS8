#include "relational.h"

//-------------------------------------
//              C T O R S
//-------------------------------------

Relational::Relational():Token()
{

}

Relational::Relational(string op):Token(op,TokenType::RELATIONAL), _token(op) 
{

}

Relational::Relational(const char* op):Token(op,TokenType::RELATIONAL),_token(op)
{

}

//-------------------------------------
//          A C C E S S O R S
//-------------------------------------

string Relational::get_tokenstr()
{
    return _token;
}

//-------------------------------------
//          M U T A T O R S
//-------------------------------------

void Relational::set_tokenstr(string s)
{
    _token=s;
}