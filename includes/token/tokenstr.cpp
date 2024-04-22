#include "tokenstr.h"

//-------------------------------------
//              C T O R S
//-------------------------------------

TokenStr::TokenStr():Token()
{
    
}

TokenStr::TokenStr(string s):Token(s,TokenType::STRING), _token(s) 
{

}

TokenStr::TokenStr(const char* c):Token(c,TokenType::STRING),_token(c)
{

}

//-------------------------------------
//          A C C E S S O R S
//-------------------------------------

string TokenStr::get_tokenstr()
{
    return _token;
}

//-------------------------------------
//          M U T A T O R S
//-------------------------------------

void TokenStr::set_tokenstr(string s)
{
    _token=s;
}