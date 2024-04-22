#include "paren.h"


//----------------------------------------------------------------------------
//                                  L  E  F  T
//----------------------------------------------------------------------------

//-------------------------------------
//              C T O R S
//-------------------------------------

LeftParen::LeftParen():Token()
{

}

LeftParen::LeftParen(string op):Token(op,TokenType::LEFT_PAREN), _token(op) 
{

}

LeftParen::LeftParen(const char* op):Token(op,TokenType::LEFT_PAREN),_token(op)
{

}

//-------------------------------------
//          A C C E S S O R S
//-------------------------------------

string LeftParen::get_tokenstr()
{
    return _token;
}


//-------------------------------------
//          M U T A T O R S
//-------------------------------------

void LeftParen::set_tokenstr(string s)
{
    _token=s;
}

//----------------------------------------------------------------------------
//                                R  I  G  H  T
//----------------------------------------------------------------------------

//-------------------------------------
//              C T O R S
//-------------------------------------

RightParen::RightParen():Token()
{

}

RightParen::RightParen(string op):Token(op,TokenType::RIGHT_PAREN), _token(op) 
{

}

RightParen::RightParen(const char* op):Token(op,TokenType::RIGHT_PAREN),_token(op)
{

}

//-------------------------------------
//          A C C E S S O R S
//-------------------------------------

string RightParen::get_tokenstr()
{
    return _token;
}


//-------------------------------------
//          M U T A T O R S
//-------------------------------------

void RightParen::set_tokenstr(string s)
{
    _token=s;
}