#include "logical.h"

//-------------------------------------
//              C T O R S
//-------------------------------------

Logical::Logical():Token()
{

}

Logical::Logical(string op):Token(op,TokenType::LOGICAL), _token(op) 
{

}

Logical::Logical(const char* op):Token(op,TokenType::LOGICAL),_token(op)
{

}

//-------------------------------------
//          A C C E S S O R S
//-------------------------------------

string Logical::get_tokenstr()
{
    return _token;
}


//-------------------------------------
//          M U T A T O R S
//-------------------------------------

void Logical::set_tokenstr(string s)
{
    _token=s;
}



//-------------------------------------
//              C T O R S
//-------------------------------------

And::And():Token()
{

}

And::And(string op):Token(op,TokenType::AND), _token(op) 
{

}

And::And(const char* op):Token(op,TokenType::AND),_token(op)
{

}

//-------------------------------------
//          A C C E S S O R S
//-------------------------------------

string And::get_tokenstr()
{
    return _token;
}


//-------------------------------------
//          M U T A T O R S
//-------------------------------------

void And::set_tokenstr(string s)
{
    _token=s;
}



//-------------------------------------
//              C T O R S
//-------------------------------------

Or::Or():Token()
{

}

Or::Or(string op):Token(op,TokenType::OR), _token(op) 
{

}

Or::Or(const char* op):Token(op,TokenType::OR),_token(op)
{

}

//-------------------------------------
//          A C C E S S O R S
//-------------------------------------

string Or::get_tokenstr()
{
    return _token;
}


//-------------------------------------
//          M U T A T O R S
//-------------------------------------

void Or::set_tokenstr(string s)
{
    _token=s;
}